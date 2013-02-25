#pragma once

namespace ODBCLib {

class CStatementHandle;
class CResultSet;
class CDiagInfo;

class CStatementResult {
public:
	CStatementResult(std::shared_ptr<CStatementHandle> statementHandle, SQLRETURN result);
	~CStatementResult();

public:
	bool isSucess() const {
		return (m_result == SQL_SUCCESS);
	};
	bool isInfo() const {
		return (m_result == SQL_SUCCESS_WITH_INFO);
	};
	bool isError() const {
		return (m_result == SQL_ERROR);
	};
	bool isErrorCode() const {
		return (this->isSucess() || this->isInfo() || this->isError());
	}
	SQLRETURN result() const {
		return m_result;
	};

private:
	std::weak_ptr<CStatementHandle> m_statementHandle;
	SQLRETURN m_result;
	std::unique_ptr<CResultSet> m_resultSet;
	std::unique_ptr<CDiagInfo> m_diagInfo;
};

}
