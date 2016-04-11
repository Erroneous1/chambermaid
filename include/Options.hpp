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

#ifndef BREWPIPP_CONFIGURATION_HPP
#define BREWPIPP_CONFIGURATION_HPP
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/system/system_error.hpp>
#include <sqlpp11/odbc/connection.h>
#include "string.hpp"

namespace brewpipp{
	struct Options{
		boost::filesystem::path conf_file;
		bool secure_only;
		bool require_login;
		sqlpp::odbc::connection_config sql_config;
		Options(const int& argc, const char_t **argv) throw (boost::system::system_error);
		Options() throw (boost::system::system_error);
		static bool is_secure();
	};
	
}//namespace brewpipp

#endif //ndef BREWPIPP_CONFIGURATION_HPP