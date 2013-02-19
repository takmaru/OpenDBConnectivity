#pragma once

#include "ODBCHandle.h"

namespace ODBCLib {

class CConnectionHandle : public CODBCHandle {
// constructor & destructor
public:
	explicit CConnectionHandle(SQLHENV environmentHandle);
	~CConnectionHandle();

// operator
public:
	operator SQLHDBC() const {
		return static_cast<SQLHDBC>(m_handle);
	};

// public method
public:
	SQLRETURN connect(const SQLWCHAR* connectionString);
	SQLRETURN disconnect();

	bool StartTransaction();
	bool Commit();
	bool Rollback();
};	// end of... class CConnectionHandle

}	// end of... namespace ODBCLib
