#pragma once

namespace ODBCLib {

class CODBCHandle;

class CDiagRecord {
public:
	CDiagRecord(CODBCHandle& handle, SQLSMALLINT idx);
//	CDiagRecord(const SQLSTATE state, int err, const wchar_t* msg, const FieldInfo& field):
//		m_state(state), m_err(err), m_msg(msg), m_field(field){};
	~CDiagRecord();

public:
	SQLSMALLINT idx() const {
		return m_idx;
	};
	SSHORT line() const {
		return m_line;
	};
	SQLINTEGER severity() const {
		return m_severity;
	};
	SQLINTEGER state() const {
		return m_state;
	};
	std::wstring sqlState() const {
		return std::wstring(m_sqlstate);
	};
	SQLINTEGER code() const {
		return m_code;
	};
	std::wstring message() const {
		return m_message;
	};

public:
	std::wstring description() const;

private:
	SQLSMALLINT m_idx;
	SSHORT m_line;
	SQLINTEGER m_severity;
	SQLINTEGER m_state;
	SQLSTATE m_sqlstate;
	SQLINTEGER m_code;
	std::wstring m_message;
};

}
