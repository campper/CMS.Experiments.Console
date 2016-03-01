#include <windows.h>
#include <strstream>
#include <sstream>

#include "experiment_oracle.h"

#include "PautaRule.h"
#include "Optics.h"
#include "StringUtil.h"
#include "DebugNew.h"

#include "DataPoint.h"
#include "DataPoint2.h"
#include "DbHelper.h"
//#include "MysqlHelper.h"
#include "OracleHelper.h"
//#include "SqlServerHelper.h"

#include "CMSException.h"
using namespace std;

static std::string tablename_ = "tr_oil_olm";

inline int symbolCount(const char *const s,const char c)
{
	int count = 0;
	for (int i = 0;i < strlen(s);i++)
	{
		if (s[i] == c)
		{
			count++;
		}
	}
	return count;
}

inline int indexof(char* srcstr, char* deststr)
{
	int j = 0;
	int i = -1;
	try
	{
		for (i = 0;srcstr[i] != '\0';i++)
		{
			if (deststr[0] == srcstr[i])
			{
				while (deststr[j] != '\0' && srcstr[i + j] != '\0')
				{
					j++;
					if (deststr[j] != srcstr[i + j])
					{
						break;
					}
				}
			}
			if (deststr[j] == '\0')
				return i;
		}
	}
	catch (...)
	{
		cout << "error:" <<srcstr<<":"<<deststr<< endl;
		return -1;
	}
	return i;
}

inline char *substr(char* str, int start, int end)
{
	char* s1=new char;
	int j=0;
	if (start >= end)
	{
		cerr << "wrong position..." << endl;
	}
	else
	{
		//start--;
		for (int i = start;i < end;i++,j++)
		{
			s1[j] = str[i];
		}
		int len = end - start;
		s1[len] = '\0';
	}
	return s1;
}

inline char* itostr(char *str, int i) //��iת��λ�ַ�������str
{
	sprintf(str, "%d", i);
	return str;
}

//��ʾ��ǰϵͳʱ��
inline char* dsptime(struct tm *ptm)
{
	char* now = (char*)malloc(255);
	char *pxq[] = { "��", "һ", "��", "��", "��", "��", "��" };

	cout << ptm->tm_year + 1900 << "��" << ptm->tm_mon + 1 << "��" << ptm->tm_mday << "�� ";
	cout << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " ";
	sprintf(now, "%d��%d��%d�� %d:%d:%d\0", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	//cout << " ����" << pxq[ptm->tm_wday] << " ����ĵ�" << ptm->tm_yday << "�� " << endl;
	return now;
}

inline double todouble(string s)
{
	double res = 0;
	int flag = 0, xs = 0, ws = 0;
	int jc = 1;
	for (int i = 0; i < (int)s.size(); i++)
	{
		if (s[i] == '.')
		{
			flag = 1;
			continue;
		}
		if (flag == 0)
		{
			res *= 10;
			res += s[i] - '0';
		}
		else
		{
			ws++;
			xs *= 10;
			xs += s[i] - '0';
		}
	}
	for (int j = 0; j < ws; j++)
	{
		jc *= 10;
	}
	res += xs*1.0 / jc;
	return res;
}

std::string experiment_oracle::getResult() const
{
	return result_;
}

//���÷�װ��OracleHelper��
void experiment_oracle::conn() {
	try {
		this->oracle_helper_ = new OracleHelper("tjcms", "2491", "orcl", 12);
		if (this->oracle_helper_) {
			cout << "connected to oracle database" << endl;
		}
		else {
			cout << "failed to connect oracle database" << endl;
		}
		
	}
	catch (SQLException &sqlex)
	{
		cerr << sqlex.getErrorCode() << ":" << sqlex.getMessage() << endl;
	}
	catch (exception &excp)
	{
		cerr << excp.what() << endl;
	}
}
//ֱ�ӵ���oci.h
/*void experiment_oracle::conn()
{
	try {
		//this->oracle_helper_ = new oraclehelper("tjcms", "2491", "orcl", 12);
		this->env_ = environment::createenvironment();

		this->conn_ = env_->createconnection("tjcms", "2491");
		cout << "connected to oracle database" << endl;
	}
	catch (sqlexception &sqlex)
	{
		cerr << sqlex.geterrorcode() << ":" << sqlex.getmessage() << endl;
	}
	catch (exception &excp)
	{
		cerr << excp.what() << endl;
	}
}*/

//�ָ��ַ���
inline void split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator, start);
		if (index != string::npos)
		{
			substring = str.substr(start, index - start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator, index);
			if (start == string::npos) return;
		}
	} while (index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

//���������vector
void experiment_oracle::savedata(char* sql, int flag)
{
	this->stmt_ = this->conn_->createStatement();
	this->stmt_->executeQuery(sql);
	this->rset_ = this->stmt_->getResultSet();

	this->fieldnum_ = this->getColumnNum(sql);
	try
	{
		while(this->rset_->next())
		{
			double *tmp = new double[fieldnum_];
			memset(tmp, 0, sizeof(double)*fieldnum_);

			DataPoint* _tmpdp = new DataPoint(tmp, (size_t)fieldnum_, ""); //�ڴ�й©
			for (int i = 0;i < this->fieldnum_ ;i++)
			{
				double value = this->rset_->getDouble(i+1);
				tmp[i] = value;
			}
			_tmpdp->SetDimension(tmp);
			this->dpdata_.push_back(_tmpdp);
			//this->dpdata_.push_back(xnr::owner_ptr<DataPoint>(_tmpdp));
			//delete _tmpdp;
			delete[] tmp;
		}
	}
	catch (SQLException &sqlex)
	{
		cerr << sqlex.getErrorCode() << ":" << sqlex.getMessage() << endl;
	}
	catch (exception* ex)
	{
		cout << ex->what() << endl;
	}
	//delete this->m_helper_;
}

//std::vector<DataPoint*> experiment_oracle::getDPdata() const
std::vector<DataPoint*> experiment_oracle::getDPdata() const
{
	return this->dpdata_;
}

experiment_oracle::experiment_oracle()
{
	cout << "experiment_optics ..." << endl;
}

experiment_oracle::~experiment_oracle()
{
	if (this->tmpcell_ != NULL)
		delete[] this->tmpcell_;
	cout << "~experiment_optics ..." << endl;
}

vector<double> experiment_oracle::getdata() const
{
	return this->data_;
}

int experiment_oracle::getFieldNum() const
{
	return this->fieldnum_;
}

//����ת��
template<class Out_Type, class In_Type>
inline Out_Type type_convert(const In_Type& T)
{
	stringstream ss;
	ss << T;
	Out_Type result;
	ss >> result;
	return result;
}

//�����ı�
inline void createdir(char* dir)
{
	ifstream fin(dir);
	if (!fin)
	{
		try
		{
			char mkdir[255];
			sprintf(mkdir, "%s %s\0", "md", dir);
			system(mkdir);
			fin.close();
		}
		catch (CMSException* ex)
		{
			fin.close();
			std::cout << "create file " << dir << "error:" << ex->what() << std::endl;
		}
	}
}

//�ϲ���ɰ���������� 
inline void mergeName(char* dir, char* name, char* filename)
{
	//char filename[255];
	char date[255];
	time_t now;
	struct tm *p;
	time(&now);
	p = localtime(&now);
	assert(p);
	strftime(date, 255, "%Y%m%d", p);
	sprintf(filename, "%s%s_%s.txt\0", dir, name, date);
}

inline const char* now()
{
	time_t nowtime;
	nowtime = time(NULL); //��ȡ����ʱ�� 
	struct tm *local;
	local = localtime(&nowtime);  //��ȡ��ǰϵͳʱ��
	return dsptime(local);
}

//�ж��ļ��Ƿ���� δ�õ�
bool isExistFile(const char *pszFileName) {
	FILE *fp = fopen(pszFileName, "rb");
	if (fp == NULL)
		return false;
	fclose(fp);
	return true;
}

//�����ļ� ������ʱҲ����
inline void createfile(char* path)
{
	ifstream fin(path);
	ofstream oin;
	if (!fin)
	{
		try
		{
			oin.open(path, ios::out | ios::trunc);
			oin.close();
			fin.close();
		}
		catch (CMSException* ex)
		{
			oin.close();
			fin.close();
			std::cout << "create file " << path << "error:" << ex->what() << std::endl;
		}
	}
}

DataPoint* experiment_oracle::getDataPointPtr() const
{
	return this->tmpcell_;
}

int experiment_oracle::getColumnNum(char* sql)
{
	int _n = 0;
	char* __start = "select";
	char* __end = "from";
	if (::indexof(sql, __start) > -1)
	{
		int _enddot = ::indexof(sql, __end);
		char* select = ::substr(sql, ::indexof(sql, __start)+(int)(strlen(__start)), _enddot);
		_n = ::symbolCount(select, ',')+1;
	}
	
	return _n;
}

//�����㷨������
//void experiment_oracle::saveOPTICSResult(std::vector<DataPoint*> resultlist,const ofstream& out)
//{
//	for (std::vector<DataPoint*>::iterator optic_item = resultlist.begin(); optic_item != resultlist.end(); ++optic_item)
//	{
//
//		out << "dpId: " << dynamic_cast<DataPoint *>(*optic_item)->GetDpId() << "| ClusterId :" << dynamic_cast<DataPoint *>(*optic_item)->GetClusterId() << "\t| reachable distance: " << dynamic_cast<DataPoint *>(*optic_item)->getReachableDistance() << "\t| core distance: " << dynamic_cast<DataPoint *>(*optic_item)->getCoreDistance();
//		int __len = dynamic_cast<DataPoint *>(*optic_item)->size();
//		out << "| dimensions:\t";
//		for (int i = 0;i < __len;i++)
//		{
//			double __value = dynamic_cast<DataPoint *>(*optic_item)->GetDimension()[i];
//			out << __value << "\t";
//		}
//		out << endl;
//	}
//}

//���������
//OPTICS�����ܶ�����
//���ܣ�δ���س���ʱ��ͳ��
int main(void)
{
	char* dir = "E:\\Experiments\\OPTICS\\";
	experiment_oracle e;
	::createdir(dir);

	Optics o;

	char optics_path[255] = { 0 };
	::mergeName(dir, "optics_out", optics_path);
	::createfile(optics_path);


	ofstream optics_out(optics_path, ios::out | ios::trunc);
	assert(optics_out);

	try
	{
		e.conn();

		char * tr_oil_olm_sql = "select t.����,t.����,t.��ϩ,t.��Ȳ,t.����,t.һ����̼,t.������̼ from tr_oil_olm t  where t.���б��='2#����' and t.����ʱ��<=to_date('2012-12-01','yyyy-MM-dd HH24:mi:ss')";

		char * gis_sql = "select �¶�,����ѹ��,���ʪ��,�ܶ�,ѹ��20��,�����,������,¶��,ˮ�� from gis_sf6_day_avg t where t.���б�� ='102GIS' and t.ͨ��='SF6 6-2' and t.����ʱ��<'2012-05-01' order by t.����ʱ�� asc";

		char * tr_oil_fields = "[cms ] [table: tr_oil_olm] [fields:\t����\t����\t��ϩ\t��Ȳ\t����\tһ����̼\t������̼\t����]";
		char * gis_fields = "[cms ] [table: gis_sf6_day_avg] [fields:\t�¶�\t����ѹ��\t���ʪ��\t�ܶ�\tѹ��20��\t�����\t������\t¶��\tˮ��]";


		//��ȡ����
		optics_out << "SQL Query :" << tr_oil_olm_sql << endl;

		cout << "Data collecting ..." << endl;
		optics_out << "Data collecting ...  " << endl;
		e.savedata(tr_oil_olm_sql,0);
		
		//��ʵ������ʼ������
		o.InitData(e.getDPdata());
		cout << "End collecting ..." << endl;
		optics_out << "End collecting ...  " << endl;

		//�㷨����
		o.setParameters(2, 4);
		o.startAnalysis();//��Ҫ��������

		std::vector<DataPoint*> optics_result = o.getSeedList();
		cout << "Save Data..." << endl;
		cout << tr_oil_fields << endl;
		optics_out << "Save Data..." << endl;
		optics_out << tr_oil_fields << endl;
		optics_out << "epsilon:\t"<<o.getEpsilon() <<"\t minPts: \t"<< o.getMinPts() << endl;
		optics_out << "seedList Num: "<<o.getSeedList().size() << endl;

		//�����ӡ
		//����Optics�ܶȾ�����
		//e.saveOPTICSResult(optics_result, optics_out);
		for (std::vector<DataPoint*>::iterator optic_item = optics_result.begin(); optic_item != optics_result.end(); ++optic_item)
		{

			optics_out << "dpId:\t " << (*optic_item)->GetDpId() << "\t| ClusterId :\t" << (*optic_item)->GetClusterId() << "\t| reachable distance:\t " << (*optic_item)->getReachableDistance() << "\t| core distance:\t " << (*optic_item)->getCoreDistance();
			size_t __len = (*optic_item)->size();
			optics_out << "\t| dimensions:\t";
			for (int i = 0;i < __len;i++)
			{
				double __value = (*optic_item)->GetDimension()[i];
				optics_out << __value << "\t";
			}
			optics_out << endl;
		}
		//����ָ�룬��ֹ�ڴ�й©
		e.getDPdata().clear();
		optics_out.clear();
		optics_out.close();

		for (int i = 0;i < e.getDPdata().size();i++)
		{
			delete e.getDPdata()[i];
		}
	}
	catch (exception* ex)
	{
		cout << ex->what() << endl;
		optics_out.close();
		return -1;
	}
	return 0;
}
