#pragma once

namespace ODBCLib {

class CEnvironmentHandle {
// constructor & destructor
public:
	CEnvironmentHandle();
	~CEnvironmentHandle();

// operator
public:
	operator SQLHENV() const {
		return m_environmentHandle;
	};

// public method
public:
	// SQL_OV_ODBC3, SQL_OV_ODBC2, SQL_OV_ODBC3_80
	SQLRETURN setVersion(SQLSMALLINT version);

// public const method
private:
	bool isHandleEnable() const {
		return (m_environmentHandle != SQL_NULL_HANDLE);
	}

// private method
private:
	void FreeHandle();

// private member
private:
	SQLHENV m_environmentHandle;

};	// end of... class CEnvironmentHandle

}	// end of... namespace ODBCLib
