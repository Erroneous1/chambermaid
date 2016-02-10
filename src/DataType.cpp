#include "DataType.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace brewpipp {
	
	std::string DataType::asSqlString() const{
		if(this->isNull()){
			return "NULL";
		}else{
			return this->_asSqlString();
		}
	}
	
	std::string DataType::asJsonString() const{
		if(this->isNull()){
			return "null";
		}else{
			return this->_asJsonString();
		}
	}
	

	bool DataType::isNull() const{
		return indicator == SQL_NULL_DATA;
	}
	
	std::shared_ptr<DataType> DataType::copy() const{
		std::shared_ptr<DataType> ret = this->_copy();
		ret.get()->indicator = indicator;
		return ret;
	}
	
	DataType::DataType() : indicator(SQL_NULL_DATA){}
	
	DataType::~DataType(){}
	
	VarChar::VarChar() : maxSize(255), data(new char[256]){}
	VarChar::VarChar(const std::string &_data, const uint8_t &maxSize) : maxSize(maxSize), data(new char[maxSize+1]){
		assert(_data.size() < size_t(maxSize));
		indicator = _data.size();
		std::copy(_data.begin(), _data.end(), data.get());
	}
	VarChar::VarChar(const uint8_t &_maxSize) : maxSize(_maxSize), data(new char[_maxSize+1]){}
	
	void VarChar::setMax(const uint8_t &max){ 
		if(indicator < max && indicator >=0){
			indicator = max;
		}
		if(maxSize < max){
			std::unique_ptr<char[]> newData(new char[max+1]);
			std::copy(data.get(),data.get()+(indicator < 0 ? 0 : indicator), newData.get());
			std::swap(newData,data);
		}
		maxSize = max; 
	}
	uint8_t VarChar::getMax() const{ return maxSize; }

#ifdef BREWPIPP_ODBC
	void VarChar::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
		SQLBindCol(statementHandle, columnNumber, SQL_C_CHAR, data.get(), (SQLLEN)maxSize+1, (SQLLEN*) &indicator);
	}
#endif

	std::string VarChar::_asSqlString() const{
		std::ostringstream ret;
		ret << '\'';
		char *end;
		if(indicator >= 0){
			end = data.get() + indicator;
		}else{
			assert(indicator != SQL_NULL_DATA);
			end = data.get() + maxSize;
			for(char *c = data.get(); c != end; c++){
				if(c == '\0'){
					end = c;
					break;
				}
			}
		}
			
		for(auto itr = data.get(); itr != end; itr++){
			switch (*itr) {
				case '\'': ret << "''"; break;
				case '\\': ret << "\\\\"; break;
				case '%': ret << "\\%"; break;
				case '_': ret << "\\_"; break;
				case '[': ret << "\\["; break;
				default: ret << *itr; break;
			}
		}
		ret << '\'';
		return ret.str();
	}
	std::string VarChar::_asJsonString() const{
		std::ostringstream ret;
		size_t end;
		if(indicator >= 0){
			end = indicator;
		}else{
			end = 0;
			for(char *c = data.get(); c != data.get() + maxSize && *c; c++){
				end++;
			}
		}
		ret << '"';
		for(auto itr = data.get(); itr != data.get() + end; itr++){
			switch (*itr) {
				case '"': ret << "\\\""; break;
				case '\\': ret << "\\\\"; break;
				case '\b': ret << "\\b"; break;
				case '\f': ret << "\\f"; break;
				case '\n': ret << "\\n"; break;
				case '\r': ret << "\\r"; break;
				case '\t': ret << "\\t"; break;
				default:
					if ('\x00' <= *itr && *itr <= '\x1f') {
						ret << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)*itr;
					} else {
						ret << *itr;
					}
			}
		}
		ret << '"';
		return ret.str();
	}
	
	std::string VarChar::getData() const {
		if(indicator <= 0){
			return std::string();
		}else{
			return std::string(data.get(), size_t(indicator));
		}
	}
	void VarChar::setData(const std::string &_data) {
		indicator = std::min(_data.size(), size_t(maxSize));
		std::copy(_data.begin(), _data.begin()+indicator, data.get());
	}
	
	std::shared_ptr<DataType> VarChar::_copy() const{
		return std::make_shared<VarChar>(getData(), getMax());
	}
	
	VarChar::~VarChar(){}
	
	template<typename T> IntT<T>::IntT() : data(0){}
	template<typename T> IntT<T>::IntT(const T &_data) : data(_data){}
	
#ifdef BREWPIPP_ODBC
	#define line __LINE__
	#define file __FILE__
		template<> void IntT<SQLSCHAR>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_STINYINT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLCHAR>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_UTINYINT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLSMALLINT>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_SSHORT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLUSMALLINT>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_USHORT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLINTEGER>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_SLONG, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLUINTEGER>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_ULONG, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLBIGINT>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_SBIGINT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
		template<> void IntT<SQLUBIGINT>::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
			sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_UBIGINT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, line, file);
		}
	#undef line
	#undef file
#endif
	template<typename T> std::string IntT<T>::_asSqlString() const{ return std::to_string(data); }
	template<typename T> std::string IntT<T>::_asJsonString() const{ return std::to_string(data); }
	
	template<typename T>
	T IntT<T>::getData() const {
		return data;
	}
	template<typename T>
	void IntT<T>::setData(const T &_data) { data = _data; }
	
	template<typename T>
	std::shared_ptr<DataType> IntT<T>::_copy() const{
		return std::make_shared<IntT<T> >(data);
	}
	
	template<typename T>
	IntT<T>::~IntT(){}
	
	template class IntT<SQLSCHAR>;
	template class IntT<SQLSMALLINT>;
	template class IntT<SQLINTEGER>;
	template class IntT<SQLBIGINT>;
	template class IntT<SQLCHAR>;
	template class IntT<SQLUSMALLINT>;
	template class IntT<SQLUINTEGER>;
	template class IntT<SQLUBIGINT>;
	
	
	BoolData::BoolData() : data(false) {}
	BoolData::BoolData(const bool &_data) : data(_data ? 1 : 0) {}
	
	#ifdef BREWPIPP_ODBC
	void BoolData::bindColumn(SQLHSTMT statementHandle, const SQLUSMALLINT &columnNumber) throw (brewpipp::SqlError){
		sql_succeed(SQLBindCol(statementHandle, columnNumber, SQL_C_UTINYINT, &data, sizeof(data), &indicator), statementHandle, SQL_HANDLE_STMT, __LINE__, __FILE__);
	}
	#endif
	
	bool BoolData::getData() const { return data != 0; }
	void BoolData::setData(const bool &_data) { this->data = _data ? 1 : 0; }
	
	std::string BoolData::_asSqlString() const{ return data ? "1" : "0"; }
	std::string BoolData::_asJsonString() const{ return data ? "true" : "false"; }
	
	std::shared_ptr<DataType> BoolData::_copy() const{
		return std::make_shared<BoolData>(data);
	}
	
	BoolData::~BoolData(){}

} //namespace brewpipp