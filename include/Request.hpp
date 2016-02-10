#ifndef BREWPIPP_REQUEST_HPP
#define BREWPIPP_REQUEST_HPP

#include <string>
#include <map>
#include <fcgi_config.h>
#include <fcgiapp.h>
#include <ctime>
#include <memory>
#include "Configuration.hpp"

namespace brewpipp{

	/** Request class to simplify FCGI functions */
	class Request{
	public:
		Request(FCGX_Request &&request, const Configuration &conf);
		
		/** Parse POST data and return first match
		* @param search case sensitive string
		* @return associated data for search string */
		std::string getPost(const std::string &search);
		/** Parse GET data and return first match
		* @param search case sensitive string
		* @return associated data for search string */
		std::string getGet(const std::string &search);
		/** Parse POST data and return first match. If POST data is empty, parse GET data and return first match.
		* @param search case sensitive string
		* @return associated data for search string */
		std::string getParam(const std::string &search);
		
		/// Write out to web server
		FCGX_Stream &out();
		
		/// Read in from web server
		FCGX_Stream &in();
		
		/// Create a cookie string
		std::string setCookie(const std::string &name, const std::string &value, const time_t &expire);
		
	private:
		std::unique_ptr<std::map<std::string,std::string> > post;
		std::unique_ptr<std::map<std::string,std::string> > get;
		std::unique_ptr<std::map<std::string,std::string> > cookies;
	protected:
		FCGX_Request request;
	}

} //namespace brewpipp

#endif //ndef BREWPIPP_REQUEST_HPP