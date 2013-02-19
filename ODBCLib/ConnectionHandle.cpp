#include "stdafx.h"
#include "ConnectionHandle.h"

ODBCLib::CConnectionHandle::CConnectionHandle(SQLHENV environmentHandle) : CODBCHandle(SQL_HANDLE_DBC) {
	// ハンドル作成
	::SQLAllocHandle(SQL_HANDLE_DBC, environmentHandle, &m_handle);
}
ODBCLib::CConnectionHandle::~CConnectionHandle() {
}

SQLRETURN ODBCLib::CConnectionHandle::connect(const SQLWCHAR* connectionString) {
	SQLWCHAR connectionStringCopy[1024] = {0};
	wcscpy_s(connectionStringCopy, connectionString);
	// データベースへ接続
	return ::SQLDriverConnectW(static_cast<SQLHDBC>(m_handle), NULL, connectionStringCopy, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
}
SQLRETURN ODBCLib::CConnectionHandle::disconnect() {
	return ::SQLDisconnect(static_cast<SQLHDBC>(m_handle));
}

bool ODBCLib::CConnectionHandle::StartTransaction() {
	// トランザクション開始
	return SQL_SUCCEEDED(::SQLSetConnectAttrW(static_cast<SQLHDBC>(m_handle), SQL_ATTR_AUTOCOMMIT, static_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER));
}
bool ODBCLib::CConnectionHandle::Commit() {
	// コミット
	return SQL_SUCCEEDED(::SQLEndTran(SQL_HANDLE_DBC, m_handle, SQL_COMMIT));
}
bool ODBCLib::CConnectionHandle::Rollback() {
	// ロールバック
	return SQL_SUCCEEDED(::SQLEndTran(SQL_HANDLE_DBC, m_handle, SQL_ROLLBACK));
}
