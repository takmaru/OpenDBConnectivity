#include "stdafx.h"
#include "ODBCStatement.h"

#include "StatementHandle.h"
#include "DiagInfo.h"

ODBCLib::CODBCStatement::CODBCStatement(std::shared_ptr<CConnectionHandle> connectionHandle):
	m_statementHandle(new CStatementHandle(connectionHandle)) {
}

ODBCLib::CODBCStatement::~CODBCStatement() {
}

bool ODBCLib::CODBCStatement::prepare(const wchar_t* statement) {
	bool result = false;
	std::vector<wchar_t> statementCopy(wcslen(statement), L'\0');
	SQLRETURN ret = m_statementHandle->prepare(&(*statementCopy.begin()));
	if(ret == SQL_SUCCESS) {
		result = true;
	} else {
		std::wcerr << L"CODBCSession::startSession() CConnectionHandle::connect()=" << ret << std::endl <<
			ODBCLib::CDiagInfo(m_statementHandle).description() << std::endl;
	}
	return  result;
}