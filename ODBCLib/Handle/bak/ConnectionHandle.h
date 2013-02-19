#pragma once

#include "ODBCHandle.h"

namespace MyLib
{
	namespace ODBC
	{
		class CConnectionHandle : public CODBCHandle
		{
		public:
			explicit CConnectionHandle(SQLHENV EnvironmentHandle);
			virtual ~CConnectionHandle();
		public:
			virtual bool SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength);

		public:
			bool Connect(const std::wstring& ServerName, const std::wstring& DBName);
			bool Connect(const std::wstring& DBName);
		private:
			bool _Connect(const std::wstring& ConnectionString);
		public:
			void DisConnect();

			bool Rollback();
			bool Commit();
		private:
			const SQLHENV m_EnvironmentHandle;
		};
	}
}