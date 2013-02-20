#include "stdafx.h"
#include "DiagInfo.h"

#include "ODBCHandle.h"
#include "DiagRecord.h"
#include "ODBCLibUtil.h"

ODBCLib::CDiagInfo::CDiagInfo(CODBCHandle& handle):
	m_type(handle.type()), m_handle(handle.handle()), m_records() {

	// レコード数取得
	SQLSMALLINT recordCount = 0;
	ODBCLib::GetDiagFieldInfo_Value(handle.type(), handle.handle(), 0, SQL_DIAG_NUMBER, &recordCount, SQL_IS_SMALLINT);
	// 詳細情報取得ループ
	m_records.reserve(recordCount);
	SQLSMALLINT idx = 0;
	while(idx <= recordCount) {
		m_records.push_back(DiagRecords::value_type(new CDiagRecord(handle, idx)));
	}
/*
	SQLSMALLINT record = 1;
	SQLSTATE sqlState = {0};
	SQLINTEGER nativeError = 0;
	SQLSMALLINT errorMsgLen = 0;

	std::vector<SQLWCHAR> message(1024, L'\0');

	SQLINTEGER count = GetRecordCount();
	while(record <= count) {
		CDiagRecord
		SQLSMALLINT msgLen = message.size();
		 
		SQLRETURN ret = ::SQLGetDiagRecW(m_type, m_handle, record, sqlState, &nativeError, &(*message.begin()), msgLen, &errorMsgLen);
		if(SQL_SUCCEEDED(ret)) {

			if(	(ret == SQL_SUCCESS) ||
				(	(ret == SQL_SUCCESS_WITH_INFO) &&
					((errorMsgLen > 0) && (errorMsgLen <= msgLen))	)	) {
				// 成功か、追加情報ありだけどメッセージ長が足りている場合、レコード情報へ追加
				m_errors.push_back(Record(sqlState, nativeError, &(*message.begin()), GetDiagFieldInfo(record)));
				record++;

			} else if (errorMsgLen > 0) {
				// メッセージ長が足りない場合はメッセージバッファをリサイズしてリトライ
				message.resize(errorMsgLen + 1, L'\0');

			} else {
				// その他のエラー
				std::wcerr << L"CDiagInfo::CDiagInfo() SQLGetDiagRecW" <<
					L" 成功したが取得メッセージ長が異常" <<
					L" record=" << record <<
					L" len=" << errorMsgLen <<
					std::endl;
				break;
			}
		} else {
			// エラー(SQL_ERROR, SQL_INVALID_HANDLE)か、データなし(SQL_NO_DATA)
				std::wcerr << L"CDiagInfo::CDiagInfo() SQLGetDiagRecW" <<
					L" error(" << ret << L")" <<
					L" record=" << record <<
					std::endl;
			break;
		}
	}
*/
}
ODBCLib::CDiagInfo::~CDiagInfo() {
}

std::wstring ODBCLib::CDiagInfo::description() const {
	std::wostringstream oss;
	DiagRecords::const_iterator it;
	for(it = m_records.begin(); it != m_records.end(); ++it) {
		if(it != m_records.begin()) {
			oss << std::endl;
		}
		(*it)->description();
	}
	return oss.str();
/*
	std::wostringstream oss;
	for(unsigned int i = 0; i < GetCount(); ++i) {

		if(i > 0) {
			oss << std::endl;
		}

		const ODBCLib::CDiagInfo::FieldInfo& fieldInfo = GetFieldInfo(i);
		oss << L"Info[" << i << L"]" << std::endl;
		// Row --->>
		oss << L"   Row(" <<
			L"state=" << GetState(i) <<
			L" code=" << GetErrorCode(i) <<
			L" msg(" << GetErrorMessage(i) << L")" <<
			L")" << std::endl;
		// Row <<---
		// Field --->>
		oss << L"   Field(" <<
				L"rownum=" << fieldInfo.rowNumber <<
				L" line=" << fieldInfo.line <<
				L" state=" << fieldInfo.msgState <<
				L" severity=" << fieldInfo.severity;
		if(fieldInfo.procName.size() > 0) {
			oss << L" procName[" << fieldInfo.procName << L"]";
		}
		if(fieldInfo.serverName.size() > 0) {
			oss << L" serverName[" << fieldInfo.serverName << L"]";
		}
		oss << L")";
		// Field <<---
	}
	return oss.str();
*/
}
/*
SQLINTEGER ODBCLib::CDiagInfo::GetRecordCount() const {
	ODBCLib::
	SQLINTEGER count = 0;
	SQLSMALLINT len = 0;
	SQLRETURN ret = ::SQLGetDiagFieldW(m_type, m_handle, 0, SQL_DIAG_NUMBER, &count, sizeof(count), &len);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CDiagInfo::GetRecordCount() SQLGetDiagFieldW error(" << ret << L")" << std::endl;
	}
	return count;
}

ODBCLib::CDiagInfo::FieldInfo ODBCLib::CDiagInfo::GetDiagFieldInfo(SQLSMALLINT record) const {
	SQLRETURN ret = SQL_SUCCESS;
	ODBCLib::CDiagInfo::FieldInfo field;

	// rowNumber
	ret = ::SQLGetDiagFieldW(m_type, m_handle, record, SQL_DIAG_ROW_NUMBER, &field.rowNumber, SQL_IS_INTEGER, NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CDiagInfo::GetFieldInfo() SQL_DIAG_ROW_NUMBER SQLGetDiagFieldW error(" << ret << L")" << std::endl;
	}
	// line
	ret = ::SQLGetDiagFieldW(m_type, m_handle, record, SQL_DIAG_SS_LINE, &field.line, SQL_IS_USMALLINT, NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CDiagInfo::GetFieldInfo() SQL_DIAG_SS_LINE SQLGetDiagFieldW error(" << ret << L")" << std::endl;
	}
	// msgState
	ret = ::SQLGetDiagFieldW(m_type, m_handle, record, SQL_DIAG_SS_MSGSTATE, &field.msgState, SQL_IS_INTEGER, NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CDiagInfo::GetFieldInfo() SQL_DIAG_SS_MSGSTATE SQLGetDiagFieldW error(" << ret << L")" << std::endl;
	}
	// severity
	ret = ::SQLGetDiagFieldW(m_type, m_handle, record, SQL_DIAG_SS_SEVERITY, &field.severity, SQL_IS_INTEGER, NULL);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CDiagInfo::GetFieldInfo() SQL_DIAG_SS_SEVERITY SQLGetDiagFieldW error(" << ret << L")" << std::endl;
	}
	// procName
	field.procName = GetDiagFieldInfo_String(record, SQL_DIAG_SS_PROCNAME);
	// serverName
	field.serverName = GetDiagFieldInfo_String(record, SQL_DIAG_SS_SRVNAME);

	return field;
}

std::wstring ODBCLib::CDiagInfo::GetDiagFieldInfo_String(SQLSMALLINT record, SQLSMALLINT diagId) const {

	std::wstring fieldStr;

	SQLRETURN ret = SQL_SUCCESS;
	SQLSMALLINT bytes = 0;
	std::vector<unsigned char> fieldStrBuffer;
	ret = ::SQLGetDiagFieldW(m_type, m_handle, record, diagId, NULL, 0, &bytes);
	if(ret == SQL_SUCCESS) {
		if(bytes > 0) {
			// SQL_SUCCESSでデータ長が返ってきていれば、文字列取得
			bytes += sizeof(wchar_t);	// NULL文字分、追加
			fieldStrBuffer.resize(bytes, L'\0');
			ret = ::SQLGetDiagFieldW(m_type, m_handle, record, diagId, &(*fieldStrBuffer.begin()), bytes, &bytes);
			if(ret == SQL_SUCCESS) {
				fieldStr = (wchar_t*)(&(*fieldStrBuffer.begin()));
			} else {
				std::wcerr << L"CDiagInfo::GetDiagFieldInfo_String() SQLGetDiagFieldW(ID:" << diagId << ") error(" << ret << L")" << std::endl;
			}
		}
	} else {
		// getLength error
		std::wcerr << L"CDiagInfo::GetDiagFieldInfo_String() SQLGetDiagFieldW(" << diagId << ") getLength error(" << ret << L")" << std::endl;
	}

	return fieldStr;
}
*/