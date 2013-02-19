#pragma once

#include "ODBCHandle.h"

namespace ODBCLib {

class CEnvironmentHandle : public CODBCHandle {
// constructor & destructor
public:
	CEnvironmentHandle();
	~CEnvironmentHandle();

// operator
public:
	operator SQLHENV() const {
		return m_handle;
	};

// public method
public:
	// SQL_OV_ODBC3, SQL_OV_ODBC2, SQL_OV_ODBC3_80
	SQLRETURN setVersion(SQLSMALLINT version);

// public const method
private:
};	// end of... class CEnvironmentHandle

}	// end of... namespace ODBCLib
