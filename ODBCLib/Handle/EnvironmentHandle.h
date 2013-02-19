#pragma once

#include <Windows.h>
#include <sqltypes.h>

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

// private method
private:
	void FreeHandle();

// private member
private:
	SQLHENV m_environmentHandle;

};	// end of... class CEnvironmentHandle

}	// end of... namespace ODBCLib
