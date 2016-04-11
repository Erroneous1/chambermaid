/*
 * Distrubuted under The MIT License (MIT)
 *
 * Copyright (c) 2016 Aaron Bishop
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Options.hpp"
#include <sqlpp11/odbc/connection.h>
#include <boost/thread.hpp>
#include <BufferedAsyncSerial.h>
#include <array>
#include <vector>
#include "tables.hpp"
#include "util/serial.hpp"
#include "util/Log.hpp"

constexpr const char compatible_fw_version = "0.2.4";
constexpr unsigned int baud_rate = 57600;

std::map<brewpipp::tables::Groups::group_id_t,BufferedAsyncSerial> serial_ports;
std::map<brewpipp::tables::Groups::group_id_t,boost::shared_mutex> serial_port_mutexes;
std::map<std::string,bool> serial_ports_in_use;
boost::shared_mutex serial_ports_in_use_mutex;
boost::asio::io_service main_io_service;
std::unique_ptr<brewpipp::util::Log> log;


void init_tables(sqlpp::odbc::connection& db);
void init_lcd_lines(sqlpp::odbc::& db);
void monitor_group(const sqlpp::odbc::connection_config& config, const brewpipp::tables::Groups::group_id_t& group_id);
std::string get_serial_port(const brewpipp::tables::Groups::group_id_t& group_id, sqlpp::odbc::connection& db);

int main(int argc, char** argv) {
	try {
		brewpipp::Options ops(argc, argv);
		sqlpp::odbc::connection db(ops.sql_config);
		log.reset(new brewpipp::util::Log(ops.sql_config));
		init_tables(db);
		init_lcd_lines(db);
		
	} catch( const std::exception& e ) {
		std::cerr << "Exiting due to error: " << e.what() << std::endl;
		return 1;
	}
}

void init_tables( sqlpp::odbc::connection& db ) {
	for(const auto& create : brewpipp::tables::list_creates()) {
		db.execute(create);
	}
}

void init_lcd_lines( sqlpp::odbc::connection& db, const sqlpp::odbc::connection_config& config ) {
	brewpipp::tables::Groups groups;
	brewpipp::tables::LCD lcd;
	
	auto select_groups = db(select(groups.group_id).from(groups).where(true));
	for(const auto& row : select_groups) {
		auto select_lines = db(select(lcd.group_id).from(lcd).where(lcd.group_id == row.group_id));
		if(select_lines.empty()) {
			db(insert_into(lcd).set(lcd.group_id = row.group_id,
									lcd.lcd_line_1 = "Daemon starting up",
									lcd.lcd_line_2 = std::string(),
									lcd.lcd_line_3 = std::string(),
									lcd.lcd_line_4 = std::string()));
		} else {
			db(update(lcd).set(lcd.lcd_line_1 = "Daemon starting up",
							   lcd.lcd_line_2 = std::string(),
							   lcd.lcd_line_3 = std::string(),
							   lcd.lcd_line_4 = std::string()).where(lcd.group_id = row.group_id));
		}
		boost::thread(monitor_group, config, row.group_id).detach();
	}
}

void monitor_group(const sqlpp::odbc::connection_config& config, const brewpipp::tables::Groups::group_id_t& group_id) {
	std::string serial_port_location;
	try {
		sqlpp::odbc::connection db(config);
		//Grab the serial port device
		serial_port_location = get_serial_port(group_id, db);
		if(serial_port_location.empty()) {
			throw std::runtime_error("No serial port defined");
		}
		
		//Ensure no other groups happen to use the same serial port location
		boost::unique_lock<boost::shared_mutex> serial_location_lock(serial_ports_in_use_mutex,std::chrono::duration<std::chrono::seconds>(10));
		if(!serial_location_lock.owns_lock()) {
			throw std::runtime_error("Group " + std::to_string(group_id) + " trying to access serial port " + serial_port_location + ", failed to lock serial location mutex");
		}
		if(serial_ports_in_use[serial_port_location]) {
			throw std::runtime_error("Group " + std::to_string(group_id) + " trying to access serial port " + serial_port_location + ", but that port is already in use");
		}
		serial_ports_in_use[serial_port_location] = true;
		serial_location_lock.unlock();
		serial_location_lock.release();
		
		//Lock the serial port to this group
		boost::unique_lock<boost::shared_mutex> serial_lock(serial_port_mutexes[group_id],std::chrono::duration<std::chrono::seconds>(10));
		if(!serial_lock.owns_lock()) {
			throw std::runtime_error("Group " << group_id << " timed out while grabbing serial port lock" << std::endl;
		}
		serial_ports[group_id] = BufferedAsyncSerial(serial_port_location, baud_rate);
		auto& serial_port = serial_ports[group_id];
		
		const auto version = brewpipp::util::get_version_info(serial_port);
		if(version.version < brewpipp::util::VersionCode("0.0.0.0")) {
			*log << "Could not connect to device at " << serial_port_location << std::endl;
			return;
		}
		*log << "Conntected to " << version.to_string() << std::endl;
		if(version.version < brewpipp::util::VersionCode(compatible_fw_version)) {
			*log << "Version " << version.version.to_string() << " below minimum of " << compatible_fw_version << ", attempting upgrade" << std::endl;
		}
	} catch( const std::exception& e ) {
		*log << e.what() << std::endl;
		*log << "No longer monitoring Group " << group_id << std::endl;
	}
}

std::string get_serial_port(const brewpipp::tables::Groups::group_id_t& group_id, sqlpp::odbc::connection& db) {
	using namespace brewpipp::tables;
	Devices devices;
	for(const auto& row : db(select(devices.device_location).from(devices).where(devices.device_type = static_cast<Devices::device_type_t>(Devices::DeviceType::SerialPort)))) {
		return row.device_location;
	}
	return std::string();
}