#pragma once

namespace ODBCLib {

class CEnvironmentHandle;
class CConnectionHandle;

class CODBCSession {
// constructor, destructor
public:
	CODBCSession();
	~CODBCSession();

// public method
public:
	bool startSession();
	bool endSession();

// setter, getter
public:
	void setODBCVersion(SQLSMALLINT version) {
		m_ODBCVersion = version;
	};
	SQLSMALLINT odbcVersion() const {
		return m_ODBCVersion;
	};

	void setDriverName(const wchar_t* driverName) {
		m_driverName = driverName;
	};
	void setServerName(const wchar_t* serverName) {
		m_serverName = serverName;
	};
	void setDatabaseName(const wchar_t* databaseName) {
		m_databaseName = databaseName;
	};
	void setTrutedConnection(bool isTruted) {
		m_isTrutedConnection = isTruted;
	};
	std::wstring connectionString() const;

private:
	std::unique_ptr<CEnvironmentHandle> m_environmentHandle;
	SQLSMALLINT m_ODBCVersion;

	std::unique_ptr<CConnectionHandle> m_connectionHandle;
	std::wstring m_driverName;
	std::wstring m_serverName;
	std::wstring m_databaseName;
	bool m_isTrutedConnection;
};

}
