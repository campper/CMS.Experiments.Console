// K-Means.cpp : �������̨Ӧ�ó������ڵ㡣
#include <iostream>
#include <fstream>
#include <strstream>
#include <map>
#include <string>
#include <cmath>
#include "K-Means.h"
#include "distance.h"
using namespace std;

const int MAX=10000;
//vector <double> line;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction/���캯��
//////////////////////////////////////////////////////////////////////

CCluster::CCluster()
{
	centernum=0;
	patternnum=0;
	m_pattern=NULL;
	m_center=NULL;
}

//��������
CCluster::~CCluster()
{
}

/***************************************************************
*��������		GetDistance(CCluster::Pattern pattern,  CCluster::Center center, const int distype)
*����			CCluster::Pattern pattern  ��Ʒ
*				CCluster::Center center    ����
*����ֵ			double
*��������		������Ʒ�;������ļ�ľ���
************************************************************/
double CCluster::GetDistance(CCluster::Pattern  pattern, CCluster::Center center)
{
	//int i;
	double   result=0;

	if (1)    //Ӧ�ÿ��Ǿ������İ�����ģʽ��Ŀ�����Ϊ0Ӧ�á���������������������
	{
		//for (i=0; i<N; i++)
		//	result+=(pattern.feature[i]-center.feature[i])*(pattern.feature[i]-center.feature[i]);
		//return sqrt(result);
		result = distance::getEuclidean(pattern.feature, center.feature, N);
		return result;
	}
	else
	{
	}
}

/*********************************************************
*��������		CalCenter(CCluster::Center *pcenter)
*����			CCluster::Center *pcenter
*����ֵ		    void
*��������		��������pcenter������ֵ������������Ʒ�ľ�ֵ��������Ʒ����
************************************************************/	
void CCluster::CalCenter(Center* pcenter)
{
	double *temp;//��ʱ�洢���ĵ�����ֵ
	temp=new double[N];
	int a=0;//��ʱ�洢���ĵ�����ֵ

	for (int i=0;i<N;i++)//�������
		temp[i]=0;
	
	for (int i = 0; i < patternnum; i++)
	{
		if (m_pattern[i].category == pcenter->index)//�ۼ�����pcenter��������Ʒ����������ֵ
		{
			a++;
			for (int j = 0; j < N; j++)
				temp[j] += m_pattern[i].feature[j];
		}
	}

	pcenter->patternnum=a;
	
	for (int i = 0; i < N; i++)
	{
		if (a != 0)   //������������������Ϊ0
		{
			pcenter->feature[i] = (double)(temp[i] / (double)a);//����ֵȡ�ۼӺ͵ľ�ֵ
		}
		else
		{
			pcenter->feature[i] = temp[i];     //�˴����ǸĽ���������������������
		}
	}
}

/************************************************************
*��������		K_means()
*����			void
*����ֵ		    void
*��������		����K��ֵ�㷨��ȫ����Ʒ���з���(����pattern���顢������Ŀ������ά����������Ŀ���Ѹ�ֵ���)
************************************************************/
void CCluster::K_means()
{
//	int		distype;//�������ʽ(ŷ�ϡ�����...)
	int		times=10000;//max loop number
	int		i,j;

	bool change=true;//�˳���־��falseʱ��ʾ��Ʒ��������ٱ仯����ֹ����
	int counter=0;//��¼��ǰ�Ѿ�ѭ���Ĵ���
	double distance;//�������ĵľ���
	distance=MAX;
	int index;  //////////////////
	
	m_center = new Center[centernum];	//std::auto_ptr<Center>(new Center[centernum]);  //����centernum����������
	
	for ( i=0;i<patternnum;i++)
	{
		m_pattern[i].distance = MAX;  //��ʼ������������������ĵľ���ΪMAX
		m_pattern[i].category = -1;  //��ʼ�������������ĺ�Ϊ��1
	}

	for ( i=0;i<centernum;i++)//��ʼ����ǰcenternum������Ϊcenternum����������
	{
		m_pattern[i].category=i;
		m_pattern[i].distance=0;

		m_center[i].feature = new double[N];
		for ( j=0;j<N;j++)
			m_center[i].feature[j]=m_pattern[i].feature[j];

		m_center[i].index=i;
	}

	while (change && counter<times)
	{
		counter++;
		change=false;

		for(i=0;i<patternnum;i++)//��������Ʒ���¹���
		{
			//�����i��ģʽ�������������ĵ���С���룬
			index=-1;  ///////////////////////////////////////////////////////////
			distance=MAX;
            
			for (int j=0;j<centernum;j++)      //��ÿ����������
				if (distance>GetDistance(m_pattern[i],m_center[j]))
				{
					distance=GetDistance(m_pattern[i],m_center[j]);
					index=j;//�ҵ���С����,�ǵ���index���������ĵľ���
				}

			
			//�Ƚ�ԭ���ĺ��������ĺ�
			//�����ͬ������������ģ����Ķ�change���
			if (m_pattern[i].category!=m_center[index].index)//������ԭ��
			{
				m_pattern[i].category=m_center[index].index;//��������
				change=true;   
			}
		}    //��ÿ�����������¾������

		for (int j=0;j<centernum;j++)      //��ÿ����������
			CalCenter(&m_center[j]);//�������������� CalCenter(&m_center[j]);
	}//end of while
	SetCenter();

	//ɾ����������ָ��
	for (i=0;i<centernum;i++)
	{
		delete []m_center[i].feature;
	}
	delete []m_center;
}

/************************************************************
*��������		K_means()
*����			void
*����ֵ		    void
*��������		����K��ֵ�㷨��ȫ����Ʒ���з���(����pattern���顢������Ŀ������ά����������Ŀ���Ѹ�ֵ���)
************************************************************/
void CCluster::K_means(KResult* kresult)
{
	//	int		distype;//�������ʽ(ŷ�ϡ�����...)
	int		times = 10000;//max loop number
	int		i, j;

	bool change = true;//�˳���־��falseʱ��ʾ��Ʒ��������ٱ仯����ֹ����
	int counter = 0;//��¼��ǰ�Ѿ�ѭ���Ĵ���
	double distance;//�������ĵľ���
	distance = MAX;
	int index;  //////////////////

	m_center = new Center[centernum];  //����centernum����������

	for (i = 0; i<patternnum; i++)
	{
		m_pattern[i].distance = MAX;  //��ʼ������������������ĵľ���ΪMAX
		m_pattern[i].category = -1;  //��ʼ�������������ĺ�Ϊ��1
	}

	for (i = 0; i<centernum; i++)//��ʼ����ǰcenternum������Ϊcenternum����������
	{
		m_pattern[i].category = i;
		m_pattern[i].distance = 0;

		m_center[i].feature = new double[N];
		for (j = 0; j<N; j++)
			m_center[i].feature[j] = m_pattern[i].feature[j];

		m_center[i].index = i;
	}

	while (change && counter<times)
	{
		counter++;
		change = false;

		for (i = 0; i<patternnum; i++)//��������Ʒ���¹���
		{
			//�����i��ģʽ�������������ĵ���С���룬
			index = -1;  ///////////////////////////////////////////////////////////
			distance = MAX;

			for (int j = 0; j<centernum; j++)      //��ÿ����������
				if (distance>GetDistance(m_pattern[i], m_center[j]))
				{
					distance = GetDistance(m_pattern[i], m_center[j]);
				index = j;//�ҵ���С����,�ǵ���index���������ĵľ���
				}


			//�Ƚ�ԭ���ĺ��������ĺ�
			//�����ͬ������������ģ����Ķ�change���
			if (m_pattern[i].category != m_center[index].index)//������ԭ��
			{
				m_pattern[i].category = m_center[index].index;//��������
				change = true;
			}
		}    //��ÿ�����������¾������

		for (int j = 0; j<centernum; j++)      //��ÿ����������
			CalCenter(&m_center[j]);//��������������
	}//end of while 

	SetCenter(kresult);
	//ɾ����������ָ��
	for (i=0;i<centernum;i++)
	{
		delete []m_center[i].feature;
	}
	delete []m_center;
}

/***********************************************************
*��������		InitData(vector<double> data)
*����			data:vector����
*����ֵ			void
*��������		��ȡ�ӿ��ṩ��vector<double>����
************************************************************/
void CCluster::InitData(vector<double>& data)
{
	this->data_ = data;
}

/***********************************************************
*��������		GetResult()
*����			
*����ֵ			Result�ṹ��
*��������		��ȡ�������ṹ��
************************************************************/
vector<CCluster::Result*> CCluster::GetResult() const
{
	return result_;
}

/***********************************************************
*��������		K_meansPort(int patternNum,int N,int K,KResult* kresult)
*����			patternNum:������ N:ָ������ʸ����ά�� K:������
*����ֵ			void
*��������		�ṩ�����K-means����ӿ�(�ṩ����pattern���顢������Ŀ������ά����Ϊ��ֵ����)
*����			Alxor
************************************************************/
void CCluster::startAnalysis(int patternNum, int N, int K, KResult* kresult)
{
	int i, j, id, count;
	string str, word;
	double tf_idf = 0.0;
	int line_num = 0;
	CCluster m_cluster;

	try
	{
		//cout << "K_means Start ..." << endl;
		m_cluster.patternnum = patternNum;
		m_cluster.N = N;
		m_cluster.centernum = K;
		count = data_.size();

		//�½�m_cluster.patternnum++��ģʽ��ÿ��ģʽ����m_cluster.Nά��
		m_cluster.m_pattern = new CCluster::Pattern[m_cluster.patternnum];
		for (i = 0; i < m_cluster.patternnum; i++)
			m_cluster.m_pattern[i].feature = new double[m_cluster.N];

		//����ά�������뵽ģʽ������
		for (i = 0; i < count; i++)
		{
			id = i / m_cluster.N;
			//cout << data_[i] << endl;
			m_cluster.m_pattern[id].index = id;
			j = i % m_cluster.N;
			m_cluster.m_pattern[id].feature[j] = data_[i];
			cout << m_cluster.m_pattern[id].feature[j] << " ";
		}
		cout << endl;
		Normalization(m_cluster);
		m_cluster.K_means(kresult);//K-means
		for (i = 0; i < m_cluster.patternnum; i++)
		{
			Result* current = new Result;//std::auto_ptr<Result> current;
			//std::auto_ptr<Result> current = std::auto_ptr<Result>(new Result);
			current->id = m_cluster.m_pattern[i].index;
			current->category = m_cluster.m_pattern[i].category;
			if (m_cluster.N < FIELDMAX)
			{
				for (j = 0; j < m_cluster.N; j++)
				{
					current->feature[j] = m_cluster.m_pattern[i].feature[j];
				}
			}
			result_.push_back(current);
			//cout << m_cluster.m_pattern[i].category << endl;
			//delete current;
		}
		//cout << "K_means End ..." << endl;
	}
	catch (exception* ex)
	{
		throw ex;
	}
	//cin.get();
}

/***********************************************************
*��������		K_meansPort(int patternNum,int N,int K)
*����			patternNum:������ N:ָ������ʸ����ά�� K:������
*����ֵ			void
*��������		�ṩ�����K-means����ӿ�(�ṩ����pattern���顢������Ŀ������ά����Ϊ��ֵ����)
*����			Alxor
************************************************************/
void CCluster::startAnalysis(int patternNum, int N, int K)
{
	int i, j, id, count;
	string str, word;
	double tf_idf = 0.0;
	int line_num = 0;
	CCluster m_cluster;

	try
	{
		//cout << "K_means Start ..." << endl;
		m_cluster.patternnum = patternNum;
		m_cluster.N = N;
		m_cluster.centernum = K;
		count = data_.size();

		//�½�m_cluster.patternnum++��ģʽ��ÿ��ģʽ����m_cluster.Nά��
		m_cluster.m_pattern = new CCluster::Pattern[m_cluster.patternnum];
		for (i = 0; i < m_cluster.patternnum; i++)
			m_cluster.m_pattern[i].feature = new double[m_cluster.N];

		//����ά�������뵽ģʽ������
		for (i = 0; i < count; i++)
		{
			id = i / m_cluster.N;
			//cout << data_[i] << endl;
			m_cluster.m_pattern[id].index = id;
			j = i % m_cluster.N;
			m_cluster.m_pattern[id].feature[j] = data_[i];
			cout << m_cluster.m_pattern[id].feature[j] << "\t";
		}
		cout << endl;
		Normalization(m_cluster);			//������һ������
		m_cluster.K_means();//K-means
		for (i = 0; i < m_cluster.patternnum; i++)
		{
			Result* current = new Result;//std::auto_ptr<Result> current; // = new Result;
			current->id = m_cluster.m_pattern[i].index;
			current->category = m_cluster.m_pattern[i].category;
			if (m_cluster.N < FIELDMAX)
			{
				for (j = 0; j < m_cluster.N; j++)
				{
					current->feature[j] = m_cluster.m_pattern[i].feature[j];
				}
			}
			result_.push_back(current);
			//cout << m_cluster.m_pattern[i].category << endl;
			delete current;
		}
		//cout << "K_means End ..." << endl;
	}
	catch (exception* ex)
	{
		throw ex;
	}
	//cin.get();
}

//���þ�������
void CCluster::SetCenter(KResult* pcenter)
{
	try
	{
		for (int c = 0; c < centernum; c++)
		{
			pcenter[c].feature_ = new double[N];
			for (int n = 0; n < N; n++)
			{
				pcenter[c].feature_[n] = this->m_center[c].feature[n];
			}
			pcenter[c].index_ = this->m_center[c].index;
			pcenter[c].patternnum_ = this->m_center[c].patternnum;
		}
	}
	catch (exception* ex)
	{
		throw ex;
	}
}

//���þ�������
void CCluster::SetCenter()
{
	try
	{
		for (int c = 0; c < centernum; c++)
		{
			this->pcenter_[c].feature_ = new double[N];
			for (int n = 0; n < N; n++)
			{
				this->pcenter_[c].feature_[n] = this->m_center[c].feature[n];
			}
			this->pcenter_[c].index_ = this->m_center[c].index;
			this->pcenter_[c].patternnum_ = this->m_center[c].patternnum;
		}
	}
	catch (exception* ex)
	{
		throw ex;
	}
}

//�������ݹ�һ�� Xi = 
void CCluster::Normalization(CCluster& m_cluster)
{

	for (int j = 0; j < m_cluster.patternnum; j++)
	{
		double sum = 0.0;
		for (int i = 0; i < m_cluster.N; i++)
		{
			sum += abs(m_cluster.m_pattern[j].feature[i]);
		}
		
		double current;
		try
		{
			for (int n = 0; n < m_cluster.N; n++)
			{
				current = abs(m_cluster.m_pattern[j].feature[n]);
				if (sum != 0)
				{
					m_cluster.m_pattern[j].feature[n] = (double)(current / sum);
				}
				else
				{
					m_cluster.m_pattern[j].feature[n] = 0.0;
				}
			}
		}
		catch (double)
		{
			throw -1.0;
		}
	}

}