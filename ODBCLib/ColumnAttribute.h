#pragma once

namespace ODBCLib {

struct _ColumnAttribute {

	SQLSMALLINT idx;

	std::wstring name;
	SQLSMALLINT type;
	std::wstring typeName;
	SQLINTEGER length;
	SQLINTEGER bytes;

} typedef ColumnAttribute;

}
