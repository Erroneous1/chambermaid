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
#include "brewpipp_config.hpp"
#include <iostream>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <memory>
#ifdef __windows__
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <pwd.h>
	#include <termios.h>
#endif

#include <sqlpp11/odbc/odbc.h>

namespace brewpipp {
	boost::filesystem::path optionsGetConfigFile(const boost::filesystem::path& conf_file) throw(boost::system::system_error);
	std::map<string,sqlpp::odbc::connection_config::ODBC_Type> get_types();
	
	std::map<string,sqlpp::odbc::connection_config::ODBC_Type> get_types() {
		return std::map<string,sqlpp::odbc::connection_config::ODBC_Type>({
			{BC("MySQL"),sqlpp::odbc::connection_config::ODBC_Type::MySQL},
			{BC("PostgreSQL"),sqlpp::odbc::connection_config::ODBC_Type::PostgreSQL},
			{BC("SQLite3"),sqlpp::odbc::connection_config::ODBC_Type::SQLite3},
			{BC("TSQL"),sqlpp::odbc::connection_config::ODBC_Type::TSQL}
		});
	}
	
	void set_console_echo(bool enable){
		#ifdef __windows__
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
			DWORD mode;
			GetConsoleMode(hStdin, &mode);
			
			if( !enable )
				mode &= ~ENABLE_ECHO_INPUT;
			else
				mode |= ENABLE_ECHO_INPUT;
			
			SetConsoleMode(hStdin, mode );
		#else
			struct termios tty;
			tcgetattr(STDIN_FILENO, &tty);
			if( !enable )
				tty.c_lflag &= ~ECHO;
			else
				tty.c_lflag |= ECHO;
			
			(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
		#endif
	}
	Options::Options() throw (boost::system::system_error) 
	: conf_file(), secure_only(false), require_login(true), sql_config() {
		conf_file = optionsGetConfigFile(conf_file);
		
		boost::filesystem::ifstream confStream(conf_file);
		if(!confStream.is_open()){
			throw boost::system::system_error(boost::system::error_code(boost::system::errc::io_error,
																		boost::system::get_generic_category()),
											  "Could not open "+conf_file.string());
		}
		boost::property_tree::ptree props;
		boost::property_tree::ini_parser::read_ini(confStream, props);
		const auto odbc_types = get_types();
		for(const auto& v : props){
			if(v.first.compare("SECURE_ONLY")==0){
				secure_only = v.second.get_value<bool>();
			}else if(v.first.compare("REQUIRE_LOGIN")==0){
				require_login = v.second.get_value<bool>();
			}else if(v.first.compare("DB.TYPE")==0){
				std::string s = v.second.get_value<std::string>();
				const auto found = odbc_types.find(s);
				if(found == odbc_types.end()) {
					throw boost::system::system_error(boost::system::error_code(boost::system::errc::invalid_argument,
																				boost::system::get_generic_category()),
													  "Unknown Database Type: " + s);
				} else {
					sql_config.type = found->second;
				}
			}else if(v.first.compare("DB.DATABASE")==0){
				sql_config.database = v.second.get_value<std::string>();
			}else if(v.first.compare("DB.USERNAME")==0){
				sql_config.username = v.second.get_value<std::string>();
			}else if(v.first.compare("DB.PASSWORD")==0){
				sql_config.password = v.second.get_value<std::string>();
			}else if(v.first.compare("DB.DSN")==0){
				sql_config.data_source_name = v.second.get_value<std::string>();
			}else{
				throw boost::system::system_error(boost::system::error_code(boost::system::errc::invalid_argument,
																			boost::system::get_generic_category()),
												  "Unknown parameter: "+v.first);
			}
		}
	}
	
	bool Options::is_secure() {
		auto https_c = getenv("HTTPS");
		if(!https_c) {
			return false;
		}
		return std::string(https_c).compare("on")==0;
	}
	
	inline void throw_arg_required(const std::string& s) {
		throw boost::system::system_error(boost::system::error_code(boost::system::errc::invalid_argument,
																	boost::system::get_generic_category()),
										  "Argument required" + s);
	}
	
	Options::Options(const int& argc, const char_t **argv) throw (boost::system::system_error)
		: Options() {
		for(const char_t **argItr = argv+1, **argEnd =argc+argv; argItr != argEnd; argItr++) {
			string flag(*argItr);
			if(!flag.empty()) {
				if(flag.compare(BC("-conf"))==0) {
					if(++argItr != argEnd) {
						conf_file = *argItr;
					} else {
						throw_arg_required("-conf");
					}
				} else if(flag.compare(BC("-secure"))==0) {
					secure_only = true;
				} else if(flag.compare(BC("-insecure"))==0) {
					secure_only = false;
				} else if(flag.compare(BC("-anonymous"))==0) {
					require_login = false;
				} else if(flag.compare(BC("-login"))==0) {
					require_login = true;
				} else if(flag.compare(BC("-type"))==0) {
					if(++argItr != argEnd) {
						string s(*argItr);
						const auto odbc_types = get_types();
						const auto found = odbc_types.find(s);
						if(found == odbc_types.end()) {
							throw boost::system::system_error(boost::system::error_code(boost::system::errc::invalid_argument,
																						boost::system::get_generic_category()),
															  "Unknown Database Type: " + to_string(s));
						} else {
							sql_config.type = found->second;
						}
					}else{
						throw_arg_required("-type");
					}
				}else if(flag.compare("-u")==0){
					if(++argItr != argEnd){
						sql_config.username = to_string(*argItr);
					}else{
						throw_arg_required("-u");
					}
				}else if(flag.compare("-P")){
					std::unique_ptr<char[]> buffer(new char[256]);
					std::cout << "Please enter DB password: ";
					set_console_echo(false);
					std::cin.getline(buffer.get(), 256);
					set_console_echo(true);
					sql_config.password = buffer.get();
				}else if(flag.compare("-dsn")==0){
					if(++argItr != argEnd) {
						sql_config.data_source_name = to_string(*argItr);
					}else{
						throw_arg_required("-dsn");
					}
				}else if(flag.compare("-db")==0) {
					if(++argItr != argEnd) {
						sql_config.database = to_string(*argItr);
					}else{
						throw_arg_required("-db");
					}
				}else if(flag.compare("--debug")==0) {
					sql_config.debug = true;
				}else{
					throw boost::system::system_error(boost::system::error_code(boost::system::errc::invalid_argument,
																				boost::system::get_generic_category()),
													  "Invalid Argument: "+to_string(flag));
				}
			}
		}
	}
	
	boost::filesystem::path optionsGetConfigFile(const boost::filesystem::path& conf_file) throw(boost::system::system_error){
		if(!conf_file.empty()){
			return conf_file;
		}
#ifdef __windows__ //Look in install dir, then %APPDATA%
		std::unique_ptr<char_t[]> szPath(new char_t[MAX_PATH]);
		if(GetModuleFileNameW(nullptr, szPath.get(), MAX_PATH)){
			boost::filesystem::path moduleFileName = boost::filesystem::path(szPath.get()).stem().append(BREWPIPP_CONF_NAME);
			if(boost::filesystem::is_regular_file(moduleFileName)){
				return moduleFileName;
			}
		}
		if(_wdupeenv_s(szPath.get(), MAX_PATH, BC"APPDATA") == 0){
			boost::filesystem::path appdata = boost::filesystem::path(szPath.get()).stem().append(BREWPIPP_CONF_NAME);
			if(boost::filesystem::is_regular_file(appdata)){
				return appdata;
			}
		}
#else //Look in /etc then ${HOME} then ~user
		if(boost::filesystem::is_regular_file("/etc/" BREWPIPP_CONF_NAME)){
			return "/etc/" BREWPIPP_CONF_NAME;
		}
		char *conf_env = getenv("CONF_FILE");
		char *home_env = getenv("HOME");
		if(home_env != nullptr && boost::filesystem::is_regular_file(boost::filesystem::path(home_env).append(BREWPIPP_CONF_NAME))){
			return boost::filesystem::path(home_env).append(BREWPIPP_CONF_NAME);
		}else{
			std::unique_ptr<struct passwd> pw(new struct passwd);
			auto buffer_size = sysconf(_SC_GETPW_R_SIZE_MAX);
			if(buffer_size < 1){
				buffer_size = 16384;
			}
			struct passwd *result(nullptr);
			std::unique_ptr<char[]> buffer(new char[buffer_size]);
			
			if(getpwuid_r(getuid(),pw.get(),buffer.get(), buffer_size, &result) == 0 && result != nullptr){
				boost::filesystem::path home = boost::filesystem::path(pw->pw_dir).append(BREWPIPP_CONF_NAME);
				if(boost::filesystem::is_regular_file(home)){
				   return home;
				}
			}
		}
#endif
		//look in runtime directory
		if(boost::filesystem::is_regular_file(BREWPIPP_CONF_NAME)){
			return BREWPIPP_CONF_NAME;
		}else{
			throw boost::system::system_error(boost::system::error_code(boost::system::errc::no_such_file_or_directory,
																		boost::system::get_generic_category()),
											  "Could not find " BREWPIPP_CONF_NAME);
		}
	}
} //namespace brewpipp