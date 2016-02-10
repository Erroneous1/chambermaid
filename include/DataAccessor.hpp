#ifndef BREWPIPP_DATA_ACCESSOR_HPP
#define BREWPIPP_DATA_ACCESSOR_HPP

#include "brewpipp_config.h"
#include <string>


#include "SqlError.hpp"

namespace brewpipp{
	
	class DataAccessor{
		
	};
	
	class FileAccessor : public DataAccessor{
		
	};
	
	class SqlAccessor : public DataAccessor{
		SqlAccessor(const std::string &dsn) throw (SqlError)
	};
	
}//namespace brewpipp

#endif //ndef BREWPIPP_DATA_ACCESSOR_HPP