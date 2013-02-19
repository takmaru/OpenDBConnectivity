#pragma once

#include "../ODBC/ODBCHandle.h"

#include <list>

namespace MyLib
{
	namespace ODBC
	{
		class CStatementHandle : public CODBCHandle
		{
		public:
			struct _BindParamInfo
			{
				SQLSMALLINT	IOType;			// SQL_PARAM_INPUT || SQL_PARAM_OUTPUT
				SQLSMALLINT	ValType_C;
				SQLSMALLINT	ValType_SQL;
				SQLULEN		ColumnSize;
				SQLPOINTER	ValPtr;
				SQLLEN*		ValLen;			// *ValLen <- 0 || SQL_NTS
			} typedef BindParamInfo, *PBindParamInfo;
			typedef std::list<BindParamInfo> BINDPARAM_LST;

			struct _BindResultInfo
			{
				SQLSMALLINT ValType_C;
				SQLPOINTER	ValPtr;
				SQLLEN		ValPtrSize;
				SQLLEN*		ValLen;
			} typedef BindResultInfo, *PBindResultInfo;
			typedef std::list<BindResultInfo> BINDRES_LST;

		public:
			explicit CStatementHandle(SQLHDBC ConnectionHandle);
			virtual ~CStatementHandle();

		public:
			virtual bool SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength);

			bool Prepare(const std::wstring& sql);
			bool BindParam(BINDPARAM_LST& BindParams);
			bool Execute();
			bool GetExecRowCount(SQLLEN& cnt);
			bool BindRes(BINDRES_LST& BindRess);
			bool Fetch();

		private:
			SQLHDBC m_ConnectionHandle;
		};
	}
}