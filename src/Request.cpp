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

#include "Request.hpp"
#include <qdecoder.h>

namespace brewpipp {
	inline const std::list<string>& request_find(const std::map<string,std::list<string> >& from, const string& search, const std::list<string>& empty) {
		const auto ret = from.find(search);
		if(ret == from.end() || ret->empty()) {
			return empty;
		} else {
			return *from;
		}
	}
	
	const std::list< string >& Request::getGets ( const string& search ) { return request_find(gets, search, not_found); }
	
	const std::list< string >& Request::getParams ( const string& search ) {
		const auto ret1 = posts.find(search);
		if(ret1 == posts.end() || ret1->empty()) {
			const auto ret2 = gets.find(search);
			if(ret2 == gets.end() || ret2->empty()) {
				return not_found;
			} else {
				return *ret2;
			}
		} else {
			return *ret1;
		}
	}
	
	const std::list< string >& Request::getPosts ( const string& search ) { return request_find(posts, search, not_found); }
	
	Request::Request ( FCGX_Request&& request_, const Options& conf_ ) : 
		//private:
		posts(), gets(), cookies(), not_found({string()}), not_found_string(),
		//protected:
		request(request_), conf(conf_)
	{
		
	}
	
	string Request::setCookie ( const string& name, const string& value, const time_t& expire ) {
		
	}

}