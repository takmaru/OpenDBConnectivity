#include "ODBCHandle.h"

#include <sqlucode.h>

using namespace MyLib::ODBC;

CODBCHandle::CODBCHandle(SQLSMALLINT HandleType):
	m_SQLHandle(SQL_NULL_HANDLE), m_SQLHandleType(HandleType),
	m_LastResult(SQL_SUCCESS), m_ErrorInfos(){}


CODBCHandle::~CODBCHandle()
{Destroy();}

bool CODBCHandle::Create(SQLSMALLINT ParentHandleType, SQLHANDLE ParentHandle)
{
	m_LastResult = ::SQLAllocHandle(m_SQLHandleType, ParentHandle, &m_SQLHandle);
	bool bResult = SQL_SUCCEEDED(m_LastResult);
	if(	!bResult &&
		(ParentHandle != SQL_NULL_HANDLE)	)	SetLastErrorInfos(ParentHandleType, ParentHandle);
	return bResult;
}

void CODBCHandle::Destroy()
{if(IsCreate())	::SQLFreeHandle(m_SQLHandleType, m_SQLHandle);}

void CODBCHandle::SetLastErrorInfos(SQLSMALLINT ParentHandleType, SQLHANDLE ParentHandle)
{
	m_ErrorInfos.clear();
	SQLSMALLINT RecNumber = 1;
	SQLWCHAR State[6] = {L'\0'};
	SQLINTEGER ErrorCode = 0;
	SQLWCHAR ErrorMsg[SQL_MAX_MESSAGE_LENGTH] = {L'\0'};
	SQLSMALLINT Size = 0;
	while(SQL_SUCCEEDED(::SQLGetDiagRecW(ParentHandleType, ParentHandle, RecNumber, State, &ErrorCode, ErrorMsg, _countof(ErrorMsg), &Size)))
	{
		m_ErrorInfos.push_back(CODBCErrorInfo(State, ErrorCode, ErrorMsg));
		RecNumber++;
	}
}

bool CODBCHandle::SetResult(SQLRETURN Result)
{
	m_LastResult = Result;
	bool bResult = SQL_SUCCEEDED(m_LastResult);
	if(!bResult)	SetLastErrorInfos(m_SQLHandleType, m_SQLHandle);
	return bResult;
}
