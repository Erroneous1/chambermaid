#ifndef BREWPIPP_CONFIGURATION_HPP
#define BREWPIPP_CONFIGURATION_HPP

#include <map>
#include <string>
#include <stdexcept>

#include "Odbc.hpp"

namespace brewpipp{
		
	class Configuration{
	public:
		Configuration() throw (std::runtime_error);
		
		//Used to read/write data
		DataAccessor &data(){ return dataAccessor; }
		
		bool isSecureOnly() const;
		void setSecureOnly(const bool &data);
		
		bool requiresLogin() const;
		void setRequiresLogin(const bool &data);
	private:
		DataAccessor dataAccessor;
		
		std::map<uint16_t,std::shared_ptr<DataType> > values;
	};
	
}//namespace brewpipp

#endif //ndef BREWPIPP_CONFIGURATION_HPP