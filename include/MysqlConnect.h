#pragma once

#include <vector>
#include <string>

class CSelectResult
{
public:
	std::vector< std::string > m_vtFieldValues;
	unsigned int m_nRowCount;			// m_vtFieldValues.size()/m_nRowCount ����filedcount
	unsigned int m_nFieldCount;

	std::string & GetField( unsigned int nRow, unsigned int nFieldIndex ){
		return m_vtFieldValues[ nRow * m_nFieldCount + nFieldIndex ];
	}
};

class CInsertResult{
public:
	unsigned long long m_nInsertID;
};

class CUpdateOrDeleteResult{
public:
	unsigned long long m_nAffectedRows;
};


struct S_MysqlConnectPimpl;

class CMysqlConnect
{
public:
	CMysqlConnect(void);
	virtual ~CMysqlConnect(void);

public:
	bool Connect(const char * host, const char * username, 
		const char * password, const char * db, unsigned int port );

	bool IsConnected();

	bool ExecuteSql( const char * sql, 
		std::vector< std::string > * pvtFieldValues = 0,
		unsigned int * pRowCount = 0,			// �����select����Ҫ֪���ж�����
		unsigned int * pFieldCount = 0,			// �����select������Ҫ֪���ж��ٸ�field
		unsigned long long * pAffectedRows = 0,		// �����update, insert��䣬������֪���ж�������Ӱ��
		unsigned long long * pInsertID = 0,			// �����insert����Ҫ֪���ϴβ����ID
		unsigned int * pWarningCount = 0
		);

	bool ExecuteSelect( const char * sql, CSelectResult & ret );
	bool ExecuteInsert( const char * sql, CInsertResult & ret );
	bool ExecuteUpdateOrDelete ( const char * sql, CUpdateOrDeleteResult & ret );

	void * GetHandle();					// ��ȡ MYSQL ָ�� 

	const std::string & GetCurrentError();
	int GetReturnValue();

private:
	S_MysqlConnectPimpl * m_pimpl;

};

