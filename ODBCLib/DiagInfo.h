#pragma once

//#include "DiagRecord.h"

namespace ODBCLib {

class CODBCHandle;
class CDiagRecord;

class CDiagInfo {
private:
	typedef std::vector<std::unique_ptr<CDiagRecord> > DiagRecords;

// constructor & destructor
public:
	explicit CDiagInfo(std::shared_ptr<CODBCHandle> handle);
	~CDiagInfo();

// public const method
public:
/*
	unsigned int GetCount() const {
		return m_errors.size();
	};
	std::wstring GetState(unsigned int idx) const {
		if(idx >= m_errors.size()) {
			return std::wstring();
		}
		return m_errors[idx].GetState();
	};
	int GetErrorCode(unsigned int idx) const {
		if(idx >= m_errors.size()) {
			return 0;
		}
		return m_errors[idx].GetErrorCode();
	};
	std::wstring GetErrorMessage(unsigned int idx) const {
		if(idx >= m_errors.size()) {
			return std::wstring();
		}
		return m_errors[idx].GetErrorMessage();
	};
	FieldInfo GetFieldInfo(unsigned int idx) const {
		if(idx >= m_errors.size()) {
			return FieldInfo();
		}
		return m_errors[idx].GetFieldInfo();
	};
*/
	std::wstring description() const;

// private method
private:
	int recordCount() const {
		return m_records.size();
	}

// private member
private:
	SQLSMALLINT m_type;
	SQLHANDLE m_handle;

	DiagRecords m_records;
};	// end of... class CStatementHandle

};	// end of... namespace ODBCLib
