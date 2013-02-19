#pragma once

namespace ODBCLib {

class CODBCHandle;

class CDiagInfo {
// private class
private:
	struct _FieldInfo {
	public:
		SQLINTEGER rowNumber;
		USHORT line;
		SDWORD msgState;
		SDWORD severity;
		std::wstring procName;
		std::wstring serverName;
	} typedef FieldInfo;

	class Record {
	public:
		Record(const wchar_t* state, int err, const wchar_t* msg, const FieldInfo& field):
		  m_state(state), m_err(err), m_msg(msg), m_field(field){};
		~Record(){};
	public:
		std::wstring GetState() const {
			return m_state;
		};
		int GetErrorCode() const {
			return m_err;
		};
		std::wstring GetErrorMessage() const {
			return m_msg;
		};
		const FieldInfo& GetFieldInfo() const {
			return m_field;
		};
	private:
		std::wstring m_state;
		int m_err;
		std::wstring m_msg;
		FieldInfo m_field;
	};

// constructor & destructor
public:
	explicit CDiagInfo(CODBCHandle& handle);
	~CDiagInfo();

// public const method
public:
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

	std::wstring description() const;

// private method
private:
	SQLINTEGER GetRecordCount() const;

	FieldInfo GetDiagFieldInfo(SQLSMALLINT record) const;
	std::wstring GetDiagFieldInfo_String(SQLSMALLINT record, SQLSMALLINT diagId) const;

// private member
private:
	SQLSMALLINT m_type;
	SQLHANDLE m_handle;

	SQLINTEGER m_recordCount;
	std::vector<Record> m_errors;
};	// end of... class CStatementHandle

};	// end of... namespace ODBCLib
