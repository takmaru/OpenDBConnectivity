#include <StatementHandle.h>

#include <sqlucode.h>

#include <string>

using namespace MyLib::ODBC;

CStatementHandle::CStatementHandle(SQLHDBC ConnectionHandle):
	CODBCHandle(SQL_HANDLE_STMT), m_ConnectionHandle(ConnectionHandle){}
CStatementHandle::~CStatementHandle(){}

bool CStatementHandle::SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_DBC, m_ConnectionHandle)	)	return false;

	return SetResult(::SQLSetStmtAttrW(m_SQLHandle, Attribute, ValuePtr, StringLength));
}

bool CStatementHandle::Prepare(const std::wstring& sql)// throw(std::bad_alloc)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_DBC, m_ConnectionHandle)	)	return false;

	int StatementStringLen = sql.size() + 1;
	SQLWCHAR* StatementString = new SQLWCHAR[StatementStringLen];
	wcscpy_s(StatementString, StatementStringLen, sql.c_str());

	bool bResult = SetResult(::SQLPrepareW(m_SQLHandle, StatementString, SQL_NTS));

	delete [] StatementString;
	return bResult;
}

bool CStatementHandle::BindParam(BINDPARAM_LST& BindParams)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_DBC, m_ConnectionHandle)	)	return false;

	bool bResult = false;
	SQLUSMALLINT cnt = 0;
	BINDPARAM_LST::iterator it;
	for(it = BindParams.begin(); it != BindParams.end(); it++)
	{
		cnt++;
		bResult = SetResult(::SQLBindParameter(m_SQLHandle, cnt, it->IOType, it->ValType_C, it->ValType_SQL, it->ColumnSize,
												0, it->ValPtr, 0, it->ValLen));
		if(!bResult)	break;
	}
	return bResult;
}

bool CStatementHandle::Execute()
{
	::SQLCloseCursor(m_SQLHandle);
	return SetResult(::SQLExecute(m_SQLHandle));
}

bool CStatementHandle::GetExecRowCount(SQLLEN& cnt)
{return SetResult(::SQLRowCount(m_SQLHandle, &cnt));}

bool CStatementHandle::BindRes(BINDRES_LST& BindRess)
{
	if(	!IsCreate() &&
		!Create(SQL_HANDLE_DBC, m_ConnectionHandle)	)	return false;

	bool bResult = false;
	SQLUSMALLINT cnt = 0;
	BINDRES_LST::iterator it;
	for(it = BindRess.begin(); it != BindRess.end(); it++)
	{
		cnt++;
		bResult = SetResult(::SQLBindCol(m_SQLHandle, cnt, it->ValType_C, it->ValPtr, it->ValPtrSize, it->ValLen));
		if(!bResult)	break;
	}
	return bResult;
}

bool CStatementHandle::Fetch()
{return SetResult(::SQLFetch(m_SQLHandle));}
