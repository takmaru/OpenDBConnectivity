#pragma once

#include <string>
#include <vector>

#include "Include/ODBCLibDef.h"

namespace ODBCLib {

class CStatementHandle {
// constructor & destructor
public:
	explicit CStatementHandle(SQLHDBC connectionHandle);
	~CStatementHandle();

// operator
public:
	operator SQLHSTMT() const {
		return m_statementHandle;
	};

// public method
public:
	SQLRETURN Prepare(const SQLWCHAR* statement);

	SQLRETURN BindOutputParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd);
	SQLRETURN BindParameter(SQLUSMALLINT index, int* param, SQLINTEGER* lenOrInd);
	SQLRETURN BindParameter(SQLUSMALLINT index, wchar_t* param, SQLSMALLINT paramSize, SQLINTEGER* lenOrInd);
	SQLRETURN BindParameter(SQLUSMALLINT index, __int64* param, SQLINTEGER* lenOrInd);

	SQLRETURN Execute();
	SQLRETURN MoreResults();

	SQLSMALLINT GetResult_ColCount();
	SQLLEN GetResult_ColAttr(SQLUSMALLINT col, SQLUSMALLINT fieldID);
	std::wstring GetResult_ColAttrString(SQLUSMALLINT col, SQLUSMALLINT fieldID);

	SQLLEN GetRowCount();
	bool IsNoCount();

	void SetFetchCount(SQLULEN fetchCount);
	void SetColWiseBind();
	void SetFetchedCountPtr(SQLULEN* fetchedCount);
	void SetRowStatusArray(RowStatusArray& rowStatusArray);
	SQLRETURN BindCol(SQLUSMALLINT col, SQLSMALLINT type, SQLPOINTER valuePtr, SQLLEN elementSize, SQLINTEGER* len);
	SQLRETURN Fetch();

	bool SetRowWiseBinding(int rowLen, int rowCount, SQLUSMALLINT* pStatusArray, SQLULEN* pProcCount);

// private method
private:
	void FreeHandle();

// private member
private:
	SQLHSTMT m_statementHandle;
};	// end of... class CStatementHandle

}	// end of... namespace ODBCLib
