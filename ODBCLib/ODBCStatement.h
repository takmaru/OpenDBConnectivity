#pragma once

namespace ODBCLib {

class CConnectionHandle;
class CStatementHandle;

class CODBCStatement {
public:
	explicit CODBCStatement(std::shared_ptr<CConnectionHandle> connectionHandle);
	~CODBCStatement();

public:
	bool prepare(const wchar_t* statement);

private:
	std::shared_ptr<CStatementHandle> m_statementHandle;
};

}
