#include "stdafx.h"
#include "DiagRecord.h"

#include "ODBCHandle.h"
#include "ODBCLibUtil.h"

ODBCLib::CDiagRecord::CDiagRecord(CODBCHandle& handle, SQLSMALLINT idx):
	m_idx(idx), m_line(0), m_severity(0), m_state(0), m_code(0), m_message() {

	// line
	ODBCLib::GetDiagFieldInfo_Value(handle.type(), handle.handle(), m_idx, SQL_DIAG_SS_LINE, &m_line, SQL_IS_USMALLINT);
	// severity
	ODBCLib::GetDiagFieldInfo_Value(handle.type(), handle.handle(), m_idx, SQL_DIAG_SS_SEVERITY, &m_severity, SQL_IS_INTEGER);
	// state
	ODBCLib::GetDiagFieldInfo_Value(handle.type(), handle.handle(), m_idx, SQL_DIAG_SS_MSGSTATE, &m_state, SQL_IS_INTEGER);
	// sqlstate
	ODBCLib::GetDiagFieldInfo_String(handle.type(), handle.handle(), m_idx, SQL_DIAG_SQLSTATE, m_sqlstate, sizeof(m_sqlstate));
	// code
	ODBCLib::GetDiagFieldInfo_Value(handle.type(), handle.handle(), m_idx, SQL_DIAG_NATIVE, &m_code, SQL_IS_INTEGER);
	// message
	ODBCLib::GetDiagFieldInfo_String(handle.type(), handle.handle(), m_idx, SQL_DIAG_MESSAGE_TEXT, m_message);
}

ODBCLib::CDiagRecord::~CDiagRecord() {
}

std::wstring ODBCLib::CDiagRecord::description() const {
	std::wostringstream oss;
	oss <<	m_idx << L" : " <<
			L"line=" << m_line << L", " <<
			L"severity=" << m_severity << L", " <<
			L"state=" << m_state << L", " <<
			L"[" << m_sqlstate << L"]" <<
			L"[" << m_code << L"]" <<
			m_message;
	return oss.str();
}