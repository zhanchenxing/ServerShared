#include "MysqlConnect.h"

#include <WinSock2.h>
#include <mysql.h>

#define MYSQL_HANDLE m_pimpl->m_pMySQL
#define RETURN_VALUE m_pimpl->m_nReturnValue
#define SAVE_ERROR_INFO m_pimpl->m_strCurrentError = mysql_error(MYSQL_HANDLE)


struct S_MysqlConnectPimpl{
	MYSQL * m_pMySQL;

	int m_nReturnValue;
	std::string m_strCurrentError;

	bool m_bConnected;

	S_MysqlConnectPimpl():m_pMySQL(0), m_bConnected(false), m_nReturnValue(0){
	}
};

const std::string & CMysqlConnect::GetCurrentError(){
	return m_pimpl->m_strCurrentError;
}
int CMysqlConnect::GetReturnValue(){
	return m_pimpl->m_nReturnValue;
}


CMysqlConnect::CMysqlConnect(void): m_pimpl( new S_MysqlConnectPimpl )
{
	MYSQL_HANDLE = mysql_init(NULL);
}


CMysqlConnect::~CMysqlConnect(void)
{
	mysql_close(MYSQL_HANDLE);
}


bool CMysqlConnect::Connect(const char * host, const char * username, 
	const char * password, const char * db, unsigned int port ){
		if(  mysql_real_connect(MYSQL_HANDLE, host, username, password, db, port, 0, 0) == NULL )
		{
			SAVE_ERROR_INFO;
			return false;
		}

		m_pimpl->m_bConnected = true;
		return true;
}

bool CMysqlConnect::IsConnected(){
	return m_pimpl->m_bConnected;
}

//int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
//
//	puts("in filter.");
//
//	if (code == EXCEPTION_ACCESS_VIOLATION) {
//
//		puts("caught AV as expected.");
//
//		return EXCEPTION_EXECUTE_HANDLER;
//
//	}
//
//	else {
//
//		puts("didn't catch AV, unexpected.");
//
//		return EXCEPTION_CONTINUE_SEARCH;
//
//	};
//
//}

//int ExecuteSqlWithNoException( MYSQL * conn, const char * sql ){
//	__try
//	{
//		// ������ܱ��׳�seh�쳣
//		return mysql_query ( conn, sql);
//	}
//	__except(filter(GetExceptionCode(), GetExceptionInformation())) {
//		return -1;
//	}
//}

bool CMysqlConnect::ExecuteSql( const char * sql, 
	std::vector< std::string > * pvtFieldValues,
	unsigned int * pRowCount,			// �����select����Ҫ֪���ж�����
	unsigned int * pFieldCount,			// �����select������Ҫ֪���ж��ٸ�field
	unsigned long long * pAffectedRows,		// �����update, insert��䣬������֪���ж�������Ӱ��
	unsigned long long * pInsertID,			// �����insert����Ҫ֪���ϴβ����ID
	unsigned int * pWarningCount
	){
		//m_pimpl->m_nReturnValue = ExecuteSqlWithNoException(MYSQL_HANDLE, sql);
		//if( m_pimpl->m_nReturnValue ){
		//	SAVE_ERROR_INFO;
		//	return false;
		//}

		if( !m_pimpl->m_bConnected ){
			m_pimpl->m_nReturnValue = -1;
			m_pimpl->m_strCurrentError = "ExecuteSql before connected.";
			return false;
		}

		// ������ܱ��׳�seh�쳣
		if( RETURN_VALUE = mysql_query ( MYSQL_HANDLE, sql) ){
			SAVE_ERROR_INFO;
			return false;
		}

		// ��ȡ�������
		if( pWarningCount ){
			*pWarningCount = mysql_warning_count(MYSQL_HANDLE);
		}

		// ��ȡ���β����ID
		if( pInsertID ){
			*pInsertID = mysql_insert_id(MYSQL_HANDLE);
		}

		if( pFieldCount ) {
			*pFieldCount = mysql_field_count(MYSQL_HANDLE);
		}

		if( pAffectedRows ){
			*pAffectedRows = mysql_affected_rows(MYSQL_HANDLE);
		}

		if( pRowCount || pvtFieldValues ){

			// ��Ҫȫ��ȡһ��ֵ
			unsigned int nFieldCount = pFieldCount?*pFieldCount:(mysql_field_count(MYSQL_HANDLE));
			unsigned int nRowCount = 0;
			MYSQL_RES * res = mysql_store_result(MYSQL_HANDLE);
			if( res ){
				MYSQL_ROW row;
				while( (row = mysql_fetch_row(res) ) != 0 ){

					++nRowCount;

					if( pvtFieldValues ){
						unsigned long *lengths = mysql_fetch_lengths(res);
						for ( unsigned int n = 0; n<nFieldCount; ++n)
						{
							pvtFieldValues->push_back( std::string( row[n], lengths[n] ) );
						}
					}

				}
			}


			if( pRowCount ){
				*pRowCount = nRowCount;
			}

		}


		return true;
}

bool CMysqlConnect::ExecuteSelect( const char * sql, CSelectResult & ret ){
	return ExecuteSql( sql, &ret.m_vtFieldValues,&ret.m_nRowCount,  &ret.m_nFieldCount, 0, 0, 0 );
}

bool CMysqlConnect::ExecuteInsert( const char * sql, CInsertResult & ret ){
	return ExecuteSql( sql, 0, 0, 0, 0, &ret.m_nInsertID, 0 );
}

bool CMysqlConnect::ExecuteUpdateOrDelete ( const char * sql, CUpdateOrDeleteResult & ret ){
	return ExecuteSql(sql, 0, 0, 0, &ret.m_nAffectedRows, 0, 0);
}

void * CMysqlConnect::GetHandle()					// ��ȡ MYSQL ָ�� 
{
	return MYSQL_HANDLE;
}


