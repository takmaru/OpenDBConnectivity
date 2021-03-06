#include "stdafx.h"
#include "EnvironmentHandle.h"


ODBCLib::CEnvironmentHandle::CEnvironmentHandle() : CODBCHandle(SQL_HANDLE_ENV) {
	// ハンドル作成
	::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_handle);
}
ODBCLib::CEnvironmentHandle::~CEnvironmentHandle() {
}

// 属性設定
SQLRETURN ODBCLib::CEnvironmentHandle::setVersion(SQLSMALLINT version) {
	return ::SQLSetEnvAttr(static_cast<SQLHENV>(m_handle), SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(version), 0);
}
