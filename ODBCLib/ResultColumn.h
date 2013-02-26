#pragma once

#include "ColumnAttribute.h"

namespace ODBCLib {

class CStatementHandle;

class CResultColumn {
public:
	CResultColumn(SQLSMALLINT idx, std::shared_ptr<CStatementHandle> statementHandle);
	~CResultColumn();

private:
	ColumnAttribute m_columnAttr;
};

}
