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

/* 
 * Much of this code was altered from AsyncSerial and BufferedAsyncSerial by 
 * Terraneo Federico which was distributed under the Boost Software License, Version 1.0.
 */

#include "util/BufferedSerial.hpp"
#include "util/AsyncImpl.hpp"

namespace brewpipp {
	namespace util {
			class SerialImpl : public AsyncImpl {
			public:
				SerialImpl();
				SerialImpl(const std::string& devname,
							const unsigned int& baud,
							const boost::asio::serial_port_base::parity& opt_parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
							const boost::asio::serial_port_base::character_size& opt_csize = boost::asio::serial_port_base::character_size(8),
							const boost::asio::serial_port_base::flow_control& opt_flow = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
							const boost::asio::serial_port_base::stop_bits& opt_stop = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one))
					throw(boost::system::system_error);

				void open(const std::string& devname,
							const unsigned int& baud,
							const boost::asio::serial_port_base::parity& opt_parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
							const boost::asio::serial_port_base::character_size& opt_csize = boost::asio::serial_port_base::character_size(8),
							const boost::asio::serial_port_base::flow_control& opt_flow = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
							const boost::asio::serial_port_base::stop_bits& opt_stop = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one))
					throw(boost::system::system_error);
			
			protected:
				virtual void _configure() throw(boost::system::system_error);
				virtual void _close() throw(boost::system::system_error);
				
				virtual void _asyncWrite();
				virtual void _asyncRead();
			private:
				boost::asio::serial_port serial;
				std::string device_name;
				unsigned int baud_rate;
				boost::asio::serial_port_base::parity parity;
				boost::asio::serial_port_base::character_size character_size;
				boost::asio::serial_port_base::flow_control flow_control;
				boost::asio::serial_port_base::stop_bits stop_bits;
			};
		
			SerialImpl::SerialImpl() : 
				AsyncImpl(),
				serial(io),
				device_name(),
				baud_rate(9600),
				parity(boost::asio::serial_port_base::parity::none),
				character_size(8), flow_control(boost::asio::serial_port_base::flow_control::none),
				stop_bits(boost::asio::serial_port_base::stop_bits::one)
				{}
			
			SerialImpl::SerialImpl(const std::string& devname,
								   const unsigned int& baud, 
								   const boost::asio::serial_port_base::parity& opt_parity, 
								   const boost::asio::serial_port_base::character_size& opt_csize,
								   const boost::asio::serial_port_base::flow_control& opt_flow,
								   const boost::asio::serial_port_base::stop_bits& opt_stop)
					throw(boost::system::system_error) :
				AsyncImpl(),
				serial(io),
				device_name(devname),
				baud_rate(baud),
				parity(opt_parity),
				character_size(opt_csize),
				flow_control(opt_flow),
				stop_bits(opt_stop)
				{
				AsyncImpl::open();
			}
			
			void SerialImpl::open(const std::string& devname,
					  const unsigned int& baud,
					  const boost::asio::serial_port_base::parity& opt_parity,
					  const boost::asio::serial_port_base::character_size& opt_csize,
					  const boost::asio::serial_port_base::flow_control& opt_flow,
					  const boost::asio::serial_port_base::stop_bits& opt_stop)
					throw(boost::system::system_error) {
				device_name = devname;
				baud_rate = baud;
				parity = opt_parity;
				character_size = opt_csize;
				flow_control = opt_flow;
				stop_bits = opt_stop;
				AsyncImpl::open();
			}
			
			void SerialImpl::_configure() throw(boost::system::system_error) {
				serial.open(device_name);
				serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
				serial.set_option(parity);
				serial.set_option(character_size);
				serial.set_option(flow_control);
				serial.set_option(stop_bits);
				if(serial.is_open()) {
					is_open.store(true);
				} else {
					is_open.store(false);
					const boost::system::error_code ec(-1, boost::system::get_generic_category());
					error.store(ec);
					throw boost::system::system_error(ec, "Unable to open "+device_name);
				}
			}
			void SerialImpl::_close() throw(boost::system::system_error) {
				serial.close();
				is_open.store(false);
			}
			
			void SerialImpl::_asyncWrite() {
				boost::asio::async_write(serial, boost::asio::buffer(write_buffer.get(), write_buffer_size), boost::bind(&AsyncImpl::finishWrite, static_cast<AsyncImpl*>(this), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
			
			void SerialImpl::_asyncRead() {
				serial.async_read_some(boost::asio::buffer(read_buffer.data(), read_buffer_size), boost::bind(&AsyncImpl::finishRead, static_cast<AsyncImpl*>(this), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}

			BufferedSerial::BufferedSerial() : BufferedIO(std::make_shared<SerialImpl>()) {}
			
			BufferedSerial::BufferedSerial(const std::string& devname,
										const unsigned int& baud,
										const boost::asio::serial_port_base::parity& opt_parity,
										const boost::asio::serial_port_base::character_size& opt_csize,
										const boost::asio::serial_port_base::flow_control& opt_flow,
										const boost::asio::serial_port_base::stop_bits& opt_stop
										) throw(boost::system::system_error) : BufferedIO(std::make_shared<SerialImpl>(devname, baud, opt_parity, opt_csize, opt_flow, opt_stop)) {}

			void BufferedSerial::open(const std::string& devname,
								const unsigned int baud,
								const boost::asio::serial_port_base::parity& opt_parity,
								const boost::asio::serial_port_base::character_size& opt_csize,
								const boost::asio::serial_port_base::flow_control& opt_flow,
								const boost::asio::serial_port_base::stop_bits& opt_stop
								) throw(boost::system::system_error) {
				std::static_pointer_cast<SerialImpl>(pimpl)->open(devname, baud, opt_parity, opt_csize, opt_flow, opt_stop);
			}

	}
}