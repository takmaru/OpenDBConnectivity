#include "stdafx.h"
#include "ODBCSession.h"

#include "EnvironmentHandle.h"
#include "ConnectionHandle.h"
#include "DiagInfo.h"

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
	SQLRETURN ret = SQL_SUCCESS;

	ret = m_environmentHandle->setVersion(m_ODBCVersion);
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CODBCSession::startSession() CEnvironmentHandle::setVersion()=" << ret << std::endl <<
			CDiagInfo(SQL_HANDLE_ENV, (SQLHENV)m_environmentHandle);
		return false;
		throw CODBCLibException("CODBCSession::startSession() CEnvironmentHandle::setVersion");
	}
		if(m_ConnectionHandle->connect(connectionString().c_str()) == SQL_SUCCESS) {
			ret = true;
		}
	}
	return result;
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
