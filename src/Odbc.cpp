#include "Odbc.hpp"
#include <limits>

namespace brewpipp {
	Query &Query::operator<<(const DataType &data){
		*this << data.asSqlString();
		return *this;
	}
	
	Result::Result(SQLHSTMT stmt, const std::vector<std::shared_ptr<DataType> > &returnTypes, const int &line, const std::string &file) throw (SqlError)
	: stmt(stmt), returnTypes(returnTypes), _rowsAffected(0), rowsFetched(0){
		sql_assert(returnTypes.size() <= std::numeric_limits<SQLUSMALLINT>::max(), line, file);
		SQLSMALLINT columnCount;
		sql_succeed(SQLNumResultCols(stmt, &columnCount), stmt, SQL_HANDLE_STMT, line, file);
		sql_assert(columnCount == returnTypes.size(), line, file);
		for(SQLUSMALLINT i = 0; i < returnTypes.size(); i++){
			returnTypes[i]->bindColumn(stmt, i);
		}
		sql_succeed(SQLRowCount(stmt, &_rowsAffected), stmt, SQL_HANDLE_STMT, line, file);
		if(_rowsAffected <= 0){
			SQLFreeHandle(SQL_HANDLE_STMT, stmt);
			stmt = nullptr;
		}
	}
	
	std::vector<std::shared_ptr<DataType> > Result::nextRow() throw (SqlError){
		sql_assert(hasNext(), __LINE__, __FILE__);
		std::vector<std::shared_ptr<DataType> > ret;
		ret.reserve(returnTypes.size());
		sql_succeed(SQLFetch(stmt), stmt, SQL_HANDLE_STMT, __LINE__, __FILE__);
		rowsFetched++;
		for(auto column : returnTypes){
			ret.push_back(column->copy());
		}
		if(!hasNext()){
			SQLFreeHandle(SQL_HANDLE_STMT, stmt);
			stmt = nullptr;
		}
		return ret;
	}

	bool Result::hasNext() const{ return _rowsAffected > rowsFetched; }
	
	Result::~Result(){
		if(stmt){
			SQLFreeHandle(SQL_HANDLE_STMT, stmt);
			stmt = nullptr;
		}
	}

	Odbc::Odbc(const std::string &connectionString) throw (SqlError)
	: env(nullptr), dbc(nullptr), connectionString(connectionString){
#define line __LINE__
#define file __FILE__
		sql_succeed(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env), env, SQL_HANDLE_ENV, line, file);
		sql_succeed(SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0), env, SQL_HANDLE_ENV, line, file);
		sql_succeed(SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc), env, SQL_HANDLE_ENV, line, file);
		sql_succeed(SQLDriverConnect(dbc, nullptr, (SQLCHAR*) connectionString.c_str(), connectionString.length(), nullptr, 0, nullptr, SQL_DRIVER_COMPLETE), dbc, SQL_HANDLE_DBC, line, file);
#undef line
#undef file
	}
	
	Result Odbc::execute(const Query &q, const std::vector<std::shared_ptr<DataType> > &returnTypes, const int &line, const std::string &file) throw (SqlError){
		const std::string statement = q.str();
		sql_assert(statement.length() <= std::numeric_limits<SQLINTEGER>::max(), line, file);
		SQLHSTMT stmt(nullptr);
		sql_succeed(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt), dbc, SQL_HANDLE_DBC, line, file);
		sql_succeed(SQLPrepare(stmt, (SQLCHAR*) statement.c_str(), statement.length()), stmt, SQL_HANDLE_STMT, line, file);
		return Result(stmt, returnTypes, line, file);
	}
	
	bool Odbc::isConnected() const{
		return env && dbc;
	}
	
	Odbc::~Odbc(){
		if(env){
			SQLFreeHandle(SQL_HANDLE_ENV, env);
			env = nullptr;
		}
		if(dbc){
			SQLFreeHandle(SQL_HANDLE_DBC, dbc);
			dbc = nullptr;
		}
	}
} //namespace brewpipp