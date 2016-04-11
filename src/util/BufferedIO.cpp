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

#include "util/BufferedIO.hpp"
#include "util/AsyncImpl.hpp"
#include <boost/thread.hpp>
#include <iostream>

namespace brewpipp {
	namespace util {
		typedef boost::lock_guard<boost::mutex> lock_t;
		
		BufferedIO::BufferedIO(std::shared_ptr<AsyncImpl>&& impl) : pimpl(impl){}
		
		void BufferedIO::open() throw (boost::system::system_error) {
			pimpl->open();
		}

		void BufferedIO::close() throw(boost::system::system_error) {
			pimpl->close();
		}
		
		std::string BufferedIO::readStringUntil(const boost::posix_time::time_duration& timeout, const std::string& delim)
			throw(boost::system::system_error, TimeoutException) {
			for(const auto end_time = boost::posix_time::microsec_clock::universal_time() + timeout;
				boost::posix_time::microsec_clock::universal_time() < end_time;
			) {
				const auto read_str = pimpl->readStringUntil(delim);
				if(read_str) {
					return std::string(*read_str);
				}
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			}
			throw TimeoutException();
		}
		
		void BufferedIO::write(const char* data_begin, const char* data_end) throw (boost::system::system_error) {
			pimpl->write(data_begin, data_end);
		}
		
		BufferedIO::~BufferedIO() {
			try {
				close();
			} catch(...) {}
		}
	}
}