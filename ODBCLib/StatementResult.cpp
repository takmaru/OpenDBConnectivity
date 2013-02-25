#include "stdafx.h"
#include "StatementResult.h"

#include "StatementHandle.h"
#include "ResultSet.h"
#include "DiagInfo.h"

ODBCLib::CStatementResult::CStatementResult(std::shared_ptr<CStatementHandle> statementHandle, SQLRETURN result):
	m_statementHandle(statementHandle), m_result(result), m_resultSet(), m_diagInfo() {

	if(isSucess()) {
		m_resultSet.reset(new ODBCLib::CResultSet(statementHandle));
	} else if(isInfo() || isError()) {
		m_diagInfo.reset(new ODBCLib::CDiagInfo(statementHandle));
	}
}

ODBCLib::CStatementResult::~CStatementResult() {
}
