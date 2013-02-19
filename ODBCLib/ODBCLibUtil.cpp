#include "stdafx.h"
#include "ODBCLibUtil.h"

#include <sstream>

std::wstring ODBCLib::Datetime2String(const std::vector<unsigned char>& timestamp) {
	unsigned int dateCount = *(unsigned int*)(&timestamp[0]);
	unsigned int timeCount = *(unsigned int*)(&timestamp[4]);
	std::wostringstream oss;
	oss << L"date:" << dateCount << L" time:" << timeCount;
	return oss.str();
}
