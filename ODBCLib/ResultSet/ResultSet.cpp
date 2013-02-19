#include "stdafx.h"
#include "ResultSet.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../Handle/StatementHandle.h"
#include "../ErrInfo/ErrorInfo.h"


ODBCLib::CBindColumn::CBindColumn(const ColumnInfo& columnInfo):
	m_columnInfo(columnInfo), m_columnBytes(0), m_bindRowCount(0), m_buffer(), m_bindLength() {
	// 列サイズ決定
	m_columnBytes = columnInfo.bytes;
	switch(m_columnInfo.type) {
	case SQL_CHAR:
	case SQL_VARCHAR:
		m_columnBytes += sizeof(char);
		break;
	case SQL_WCHAR:
	case SQL_WVARCHAR:
		m_columnBytes += sizeof(wchar_t);
		break;
	}
}
ODBCLib::CBindColumn::~CBindColumn() {
}

void ODBCLib::CBindColumn::Bind(int rowCount, ODBCLib::CStatementHandle& statementHandle, SQLUSMALLINT col) {
	// バッファ初期化
	m_bindRowCount = rowCount;
	m_buffer.clear();
	m_buffer.resize((m_bindRowCount) * m_columnBytes, 0);
	m_bindLength.clear();
	m_bindLength.resize(m_bindRowCount, 0);
/*
std::wcout << L"CBindColumn::Bind() col=" << col <<
	L" RowCount=" << rowCount <<
	L" m_buffer.size()=" << m_buffer.size() <<
	L" m_bindLength.size()=" << m_bindLength.size() <<
	L" columnBytes=" << columnBytes << std::endl;
*/
	// バインド
	SQLRETURN ret = statementHandle.BindCol(col, m_columnInfo.type, &(*m_buffer.begin()), m_columnBytes, &(*m_bindLength.begin()));
	if(ret != SQL_SUCCESS) {
		std::wcerr << L"CBindColumn::Bind() CStatementHandle::BindCol() error(" << ret << L")" << std::endl <<
			ODBCLib::CErrorInfo(SQL_HANDLE_STMT, (SQLHSTMT)statementHandle).description() << std::endl;
	}
}

std::wstring ODBCLib::CBindColumn::description_column() const {
	std::wostringstream oss;
	oss <<
		L"name[" << m_columnInfo.name << L"] " <<
		L"type[" << m_columnInfo.type << L":" << m_columnInfo.typeName << L"] " <<
		L"len=" << m_columnInfo.length << L" " <<
		L"bytes=" << m_columnInfo.bytes;
/*
		L"colName[" << m_columnInfo.columnName << L"] " <<
		L"tableName[" << m_columnInfo.tableName << L"] " <<
		L"baseTName[" << m_columnInfo.baseTableName << L"] " <<
		L"schemaName[" << m_columnInfo.schemaName << L"] " <<
		L"catalogName[" << m_columnInfo.catalogName << L"] " <<
		L"label[" << m_columnInfo.label << L"] " <<

		L"pre[" << m_columnInfo.literalPrefix << L"] " <<
		L"suf[" << m_columnInfo.literalSuffix << L"] " <<

		L"conType=" << m_columnInfo.conciseType << L" " <<
		L"locTypename[" << m_columnInfo.localTypeName << L"] " <<

		L"dispSize=" << m_columnInfo.displaySize << L" " <<

		L"IsAutoUniqueVal=" << m_columnInfo.isAutoUniqueValue << L" " <<
		L"IsCaseSensitive=" << m_columnInfo.isCaseSesitive << L" " <<
		L"IsFixedPreScale=" << m_columnInfo.isFixedPrecScale << L" " <<
		L"IsNullable=" << m_columnInfo.isNullable << L" " <<
		L"IsUnsigned=" << m_columnInfo.isUnsigned << L" " <<

		L"numPreRadix=" << m_columnInfo.numPrecRadix << L" " <<
		L"precision=" << m_columnInfo.precision << L" " <<
		L"scale=" << m_columnInfo.scale << L" " <<
		L"searchable=" << m_columnInfo.searchable << L" " <<
		L"unnamed=" << m_columnInfo.unnamed << L" " <<
		L"updatable=" << m_columnInfo.updatable;
*/
	return oss.str();
}

std::wstring ODBCLib::CBindColumn::description_value(int idx) const {
	std::wostringstream oss;

	if(idx < m_bindRowCount) {
		const unsigned char* value = GetValuePtr(idx);
		switch(columnType()) {
		case SQL_INTEGER:	oss << *((int*)value);	break;
		case SQL_SMALLINT:	oss << *((short*)value);	break;
		case SQL_BIGINT:	oss << *((__int64*)value);	break;
		case SQL_CHAR:
		case SQL_VARCHAR:	oss << (char*)value;	break;
		case SQL_WCHAR:
		case SQL_WVARCHAR:	oss << (wchar_t*)value;	break;
//		case SQL_DATETIME:	oss << ODBCLib::Datetime2String(*it);	break;
		}
		oss << L"(" << m_bindLength[idx] << L")";
	}

	return oss.str();
}


ODBCLib::CResultSet::CResultSet(ODBCLib::CStatementHandle& statementHandle):
	m_statementHandle(statementHandle), m_bindColumns(), m_rowBytes(0), m_colBytesMax(0),
	m_rowCount(0), m_rowStatuses() {

	SQLSMALLINT colCount = m_statementHandle.GetResult_ColCount();
	if(colCount > 0) {
		m_bindColumns.reserve(colCount);
		for(SQLSMALLINT i = 1; i <= colCount; i++) {
			ColumnInfo colInfo;
			colInfo.name = m_statementHandle.GetResult_ColAttrString(i, SQL_DESC_NAME);
			colInfo.type = (SQLSMALLINT)m_statementHandle.GetResult_ColAttr(i, SQL_DESC_TYPE);
			colInfo.typeName = m_statementHandle.GetResult_ColAttrString(i, SQL_DESC_TYPE_NAME);
			colInfo.length = m_statementHandle.GetResult_ColAttr(i, SQL_DESC_LENGTH);
			colInfo.bytes = m_statementHandle.GetResult_ColAttr(i, SQL_DESC_OCTET_LENGTH);
/*			
			colInfo.columnName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_BASE_COLUMN_NAME);
			colInfo.tableName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_TABLE_NAME);
			colInfo.baseTableName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_BASE_TABLE_NAME);
			colInfo.schemaName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_SCHEMA_NAME);
			colInfo.catalogName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_CATALOG_NAME);
			colInfo.label = statementHandle.GetResult_ColAttrString(i, SQL_DESC_LABEL);

			colInfo.literalPrefix = statementHandle.GetResult_ColAttrString(i, SQL_DESC_LITERAL_PREFIX);
			colInfo.literalSuffix = statementHandle.GetResult_ColAttrString(i, SQL_DESC_LITERAL_SUFFIX);

			colInfo.conciseType = statementHandle.GetResult_ColAttr(i, SQL_DESC_CONCISE_TYPE);
			colInfo.localTypeName = statementHandle.GetResult_ColAttrString(i, SQL_DESC_LOCAL_TYPE_NAME);

			colInfo.displaySize = statementHandle.GetResult_ColAttr(i, SQL_DESC_DISPLAY_SIZE);

			colInfo.isAutoUniqueValue = statementHandle.GetResult_ColAttr(i, SQL_DESC_AUTO_UNIQUE_VALUE);
			colInfo.isCaseSesitive = statementHandle.GetResult_ColAttr(i, SQL_DESC_CASE_SENSITIVE);
			colInfo.isFixedPrecScale = statementHandle.GetResult_ColAttr(i, SQL_DESC_FIXED_PREC_SCALE);
			colInfo.isNullable = statementHandle.GetResult_ColAttr(i, SQL_DESC_NULLABLE);
			colInfo.isUnsigned = statementHandle.GetResult_ColAttr(i, SQL_DESC_UNSIGNED);

			colInfo.numPrecRadix = statementHandle.GetResult_ColAttr(i, SQL_DESC_NUM_PREC_RADIX);
			colInfo.precision = statementHandle.GetResult_ColAttr(i, SQL_DESC_PRECISION);
			colInfo.scale = statementHandle.GetResult_ColAttr(i, SQL_DESC_SCALE);
			colInfo.searchable = statementHandle.GetResult_ColAttr(i, SQL_DESC_SEARCHABLE);
			colInfo.unnamed = statementHandle.GetResult_ColAttr(i, SQL_DESC_UNNAMED);
			colInfo.updatable = statementHandle.GetResult_ColAttr(i, SQL_DESC_UPDATABLE);
*/
			m_bindColumns.push_back(CBindColumn(colInfo));

			int columnBytes = colInfo.bytes;
			switch(colInfo.type) {
			case SQL_CHAR:
			case SQL_VARCHAR:
				columnBytes += sizeof(char);
				break;
			case SQL_WCHAR:
			case SQL_WVARCHAR:
				columnBytes += sizeof(wchar_t);
				break;
			}
			m_rowBytes += columnBytes;
			m_colBytesMax = max(m_colBytesMax, columnBytes);
		}
	}
}

ODBCLib::CResultSet::~CResultSet() {
}

SQLRETURN ODBCLib::CResultSet::Fetch(int rowCount/*= 0*/) {

	// Fetch行数を確定する
	if(rowCount <= 0) {
		// 全体の最大バッファサイズ、列毎の最大バッファサイズから超えない範囲の行数を取得
		// （一行で超えてしまう場合は一行だけ）
		rowCount = min(max(min(MaxBufferSize / m_rowBytes, MaxColumnBufferSize / m_colBytesMax), 1), AutoRowCountMax);
	}

	// Fetchする行数をセット
	m_statementHandle.SetFetchCount((SQLULEN)rowCount);
	// 列バインドにセット
	m_statementHandle.SetColWiseBind();
	// Fetchした行数を受け取るポインタをセット
	m_rowCount = 0;
	m_statementHandle.SetFetchedCountPtr(&m_rowCount);
	// 行ステータスを受け取る配列をセット
	m_rowStatuses.clear();
	m_rowStatuses.resize(rowCount, SQL_ROW_NOROW);
	m_statementHandle.SetRowStatusArray(m_rowStatuses);

	// 行数を渡し、ステートメントハンドルにバインドする
	SQLUSMALLINT col = 1;
	BindColumns::iterator it;
	for(it = m_bindColumns.begin(); it != m_bindColumns.end(); ++it) {
		it->Bind(rowCount, m_statementHandle, col);
		col++;
	}

	// Fetch
	return m_statementHandle.Fetch();
}

std::wstring ODBCLib::CResultSet::description() const {
	std::wostringstream oss;

	oss <<	L"結果セット 列数=" << m_bindColumns.size() <<
			L" 一行のバッファサイズ=" << m_rowBytes <<
			L" 一列の最大バッファサイズ=" << m_colBytesMax << std::endl;
	int idx = 0;
	BindColumns::const_iterator it;
	for(it = m_bindColumns.begin(); it != m_bindColumns.end(); ++it) {
		if(idx > 0) {
			oss << std::endl;
		}
	
		oss << std::right << std::setw(6) << idx << std::left << L" : " <<
			it->description_column();

		idx++;
	}
	return oss.str();
}

std::wstring ODBCLib::CResultSet::description_resultset() const {
	std::wostringstream oss;

	oss << L"結果セット バッファ件数=" << m_rowStatuses.size() << std::endl;

	bool isOutput = false;
	SQLULEN idx = 0;
	RowStatusArray::const_iterator it;
	for(it = m_rowStatuses.begin(); it != m_rowStatuses.end(); ++it) {
		if((*it) != SQL_ROW_NOROW) {
			if(isOutput) {
				oss << std::endl;
			}
			oss << idx << L" : " << row2str(idx);
			isOutput = true;
		}
		idx++;
	}

	return oss.str();
}

std::wstring ODBCLib::CResultSet::row2str(SQLULEN idx) const {
	std::wostringstream oss;

	BindColumns::const_iterator it;
	for(it = m_bindColumns.begin(); it != m_bindColumns.end(); ++it) {
		if(it != m_bindColumns.begin()) {
			oss << L" | ";
		}
		oss << it->description_value(idx);
	}

	return oss.str();
}
