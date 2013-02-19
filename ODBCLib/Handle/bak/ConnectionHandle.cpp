#include "ConnectionHandle.h"

#include <sqlext.h>

#include <sstream>

using namespace MyLib::ODBC;

CConnectionHandle::CConnectionHandle(SQLHENV EnvironmentHandle):
	CODBCHandle(SQL_HANDLE_DBC), m_EnvironmentHandle(EnvironmentHandle){}
CConnectionHandle::~CConnectionHandle(){}

bool CConnectionHandle::SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_ENV, m_EnvironmentHandle)	)	return false;

	return SetResult(::SQLSetConnectAttrW(m_SQLHandle, Attribute, ValuePtr, StringLength));
}

bool CConnectionHandle::Connect(const std::wstring& ServerName, const std::wstring& DBName)	// throw(std::bad_alloc)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_ENV, m_EnvironmentHandle)	)	return false;

	std::wostringstream ConnectionStringStream(L"DRIVER={SQL Server Native Client 10.0};Trusted_Connection=yes;SERVER=");
	if(ServerName.empty())	ConnectionStringStream << L"(local);";
	else					ConnectionStringStream << ServerName << L';';
	if(!DBName.empty())	ConnectionStringStream << L"Database=" << DBName;

	return _Connect(ConnectionStringStream.str());
}
bool CConnectionHandle::Connect(const std::wstring& DBName)	// throw(std::bad_alloc)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_ENV, m_EnvironmentHandle)	)	return false;

	std::wostringstream ConnectionStringStream(L"DRIVER={SQL Server Native Client 10.0};Trusted_Connection=yes;SERVER=(local);");
	if(!DBName.empty())	ConnectionStringStream << L"Database=" << DBName;

	return _Connect(ConnectionStringStream.str());
}
bool CConnectionHandle::_Connect(const std::wstring& ConnectionString)	// throw(std::bad_alloc)
{
	int ConnectionStringLen = ConnectionString.size() + 1;
	SQLWCHAR* _ConnectionString = new SQLWCHAR[ConnectionStringLen];
	wcscpy_s(_ConnectionString, ConnectionStringLen, ConnectionString.c_str());

	bool bResult = SetResult(::SQLDriverConnectW(m_SQLHandle, NULL, _ConnectionString, SQL_NTS,
								NULL, 0, NULL, SQL_DRIVER_NOPROMPT));

	delete [] _ConnectionString;
	return bResult;
}
void CConnectionHandle::DisConnect()
{::SQLDisconnect(m_SQLHandle);}

bool CConnectionHandle::Rollback()
{return SetResult(::SQLEndTran(m_SQLHandleType, m_SQLHandle, SQL_ROLLBACK));}
bool CConnectionHandle::Commit()
{return SetResult(::SQLEndTran(m_SQLHandleType, m_SQLHandle, SQL_COMMIT));}
