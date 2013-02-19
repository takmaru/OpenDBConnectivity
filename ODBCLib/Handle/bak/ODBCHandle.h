#pragma once

#include <string>

#include <Windows.h>

#include <sql.h>
#include <sqltypes.h>

#include "ODBCErrorInfo.h"

namespace ODBCLib {

namespace Handle {

class CODBCHandle {
// constructor & destructor
public:
	explicit CODBCHandle(SQLSMALLINT HandleType);
	virtual ~CODBCHandle();

public:
	virtual bool SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength) = 0;

protected:
	bool Create(SQLSMALLINT ParentHandleType, SQLHANDLE ParentHandle);
	void Destroy();
	bool IsCreate() const {return (m_SQLHandle != SQL_NULL_HANDLE);};

	void SetLastErrorInfos(SQLSMALLINT ParentHandleType, SQLHANDLE ParentHandle);

	bool SetResult(SQLRETURN Result);
public:
	SQLHANDLE GetHandle(){return m_SQLHandle;}
	void GetLastErrorInfo(SQLRETURN& LastResult, ODBC_LASTERRINFO_LIST* ErrorInfos) const
	{
		LastResult = m_LastResult;
		if(ErrorInfos != NULL)	*ErrorInfos = m_ErrorInfos;
	}


protected:
	SQLHANDLE m_SQLHandle;
	const SQLSMALLINT m_SQLHandleType;

	SQLRETURN m_LastResult;
	ODBC_LASTERRINFO_LIST m_ErrorInfos;
};	// end of... class CODBCHandle

}	// end of... namesapce Handle

}	// end of... namesapce ODBCLib
