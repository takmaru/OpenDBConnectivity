#pragma once

#include "ODBCHandle.h"

namespace ODBCLib {

namespace Handle {

class CEnvironmentHandle : public CODBCHandle {
public:
	CEnvironmentHandle();
	virtual ~CEnvironmentHandle();
public:
	virtual bool SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength);
};

}	// namespace Handle

}	// namespace ODBCLib

