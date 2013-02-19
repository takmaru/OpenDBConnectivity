#pragma once

#include <string>

namespace ODBCLib {

class CConnectionHandle {
// constructor & destructor
public:
	explicit CConnectionHandle(SQLHENV environmentHandle);
	~CConnectionHandle();

// operator
public:
	operator SQLHDBC() const {
		return m_connectionHandle;
	};

// public method
public:
	SQLRETURN connect(const SQLWCHAR* connectionString);
	SQLRETURN disconnect();

	bool StartTransaction();
	bool Commit();
	bool Rollback();

// private method
private:
	void FreeHandle();

// private member
private:
	SQLHDBC m_connectionHandle;

};	// end of... class CConnectionHandle

}	// end of... namespace ODBCLib
