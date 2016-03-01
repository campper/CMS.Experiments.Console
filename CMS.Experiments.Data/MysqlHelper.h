#ifndef _MYSQLHELPER_H_
#define _MYSQLHELPER_H_

#include <winsock.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mysql.h"

using namespace std;

//class MysqlAgent
//{
//private:
//	MysqlAgent();
//	~MysqlAgent();
//public:
//	static MysqlAgent *Instance();	//����ģʽ
//	//static void Destroy();
//private:
//	static MysqlAgent *m_pAgent_;
//};
class DbHelper;
class MysqlHelper : public DbHelper
{
public:
	MysqlHelper();
	~MysqlHelper();
public:
	/*
	��Ҫ�Ĺ��ܣ���ʼ�����ݿ�; �������ݿ�; �����ַ���
	��ڲ�����
	host ��MYSQL������IP
	port:���ݿ�˿�
	Db�����ݿ�����
	user�����ݿ��û�
	passwd�����ݿ��û�������
	charset��ϣ��ʹ�õ��ַ���
	Msg:���ص���Ϣ������������Ϣ

	���ڲ�����
	int ��0��ʾ�ɹ���1��ʾʧ��
	*/
	int dbConn(char *host, char * port, char * Db, char * user, char* passwd, char * charset, char * Msg);
	int dbConn();
	/*
	��Ҫ�Ĺ��ܣ���ѯ����;
	��ڲ�����
	SQL����ѯ��SQL���
	Cnum:��ѯ������
	Msg:���ص���Ϣ������������Ϣ
	std::string::result ��ֵ�ַ���
	MYSQL_RES& result ��ֵ�����

	���ڲ�����
	string ׼�����÷��ص����ݣ�������¼����0x06����,�����λ��0x05����
	��� ���صĳ��ȣ� 0�����ʾ����
	*/
	int SelectData(char * SQL, int Cnum, char * Msg, std::string& result);
	int SelectData(char * SQL, char * Msg, std::string&);
	/*
	��Ҫ���ܣ�
	�ر����ݿ�����
	*/
	void Close();

	/*
	��Ҫ���ܣ�
	���MYSQL_RES���͵����ݽ����
	*/
	MYSQL_RES* getResult() const;

	/*
	��Ҫ���ܣ�
	����MYSQL_RES���͵����ݽ����
	*/
	void setResult(MYSQL_RES* result);

	/*
	��Ҫ���ܣ�
	ִ��MYSQL ���
	��ڲ�����
	sql: ���ݿ����

	���ڲ�����
	int ִ�гɹ�������
	*/
	int ExcuteQuery(char* sql);
private:
	void InitDb();
	void InitDb(char *host, char * port, char * Db, char * user, char* passwd, char * charset);
private:
	char *dbuser_;			// "root";
	char *dbpassword_;		// "voltampcms100";
	char *dbip_;				// "localhost";
	char *dbname_;			// "cms";
	char *tablename_;		// "tr_oil_olm";
	char *charset_;			// NULL;
	char *msg_;
	char *port_;
	MYSQL *conn_;
	MYSQL_RES *dataset_;		//MySQL���ݼ�
	std::string result_;
};

#endif // _MYSQLHELPER_H_
