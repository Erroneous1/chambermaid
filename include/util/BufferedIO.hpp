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

#ifndef BREWPIPP_UTIL_BUFFERED_IO_HPP
#define BREWPIPP_UTIL_BUFFERED_IO_HPP

#include <memory>
#include <cstring>
#include <boost/system/system_error.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace brewpipp {
	namespace util {
		class AsyncImpl;
		
		class BufferedIO {
		public:
			class TimeoutException : public std::exception {
			public:
				TimeoutException() = default;
				const char* what() const throw() { return "Operation Timed Out"; }
			};
			
			//Async statements
			template<intptr_t N>
			void write(const char (&data) [N]) throw(boost::system::system_error) { write(data, data+N-1); }
			
			///Write data and return immediately
			void write(const char* data_begin, const char* data_end) throw(boost::system::system_error);
			
			///Wait until delim has been read or timeout has been reached
			std::string readStringUntil(const boost::posix_time::time_duration& timeout, const std::string& delim="\n") throw(boost::system::system_error, TimeoutException);
			
			void close() throw(boost::system::system_error);
			
			bool isOpen() const;
			
			void open() throw(boost::system::system_error);

			boost::system::error_code errorStatus() const;
			
			virtual ~BufferedIO();
			
		protected:
			BufferedIO(std::shared_ptr<AsyncImpl>&& impl);
			std::shared_ptr<AsyncImpl> pimpl;
		};
	}
}

#endif //BREWPIPP_UTIL_BUFFERED_IO_HPP