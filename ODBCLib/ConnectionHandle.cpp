#include "stdafx.h"
#include "ConnectionHandle.h"

ODBCLib::CConnectionHandle::CConnectionHandle(SQLHENV environmentHandle) : m_connectionHandle(SQL_NULL_HANDLE) {
	// �n���h���쐬
	::SQLAllocHandle(SQL_HANDLE_DBC, environmentHandle, &m_connectionHandle);
}
ODBCLib::CConnectionHandle::~CConnectionHandle() {
	FreeHandle();
}

SQLRETURN ODBCLib::CConnectionHandle::connect(const SQLWCHAR* connectionString) {
	SQLWCHAR connectionStringCopy[1024] = {0};
	wcscpy_s(connectionStringCopy, connectionString);
	// �f�[�^�x�[�X�֐ڑ�
	return ::SQLDriverConnectW(m_connectionHandle, NULL, connectionStringCopy, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
}
SQLRETURN ODBCLib::CConnectionHandle::disconnect() {
	return ::SQLDisconnect(m_connectionHandle);
}

bool ODBCLib::CConnectionHandle::StartTransaction() {
	// �g�����U�N�V�����J�n
	return SQL_SUCCEEDED(::SQLSetConnectAttrW(m_connectionHandle, SQL_ATTR_AUTOCOMMIT, static_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER));
}
bool ODBCLib::CConnectionHandle::Commit() {
	// �R�~�b�g
	return SQL_SUCCEEDED(::SQLEndTran(SQL_HANDLE_DBC, m_connectionHandle, SQL_COMMIT));
}
bool ODBCLib::CConnectionHandle::Rollback() {
	// ���[���o�b�N
	return SQL_SUCCEEDED(::SQLEndTran(SQL_HANDLE_DBC, m_connectionHandle, SQL_ROLLBACK));
}

void ODBCLib::CConnectionHandle::FreeHandle() {
	if(m_connectionHandle != SQL_NULL_HANDLE) {
		::SQLFreeHandle(SQL_HANDLE_DBC, m_connectionHandle);
		m_connectionHandle = SQL_NULL_HANDLE;
	}
}
