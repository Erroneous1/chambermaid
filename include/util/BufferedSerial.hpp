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

#ifndef BREWPIPP_UTIL_BUFFERED_SERIAL_HPP
#define BREWPIPP_UTIL_BUFFERED_SERIAL_HPP

#include "util/BufferedIO.hpp"
#include <boost/asio/serial_port_base.hpp>

namespace brewpipp {
	namespace util {
		class BufferedSerial : public BufferedIO {
		public:
			BufferedSerial();
			BufferedSerial(const std::string& devname,
										const unsigned int& baud,
										const boost::asio::serial_port_base::parity& opt_parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
										const boost::asio::serial_port_base::character_size& opt_csize = boost::asio::serial_port_base::character_size(8),
										const boost::asio::serial_port_base::flow_control& opt_flow = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
										const boost::asio::serial_port_base::stop_bits& opt_stop = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one)
										) throw(boost::system::system_error);
			void open(const std::string& devname,
								const unsigned int baud,
								const boost::asio::serial_port_base::parity& opt_parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
								const boost::asio::serial_port_base::character_size& opt_csize = boost::asio::serial_port_base::character_size(8),
								const boost::asio::serial_port_base::flow_control& opt_flow = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
								const boost::asio::serial_port_base::stop_bits& opt_stop = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one)
								) throw(boost::system::system_error);
		};
	}
}

#endif //BREWPIPP_UTIL_BUFFERED_SERIAL_HPP