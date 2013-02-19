#include "stdafx.h"
#include "EnvironmentHandle.h"


ODBCLib::CEnvironmentHandle::CEnvironmentHandle() : m_environmentHandle(SQL_NULL_HANDLE) {
	// ハンドル作成
	::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environmentHandle);
}
ODBCLib::CEnvironmentHandle::~CEnvironmentHandle() {
	FreeHandle();
}

// 属性設定
SQLRETURN ODBCLib::CEnvironmentHandle::setVersion(SQLSMALLINT version) {
	return ::SQLSetEnvAttr(m_environmentHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(version), 0);
}


void ODBCLib::CEnvironmentHandle::FreeHandle() {
	if(m_environmentHandle != SQL_NULL_HANDLE) {
		::SQLFreeHandle(SQL_HANDLE_ENV, m_environmentHandle);
		m_environmentHandle = SQL_NULL_HANDLE;
	}
}
