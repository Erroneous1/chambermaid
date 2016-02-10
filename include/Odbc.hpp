#ifndef BREWPIPP_ODBC_HPP
#define BREWPIPP_ODBC_HPP

#include <sql.h>
#include <sqlext.h>
#include <SqlError.hpp>
#include <string>
#include <DataType.hpp>
#include <memory>
#include <vector>
#include <queue>
#include <sstream>

namespace brewpipp{
	
	class Query : public std::ostringstream{
		Query &operator<<(const DataType &b);
	};
	
	
	class Result{
	public:
		Result(SQLHSTMT stmt, const std::vector<std::shared_ptr<DataType> > &returnTypes, const int &line, const std::string &file) throw (SqlError);
		std::vector<std::shared_ptr<DataType> > nextRow() throw (SqlError);
		SQLLEN rowsAffected() { return _rowsAffected; }
		bool hasNext() const;
		virtual ~Result();
	private:
		std::vector<std::shared_ptr<DataType> > returnTypes;
		SQLHSTMT stmt;
		bool _hasNext;
		SQLLEN _rowsAffected;
		SQLLEN rowsFetched;
	};
	
	class Odbc{
	public:
		Odbc(const std::string &connectionString) throw (SqlError);
		
		Result execute(const Query &q, const std::vector<std::shared_ptr<DataType> > &returnTypes, const int &line, const std::string &file) throw (SqlError);
		
		bool isConnected() const;
		
		virtual ~Odbc();
	private:
		SQLHENV env;
		SQLHDBC dbc;
		std::string connectionString;
	};
	
} //namespace brewpipp

#endif //ndef BREWPIPP_ODBC_HPP