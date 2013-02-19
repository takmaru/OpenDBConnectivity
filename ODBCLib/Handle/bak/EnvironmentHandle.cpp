#include "EnvironmentHandle.h"

using namespace ODBC::Handle;

CEnvironmentHandle::CEnvironmentHandle():
	CODBCHandle(SQL_HANDLE_ENV){}
CEnvironmentHandle::~CEnvironmentHandle(){}

bool CEnvironmentHandle::SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength)
{
	if(	!IsCreate() &&
		!Create(0, SQL_NULL_HANDLE)	)	return false;

	return SetResult(::SQLSetEnvAttr(m_SQLHandle, Attribute, ValuePtr, StringLength));
}