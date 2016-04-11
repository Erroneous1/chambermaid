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

#include "util/Log.hpp"
#include "tables/logs.hpp"

namespace brewpipp {
	namespace util {
		Log::Log ( sqlpp::odbc::connection_config& config ) : log_mutext(), output(), db(config) {}
		
		void Log::write_entry ( const bool& only_if_full ) {
			tables::Logs logs;
			
			std::ostringstream::streampos size(tables::logs_::Entry::max_length);
			while(size > tables::logs_::Entry::max_length) {
				output.seekp(0, std::ios::end);
				size = output.tellp();
				if(size > tables::logs_::Entry::max_length) {
					output.seekp(0, std::ios::begin);
					std::string current_buffer = output.str();
					const std::string log = current_buffer.substr(0, tables::logs_::Entry::max_length);
					const auto time = std::chrono::system_clock::now();
					output.str(current_buffer.substr(tables::logs_::Entry::max_length));
					db(insert_into(logs).set(logs.timestamp = time, log));
					std::cerr << '[' << time << "] " << log << std::endl;
				}
			}
			if(!only_if_full) {
				output.seekp(0, std::ios::begin);
				const std::string log = output.str();
				output.str(std::string());
				const auto time = std::chrono::system_clock::now();
				output.str(current_buffer.substr(tables::logs_::Entry::max_length));
				db(insert_into(logs).set(logs.timestamp = time, log));
				std::cerr << '[' << time << "] " << log << std::endl;
			}
		}


	}
}