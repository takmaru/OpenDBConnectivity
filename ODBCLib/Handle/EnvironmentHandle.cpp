#include "EnvironmentHandle.h"

#include <sql.h>
#include <sqlext.h>

using namespace ODBCLib;

CEnvironmentHandle::CEnvironmentHandle() : m_environmentHandle(SQL_NULL_HANDLE) {
	// ハンドル作成
	if(SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environmentHandle))) {
		// 属性設定
		if(SQL_SUCCEEDED(::SQLSetEnvAttr(m_environmentHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))) {
			// 成功時は何もしない
		} else {
			// エラー時はハンドル解放
			FreeHandle();
		}
	}
}
CEnvironmentHandle::~CEnvironmentHandle() {
	FreeHandle();
}

void CEnvironmentHandle::FreeHandle() {
	if(m_environmentHandle != SQL_NULL_HANDLE) {
		::SQLFreeHandle(SQL_HANDLE_ENV, m_environmentHandle);
		m_environmentHandle = SQL_NULL_HANDLE;
	}
}
