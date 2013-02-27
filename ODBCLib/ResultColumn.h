#pragma once

namespace ODBCLib {

class CStatementHandle;

class CResultColumn {
public:
	CResultColumn(SQLSMALLINT idx, std::shared_ptr<CStatementHandle> statementHandle);
	~CResultColumn();

public:
	SQLSMALLINT idx() const {
		return m_idx;
	};

	std::wstring name() const {
		return m_name;
	}
	SQLSMALLINT type() const {
		return m_type;
	};
	std::wstring typeName() const {
		return m_typeName;
	};
	SQLINTEGER length() const {
		return m_length;
	};
	SQLINTEGER bytes() const {
		return m_bytes;
	};

private:
	SQLSMALLINT m_idx;

	std::wstring m_name;
	SQLSMALLINT m_type;
	std::wstring m_typeName;
	SQLINTEGER m_length;
	SQLINTEGER m_bytes;
};

}
