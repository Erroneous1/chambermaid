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

#include "util/tty.hpp"

#include <iostream>
#include <set>

#ifdef __windows__
	#using <System.dll>
#else
	#include <list>
	#include <boost/filesystem.hpp>
	#include <boost/range/iterator_range.hpp>
#endif

namespace brewpipp {
	namespace util {
		
		std::vector< string > tty::list_serial_ports() {
			std::set<string> ret;
		#ifdef __windows__
			try {
				const auto& ports = System::IO::Ports::GetPortNames();
				ret.reserve(ports.size());
				for(const auto& port : ports) {
					ret.emplace(msclr::interop::marshal_as<string>(port));
				}
			} catch(Win32Exception^ e) {
				std::cerr << "Error enumerating serial ports: " << e->Message << std::endl;
			}
		#else
			try {
				using namespace boost::filesystem;
				path p("/sys/class/tty");
				for(const auto& folder : std::vector<std::string>({"/sys/class/tty"})) {
					if(is_directory(folder)) {
						std::list<string> entries;
						auto dir_itr = directory_iterator(folder);
						for(const auto& entry : boost::make_iterator_range(dir_itr, {})) {
							const auto p = path(entry.path()).append("device");
							if(is_other(p) || is_symlink(p)) {
								path dev("/dev");
								dev.append(entry.path().filename().string<string>());
								if(is_other(dev) || is_symlink(dev)) {
									ret.emplace(canonical(dev).string<string>());
								}
							}
						}
						if(entries.empty()) {
							continue;
						}
					}
				}
			} catch(const std::exception& e) {
				std::cerr << "Error enumerating serial ports: " << e.what() << std::endl;
			}
		#endif
			return std::vector<string>(ret.begin(),ret.end());
		}

	}
}