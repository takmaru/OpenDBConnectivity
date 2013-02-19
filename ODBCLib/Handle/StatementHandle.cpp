#include "StatementHandle.h"

#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

ODBCLib::CStatementHandle::CStatementHandle(SQLHDBC connectionHandle) : m_statementHandle(SQL_NULL_HANDLE) {
	// ハンドル作成
	::SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &m_statementHandle);
}
ODBCLib::CStatementHandle::~CStatementHandle() {
	FreeHandle();
}

SQLRETURN ODBCLib::CStatementHandle::Prepare(const SQLWCHAR* statement) {
	SQLWCHAR statementCopy[1024] = {0};
	wcscpy_s(statementCopy, statement);
	// ステートメントの準備
	return ::SQLPrepareW(m_statementHandle, statementCopy, SQL_NTS);
}

SQLRETURN ODBCLib::CStatementHandle::BindOutputParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd) {
	// 出力パラメータ バインド
	return ::SQLBindParameter(m_statementHandle, index, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd) {
	// パラメータ バインド
	return ::SQLBindParameter(m_statementHandle, index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, wchar_t* param, SQLSMALLINT paramSize, SQLINTEGER* lenOrInd) {
	// パラメータ バインド
	return ::SQLBindParameter(m_statementHandle, index, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, paramSize, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}
SQLRETURN ODBCLib::CStatementHandle::BindParameter(SQLUSMALLINT index, __int64* param, SQLINTEGER* lenOrInd) {
	// パラメータ バインド
	return ::SQLBindParameter(m_statementHandle, index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
		static_cast<SQLPOINTER>(param), 0, lenOrInd);
}

SQLRETURN ODBCLib::CStatementHandle::Execute() {
	// ステートメントの実行
	return ::SQLExecute(m_statementHandle);
}

SQLRETURN ODBCLib::CStatementHandle::MoreResults() {
	return ::SQLMoreResults(m_statementHandle);
}

SQLSMALLINT ODBCLib::CStatementHandle::GetResult_ColCount() {
	SQLSMALLINT colcnt = 0;
	::SQLNumResultCols(m_statementHandle, &colcnt);
	return colcnt;
}

SQLLEN ODBCLib::CStatementHandle::GetResult_ColAttr(SQLUSMALLINT col, SQLUSMALLINT fieldID) {
	SQLLEN attr = 0;
	::SQLColAttributeW(m_statementHandle, col, fieldID, NULL, 0, NULL, &attr);
	return attr;
}

std::wstring ODBCLib::CStatementHandle::GetResult_ColAttrString(SQLUSMALLINT col, SQLUSMALLINT fieldID) {
	std::wstring attrStr;

	SQLRETURN ret = SQL_SUCCESS;
	SQLSMALLINT bytes = 0;
	std::vector<unsigned char> attrStrBuffer;
	ret = ::SQLColAttributeW(m_statementHandle, col, fieldID, NULL, 0, &bytes, NULL);
	if(ret == SQL_SUCCESS) {
		if(bytes > 0) {
			// SQL_SUCCESSでデータ長が返ってきていれば、文字列取得
			bytes += sizeof(wchar_t);	// NULL文字分、追加
			attrStrBuffer.resize(bytes, L'\0');
			ret = ::SQLColAttributeW(m_statementHandle, col, fieldID, &(*attrStrBuffer.begin()), bytes, &bytes, NULL);
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
	SQLRETURN ret = ::SQLRowCount(m_statementHandle, &rowCount);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::GetRowCount() SQLRowCount error(" << ret << L")" << std::endl;
	}
	return rowCount;
}

bool ODBCLib::CStatementHandle::IsNoCount() {
	SQLLEN value = SQL_NC_OFF;
	SQLRETURN ret = ::SQLGetStmtAttrW(m_statementHandle, SQL_SOPT_SS_NOCOUNT_STATUS, &value, sizeof(value), NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::IsNoCount() SQLGetStmtAttrW error(" << ret << L")" << std::endl;
	}
	return (value == SQL_NC_ON);
}

void ODBCLib::CStatementHandle::SetFetchCount(SQLULEN fetchCount) {
	SQLRETURN ret = ::SQLSetStmtAttrW(m_statementHandle, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)fetchCount, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchCount() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetColWiseBind() {
	SQLRETURN ret = ::SQLSetStmtAttrW(m_statementHandle, SQL_ATTR_ROW_BIND_TYPE, SQL_BIND_BY_COLUMN, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchedCountPtr() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetFetchedCountPtr(SQLULEN* fetchedCount) {
/*{
std::wostringstream oss;
oss << std::setfill(L'0') << std::right << std::hex << std::setw(8) << (void*)fetchedCount;
std::wcout << L"CStatementHandle::SetFetchedCountPtr ptr=0x" << oss.str() << std::endl;
}*/
	SQLRETURN ret = ::SQLSetStmtAttrW(m_statementHandle, SQL_ATTR_ROWS_FETCHED_PTR, fetchedCount, 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetFetchedCountPtr() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

void ODBCLib::CStatementHandle::SetRowStatusArray(RowStatusArray& rowStatusArray) {
/*{
std::wostringstream oss;
oss << std::setfill(L'0') << std::right << std::hex << std::setw(8) << (void*)(&(*rowStatusArray.begin()));
std::wcout << L"CStatementHandle::SetRowStatusArray ptr=0x" << oss.str() << std::endl;
}*/
	SQLRETURN ret = ::SQLSetStmtAttrW(m_statementHandle, SQL_ATTR_ROW_STATUS_PTR, &(*rowStatusArray.begin()), 0);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CStatementHandle::SetRowStatusArray() SQLSetStmtAttrW error(" << ret << L")" << std::endl;
	}
}

SQLRETURN ODBCLib::CStatementHandle::BindCol(SQLUSMALLINT col, SQLSMALLINT type, SQLPOINTER valuePtr, SQLLEN elementSize, SQLINTEGER* len) {
/*{
std::wostringstream oss;
oss << std::setfill(L'0') << std::right << std::hex << std::setw(8) << valuePtr;
std::wostringstream oss2;
oss2 << std::setfill(L'0') << std::right << std::hex << std::setw(8) << (void*)len;

std::wcout << L"CStatementHandle::BindCol col=" << col << L" ptr=0x" << oss.str() << L" len=0x" << oss2.str() << std::endl;
}*/
	return ::SQLBindCol(m_statementHandle, col, SQL_C_BINARY, valuePtr, elementSize, len);
}

SQLRETURN ODBCLib::CStatementHandle::Fetch() {
	return ::SQLFetch(m_statementHandle);
}

bool ODBCLib::CStatementHandle::SetRowWiseBinding(int rowLen, int rowCount, SQLUSMALLINT* pStatusArray, SQLULEN* pProcCount) {

	// 行のサイズ セット
	::SQLSetStmtAttr(m_statementHandle, SQL_ATTR_PARAM_BIND_TYPE, reinterpret_cast<SQLPOINTER>(rowLen), 0);
	// 行の件数 セット
	::SQLSetStmtAttr(m_statementHandle, SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(rowCount), 0);

	// ステータスを受け取る配列 セット
	::SQLSetStmtAttr(m_statementHandle, SQL_ATTR_PARAM_STATUS_PTR, pStatusArray, 0);
	// 処理数を受け取る変数 セット
	::SQLSetStmtAttr(m_statementHandle, SQL_ATTR_PARAMS_PROCESSED_PTR, pProcCount, 0);

	return true;
}

void ODBCLib::CStatementHandle::FreeHandle() {
	if(m_statementHandle != SQL_NULL_HANDLE) {
		::SQLFreeHandle(SQL_HANDLE_STMT, m_statementHandle);
		m_statementHandle = SQL_NULL_HANDLE;
	}
}
