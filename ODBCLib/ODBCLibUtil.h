#pragma once

#include <vector>
#include <string>

namespace ODBCLib {

std::wstring Datetime2String(const std::vector<unsigned char>& timestamp);

};