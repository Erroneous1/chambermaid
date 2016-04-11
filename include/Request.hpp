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

#ifndef BREWPIPP_REQUEST_HPP
#define BREWPIPP_REQUEST_HPP

#include <string>
#include <map>
#include <fcgi_config.h>
#include <fcgiapp.h>
#include <ctime>
#include <memory>
#include "Options.hpp"
#include "string.hpp"

namespace brewpipp{

	/** Request class to simplify FCGI functions */
	class Request{
	public:
		Request(FCGX_Request &&request, const Options &conf);
		
		/** Parse POST data and return first match
		* @param search case sensitive string
		* @return associated data for search string */
		const string& getPost(const string& search) { return getPosts(search).front(); }
		/// Parse POST data and return all matches as list
		const std::list<string>& getPosts(const string& search);
		
		/** Parse GET data and return first match
		* @param search case sensitive string
		* @return associated data for search string */
		const string& getGet(const string& search){ return getPosts(search).front(); }
		/// Parse GET data and return all matches as list
		const std::list<string>& getGets(const string& search);
		
		/** Parse POST data and return first match. If POST data is empty, parse GET data and return first match.
		* @param search case sensitive string
		* @return associated data for search string */
		const string& getParam(const string& search) { return getParams(search).front(); }
		const std::list<string>& getParams(const string& search);
		
		/// Write out to web server
		FCGX_Stream &out() { return request.out; }
		
		/// Read in from web server
		FCGX_Stream &in() { return request.in; }
		
		/// Create a cookie string
		static string setCookie(const string &name, const string &value, const time_t &expire);
		
		const string& getCookie(const string& search);
	private:
		std::unique_ptr<std::map<string,std::list<string> > > posts;
		std::unique_ptr<std::map<string,std::list<string> > > gets;
		std::unique_ptr<std::map<string,string> > cookies;
		const std::list<string> not_found;
		const string not_found_string;
	protected:
		FCGX_Request request;
		Options conf;
	};

} //namespace brewpipp

#endif //ndef BREWPIPP_REQUEST_HPP