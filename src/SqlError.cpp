#include "SqlError.hpp"
#include <memory>
#include <sstream>
#include <algorithm>
#include <sql.h>
#include <sqlext.h>

namespace brewpipp {
	const char *buildSqlError(const std::string &operation, SQLHANDLE handle, SQLSMALLINT type, const int &line, const std::string &file){
		std::unique_ptr<SQLCHAR[]> buffer(new SQLCHAR[256]);
		std::unique_ptr<SQLCHAR[]> state(new SQLCHAR[7]);
		std::ostringstream out;
		SQLINTEGER i(0);
		SQLINTEGER native(0);
		SQLSMALLINT len(0);
		
		out << "SqlError at " << file << '@' << line << " while running" << std::endl << operation;
		
		while(SQL_SUCCEEDED(SQLGetDiagRec(type, handle, ++i, state.get(), &native, buffer.get(), 256, &len))){
			out << std::endl << "State: " << state.get() << ", " << i << ':' << native << ", " << std::string((const char *)buffer.get(), (size_t)std::min(len,(SQLSMALLINT)256));
		}
		return out.str().c_str();
	}
	
	SqlError::SqlError(const std::string &operation, SQLHANDLE handle, SQLSMALLINT type, const int &line, const std::string &file) : std::runtime_error(buildSqlError(operation, handle, type, line, file)) {}

	SqlError::SqlError(const std::string &what, const int &line, const std::string &file) : std::runtime_error(std::string("SqlError at "+file+'@'+std::to_string(line)+": "+what).c_str()){}
	
	SqlError::~SqlError(){}
}
