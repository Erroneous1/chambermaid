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

#include "util/serial.hpp"
#include <iostream>
#include <boost/thread.hpp>
#include "DeathHandler/death_handler.h"
#include "util/BufferedSerial.hpp"

int main(int argc, char** argv) {
// 	if(argc != 2) {
// 		std::cerr << "Usage: " << argv[0] << " serial_port" << std::endl;
// 		return 2;
// 	}
	Debug::DeathHandler handler;
	std::string port = argc == 2 ? argv[1] : "/dev/ttyACM0";
	try {
		brewpipp::util::BufferedSerial serial(port, 57600);
// 		for(int i = 0; i < 20; i++) {
// 			try {
// 				serial.write("n");
// 				std::cout << "Read: " << serial.readStringUntil(boost::posix_time::seconds(10),"\n") << std::endl;
// 			} catch(const std::exception& e) {
// 				std::cerr << "Error: " << e.what() << std::endl;
// 			}
// 		}
		std::cout << "Version:      " << brewpipp::util::get_version_info(serial).to_string() << std::endl;
		std::cout << "Settings:     " << brewpipp::util::get_control_settings(serial).to_string() << std::endl;
		std::cout << "Constants:    " << brewpipp::util::get_control_constants(serial).to_string() << std::endl;
		std::cout << "Variables:    " << brewpipp::util::get_control_variables(serial).to_string() << std::endl;
		std::cout << "Temperatures: " << brewpipp::util::get_temperatures(serial).to_string() << std::endl;
	} catch(const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}