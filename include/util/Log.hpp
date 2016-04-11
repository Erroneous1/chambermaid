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

#ifndef BREWPIPP_UTIL_LOG_HPP
#define BREWPIPP_UTIL_LOG_HPP

#include <ostream>
#include <sqlpp11/odbc/connection.h>
#include <boost/thread/shared_mutex.hpp>
#include <sstream>

namespace brewpipp {
	namespace util {
		/// Provides an ostream to use instead of std::cerr that will log to the database
		class Log : public std::basic_ostream<char> {
		public:
			Log(sqlpp::odbc::connection_config& config);
			
			template<typename T>
			Log& operator<<(const T& t) {
				boost::shared_lock<boost::shared_mutex> lock(log_mutex);
				oss << t;
				write_entry(true);
				return *this;
			}
			
			Log& operator<<(std::ostream& (*fun)(std::ostream&)) {
				boost::shared_lock<boost::shared_mutex> lock(log_mutex);
				if(fun == std::endl) {
					write_entry(false);
				} else {
					oss << fun;
				}
				return *this;
			}
		private:
			boost::shared_mutex log_mutex;
			std::ostringstream output;
			sqlpp::odbc::connection db;
			void write_entry(const bool& only_if_full);
		};
	}
}

#endif //BREWPIPP_UTIL_LOG_HPP