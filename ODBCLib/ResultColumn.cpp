#include "stdafx.h"
#include "ResultColumn.h"

#include "StatementHandle.h"

ODBCLib::CResultColumn::CResultColumn(SQLSMALLINT idx, std::shared_ptr<CStatementHandle> statementHandle):
	m_columnAttr() {

	m_columnAttr.idx = idx;
}

ODBCLib::CResultColumn::~CResultColumn() {
}
