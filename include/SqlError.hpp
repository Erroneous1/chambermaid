#ifndef BREWPIPP_SQL_ERROR_HPP
#define BREWPIPP_SQL_ERROR_HPP

#include "brewpipp_config.h"
#include <stdexcept>
#include <cassert>
#include <string>

# define sql_assert(expr, line, file)										\
((expr)															\
? __ASSERT_VOID_CAST (0)										\
: throw SqlError(__STRING(expr), line, file))

#ifdef BREWPIPP_ODBC
#include <sql.h>
#include <sqlext.h>


#define sql_succeed(expr, handle, type, line, file)						\
(SQL_SUCCEEDED(expr)										\
? __ASSERT_VOID_CAST (0)									\
: throw SqlError(__STRING(expr), handle, type, line, file))
#else

#endif
namespace brewpipp{
	
class SqlError : public std::runtime_error{
public:
	SqlError(const std::string &operation, SQLHANDLE handle, SQLSMALLINT type, const int &line, const std::string &file);
	SqlError(const std::string &what, const int &line, const std::string &file);
	
	virtual ~SqlError();
};
	
} //namespace brewpipp

#endif //ndef BREWPIPP_SQL_ERROR_HPP