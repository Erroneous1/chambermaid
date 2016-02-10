#ifndef BREWPIPP_DATA_TYPE_HPP
#define BREWPIPP_DATA_TYPE_HPP

#include "brewpipp_config.h"
#include <string>
#include <memory>

#ifdef BREWPIPP_ODBC
	#include <sql.h>
	#include <sqlext.h>
	#include "SqlError.hpp"
#else
	#ifndef SQLINTEGER
		#include <cstdint>
		typedef uint8_t SQLCHAR
		typedef int8_t SQLSCHAR
		typedef int16_t SQLSMALLINT
		typedef uint16_t SQLUSMALLINT
		typedef int32_t SQLINTEGER
		typedef uint32_t SQLUINTEGER
		typedef int64_t SQLBIGINT
		typedef uint64_t SQLUBIGINT
	#endif
#endif
namespace brewpipp {
	class DataType{
	public:
		DataType();
		
#ifdef BREWPIPP_ODBC
		/// bind column to statementHandle for fetching results
		virtual void bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (SqlError) =0;
#endif
		/// string for SQL (including quotes)
		std::string asSqlString() const;
		
		/// string for JSON (including double-quotes)
		std::string asJsonString() const;
		
		/// copy data to a new DataType child
		std::shared_ptr<DataType> copy() const;
		
		/// SQLBindCol should also use an indicator for null
		bool isNull() const;
		
		virtual ~DataType();

	protected:
		/// string for SQL (including quotes)
		virtual std::string _asSqlString() const =0;
		
		/// string for JSON (including double-quotes)
		virtual std::string _asJsonString() const =0;
		
		virtual std::shared_ptr<DataType> _copy() const =0;
		
		/// will be set to SQL_NO_TOTAL, SQL_NULL_DATA, or size of column data
		SQLLEN indicator;
	};
	
	class VarChar : public DataType{
	public:
		VarChar();
		VarChar(const std::string &data, const uint8_t &maxSize);
		VarChar(const uint8_t &maxSize);
		
		void setMax(const uint8_t &max);
		uint8_t getMax() const;
		
#ifdef BREWPIPP_ODBC
		void bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (SqlError);
#endif
		
		std::string getData() const;
		void setData(const std::string &data);
		
		virtual ~VarChar();
	protected:
		std::string _asSqlString() const;
		std::string _asJsonString() const;

		std::shared_ptr<DataType> _copy() const;
	private:
		uint8_t maxSize;
		std::unique_ptr<char[]> data;
	};
	
	template<typename T>
	class IntT : public DataType{
	public:
		IntT();
		IntT(const T &data);
		
		std::string asSqlString() const;
		std::string asJsonString() const;
		
#ifdef BREWPIPP_ODBC
		void bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (SqlError);
#endif
		
		T getData() const;
		void setData(const T &data);

		virtual ~IntT();
	protected:
		std::string _asSqlString() const;
		std::string _asJsonString() const;
		
		virtual std::shared_ptr<DataType> _copy() const;
	private:
		T data;
	};
	
	typedef IntT<SQLSCHAR> TinyInt;
	typedef IntT<SQLSMALLINT> SmallInt;
	typedef IntT<SQLINTEGER> Int;
	typedef IntT<SQLBIGINT> BigInt;
	typedef IntT<SQLCHAR> TinyIntUnsigend;
	typedef IntT<SQLUSMALLINT> SmallIntUnsigned;
	typedef IntT<SQLUINTEGER> IntUnsigned;
	typedef IntT<SQLUBIGINT> BigIntUnsigned;

	class BoolData : public DataType{
	public:
		BoolData();
		BoolData(const bool &data);
		
#ifdef BREWPIPP_ODBC
		void bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (SqlError);
#endif
		
		bool getData() const;
		void setData(const bool &data);
		
		virtual ~BoolData();
	protected:
		std::string _asSqlString() const;
		std::string _asJsonString() const;
		
		virtual std::shared_ptr<DataType> _copy() const;
	private:
		SQLCHAR data;
	};
}// namespace brewpipp

#endif //ndef BREWPIPP_DATA_TYPE_HPP