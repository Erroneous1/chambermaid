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

#include "util/version.hpp"

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

namespace brewpipp {
	namespace util {
		std::string VersionCode::to_string() const {
			std::vector<std::string> components;
			components.reserve(codes.size());
			for(const auto& code : codes) {
				components.push_back(std::to_string(code));
			}
			return boost::algorithm::join(components, ".");
		}
		
		VersionCode::VersionCode(const std::string& s) : codes() {
			std::vector<std::string> v;
			boost::algorithm::split(v, s, boost::is_any_of("."));
			codes.reserve(v.size());
			for(const auto& c : v) {
				codes.push_back(atoi(c.c_str()));
			}
		}
		
		bool VersionCode::operator<(const VersionCode& rhs) const {
			size_t l(0), r(0);
			for(; l < codes.size() && r < rhs.codes.size();) {
				if(codes[l++] < rhs.codes[r++]) {
					return true;
				}
			}
			return r >= rhs.codes.size();
		}

	}
}