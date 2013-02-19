#include "stdafx.h"
#include "ODBCSession.h"

#include "EnvironmentHandle.h"
#include "ConnectionHandle.h"

namespace {
	static const std::wstring defaultDriverName = L"{SQL Server Native Client 10.0}";
};

ODBCLib::CODBCSession::CODBCSession():
	m_environmentHandle(new CEnvironmentHandle()),
	m_ODBCVersion(SQL_OV_ODBC3),
	m_ConnectionHandle(new CConnectionHandle((SQLHENV)*m_environmentHandle)),
	m_driverName(defaultDriverName), m_serverName(), m_databaseName(), m_isTrutedConnection(true) {
}

ODBCLib::CODBCSession::~CODBCSession() {
}

bool ODBCLib::CODBCSession::startSession() {
	bool ret = false;
	if(m_environmentHandle->setVersion(m_ODBCVersion) == SQL_SUCCESS) {
		if(m_ConnectionHandle->connect(connectionString().c_str()) == SQL_SUCCESS) {
			ret = true;
		}
	}
	return ret;
}
bool ODBCLib::CODBCSession::endSession() {
	return (m_ConnectionHandle->disconnect() == SQL_SUCCESS);
}

std::wstring ODBCLib::CODBCSession::connectionString() const {
	std::wstringstream oss;

	oss << L"DRIVER={" << m_driverName << "};";
	oss << L"SERVER=" << m_serverName << L";";
	if(m_databaseName.size() > 0) {
		oss << L"Database=" << m_databaseName << L";";
	}
	oss << L"Trusted_Connection=";
	if(m_isTrutedConnection) {
		oss << L"yes";
	} else {
		oss << L"no";
	}

	return oss.str();
}
