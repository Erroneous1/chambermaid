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

#include "string.hpp"

namespace brewpipp {
#ifdef __windows__
#include <iostream>
#include <clocale>
#include <locale>
#include <vector>
#include <limits>

std::setlocale(LC_ALL,"");
const std::locale locale("");
typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;

std::string to_string(const string& s){
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> ret(s.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t *from_next;
	char *to_next;
	const converter_type::result result = converter.out(state,
														ws.data(),
														ws.data() + ws.length(), 
														from_next, 
													 &ret[0], 
													 &ret[0] + ret.size(), 
														to_next);
	if(result == converter_type::ok || result == converter_type::no_conv){
		return std::string(ret.begin(), ret.end());
	}else{
		std::string retS;
		retS.reserve(s.length());
		for(auto c : s){
			if(c >= std::numeric_limits<char>::min && c <= std::numeric_limits<char>::max){
				retS.push_back((char)c);
			}
		}
		return retS;
	}
}
#endif

} //namespace brewpipp