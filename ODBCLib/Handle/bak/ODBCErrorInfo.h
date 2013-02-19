#pragma once

#include <list>
#include <string>

namespace MyLib
{
	namespace ODBC
	{
		class CODBCErrorInfo
		{
		public:
			CODBCErrorInfo(const wchar_t* szState, int ErrCode, const wchar_t* szErrMsg):
				State(szState), ErrorCode(ErrCode), ErrorMsg(szErrMsg){};
		public:
			std::wstring	State;
			int				ErrorCode;
			std::wstring	ErrorMsg;
		};
		typedef std::list<CODBCErrorInfo> ODBC_LASTERRINFO_LIST;
	}
}