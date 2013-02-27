#include "stdafx.h"
#include "ResultColumn.h"

#include "StatementHandle.h"

ODBCLib::CResultColumn::CResultColumn(SQLSMALLINT idx, std::shared_ptr<CStatementHandle> statementHandle):
	m_idx(), m_name(), m_type(), m_typeName(), m_length(0), m_bytes(0)
{

	m_columnAttr.idx = idx;
}

ODBCLib::CResultColumn::~CResultColumn() {
}
