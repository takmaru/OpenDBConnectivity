#include "stdafx.h"
#include "StatementHandle.h"

#include "ConnectionHandle.h"
#include "DiagInfo.h"

ODBCLib::CStatementHandle::CStatementHandle(std::shared_ptr<CConnectionHandle> connectionHandle):
	CODBCHandle(SQL_HANDLE_STMT) {
	// �n���h���쐬
	SQLRETURN ret = ::SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle->handle(), &m_handle);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::CStatementHandle() SQLAllocHandle()=" << ret << std::endl <<
			ODBCLib::CDiagInfo(connectionHandle).description() << std::endl;
	}
}
ODBCLib::CStatementHandle::~CStatementHandle() {
}

// �X�e�[�g�����g�̏���
SQLRETURN ODBCLib::CStatementHandle::prepare(SQLWCHAR* statement) {
	return ::SQLPrepareW(static_cast<SQLHSTMT>(m_handle), statement, SQL_NTS);
}

// �X�e�[�g�����g�̎��s
SQLRETURN ODBCLib::CStatementHandle::execute() {
	return ::SQLExecute(static_cast<SQLHSTMT>(m_handle));
}
// ���̌��ʎ擾
SQLRETURN ODBCLib::CStatementHandle::MoreResults() {
	return ::SQLMoreResults(static_cast<SQLHSTMT>(m_handle));
}


SQLRETURN ODBCLib::CStatementHandle::BindOutputParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd) {
	// �o�̓p�����[�^ �o�C���h
	return ::SQLBindParameter(static_cast<SQLHSTMT>(m_handle), index, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd) {
	// �p�����[�^ �o�C���h
	return ::SQLBindParameter(static_cast<SQLHSTMT>(m_handle), index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, wchar_t* param, SQLSMALLINT paramSize, SQLINTEGER* lenOrInd) {
	// �p�����[�^ �o�C���h
	return ::SQLBindParameter(static_cast<SQLHSTMT>(m_handle), index, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, paramSize, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, __int64* param, SQLINTEGER* lenOrInd) {
	// �p�����[�^ �o�C���h
	return ::SQLBindParameter(static_cast<SQLHSTMT>(m_handle), index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}

SQLSMALLINT ODBCLib::CStatementHandle::GetResult_ColCount() {
	SQLSMALLINT colcnt = 0;
	::SQLNumResultCols(static_cast<SQLHSTMT>(m_handle), &colcnt);
	return colcnt;
}

SQLLEN ODBCLib::CStatementHandle::GetResult_ColAttr(SQLUSMALLINT col, SQLUSMALLINT fieldID) {
	SQLLEN attr = 0;
	::SQLColAttributeW(static_cast<SQLHSTMT>(m_handle), col, fieldID, NULL, 0, NULL, &attr);
	return attr;
}

std::wstring ODBCLib::CStatementHandle::GetResult_ColAttrString(SQLUSMALLINT col, SQLUSMALLINT fieldID) {
	std::wstring attrStr;

	SQLRETURN ret = SQL_SUCCESS;
	SQLSMALLINT bytes = 0;
	std::vector<unsigned char> attrStrBuffer;
	ret = ::SQLColAttributeW(static_cast<SQLHSTMT>(m_handle), col, fieldID, NULL, 0, &bytes, NULL);
	if(ret == SQL_SUCCESS) {
		if(bytes > 0) {
			// SQL_SUCCESS�Ńf�[�^�����Ԃ��Ă��Ă���΁A������擾
			bytes += sizeof(wchar_t);	// NULL�������A�ǉ�
			attrStrBuffer.resize(bytes, L'\0');
			ret = ::SQLColAttributeW(static_cast<SQLHSTMT>(m_handle), col, fieldID, &(*attrStrBuffer.begin()), bytes, &bytes, NULL);
			if(ret == SQL_SUCCESS) {
				attrStr = (wchar_t*)(&(*attrStrBuffer.begin()));
			} else {
				std::wcerr << L"CStatementHandle::GetResult_ColAttrString() SQLColAttributeW(ID:" << fieldID << ") error(" << ret << L")" << std::endl;
			}
		}
	} else {
		// getLength error
		std::wcerr << L"CStatementHandle::GetResult_ColAttrString() SQLColAttributeW(ID:" << fieldID << ") getLength error(" << ret << L")" << std::endl;
	}

	return attrStr;
}

SQLLEN ODBCLib::CStatementHandle::GetRowCount() {
	SQLLEN rowCount = 0;
	SQLRETURN ret = ::SQLRowCount(static_cast<SQLHSTMT>(m_handle), &rowCount);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::GetRowCount() SQLRowCount error(" << ret << L")" << std::endl;
	}
	return rowCount;
}

bool ODBCLib::CStatementHandle::IsNoCount() {
	SQLLEN value = SQL_NC_OFF;
	SQLRETURN ret = ::SQLGetStmtAttrW(static_cast<SQLHSTMT>(m_handle), SQL_SOPT_SS_NOCOUNT_STATUS, &value, sizeof(value), NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::IsNoCount() SQLGetStmtAttrW error(" << ret << L")" << std::endl;
	}
	return (value == SQL_NC_ON);
}

void ODBCLib::CStatementHandle::SetFetchCount(SQLULEN fetchCount) {
	SQLRETURN ret = ::SQLSetStmtAttrW(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)fetchCount, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchCount() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetColWiseBind() {
	SQLRETURN ret = ::SQLSetStmtAttrW(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_ROW_BIND_TYPE, SQL_BIND_BY_COLUMN, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchedCountPtr() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetFetchedCountPtr(SQLULEN* fetchedCount) {
	SQLRETURN ret = ::SQLSetStmtAttrW(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_ROWS_FETCHED_PTR, fetchedCount, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchedCountPtr() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetRowStatusArray(RowStatusArray& rowStatusArray) {
	SQLRETURN ret = ::SQLSetStmtAttrW(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_ROW_STATUS_PTR, &(*rowStatusArray.begin()), 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetRowStatusArray() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

SQLRETURN ODBCLib::CStatementHandle::BindCol(SQLUSMALLINT col, SQLSMALLINT type, SQLPOINTER valuePtr, SQLLEN elementSize, SQLINTEGER* len) {
	return ::SQLBindCol(static_cast<SQLHSTMT>(m_handle), col, SQL_C_BINARY, valuePtr, elementSize, len);
}

SQLRETURN ODBCLib::CStatementHandle::Fetch() {
	return ::SQLFetch(static_cast<SQLHSTMT>(m_handle));
}

bool ODBCLib::CStatementHandle::SetRowWiseBinding(int rowLen, int rowCount, SQLUSMALLINT* pStatusArray, SQLULEN* pProcCount) {

	// �s�̃T�C�Y �Z�b�g
	::SQLSetStmtAttr(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_PARAM_BIND_TYPE, reinterpret_cast<SQLPOINTER>(rowLen), 0);
	// �s�̌��� �Z�b�g
	::SQLSetStmtAttr(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(rowCount), 0);

	// �X�e�[�^�X���󂯎��z�� �Z�b�g
	::SQLSetStmtAttr(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_PARAM_STATUS_PTR, pStatusArray, 0);
	// ���������󂯎��ϐ� �Z�b�g
	::SQLSetStmtAttr(static_cast<SQLHSTMT>(m_handle), SQL_ATTR_PARAMS_PROCESSED_PTR, pProcCount, 0);

	return true;
}
