#include <fstream>
#include <iosfwd>
#include <math.h>

#include "DBSCAN.h"
#include "distance.h"

#include "DataPoint.h"

using namespace std;

//��ȡά������
DBSCAN::DBSCAN()
{
}


DBSCAN::~DBSCAN()
{
}

/*
�����������ʼ������
˵�����������ļ������뾶��������С���ݸ�����Ϣд������㷨�࣬��ȡ�ļ�����������Ϣ����д���㷨�����ݼ�����
������
char* fileName;    //�ļ���
double radius;    //�뾶
int minPTs;        //������С���ݸ���
����ֵ�� true;    */
bool DBSCAN::Init(char* fileName, double radius, int minPTs)
{
	this->radius_ = radius;							//���ð뾶
	this->minPTs_ = minPTs;							//����������С���ݸ���
	this->dimNum_ = DIME_NUM;						//��������ά��
	ifstream ifs(fileName);							//���ļ�
	if (!ifs.is_open())								//���ļ��Ѿ����򿪣���������Ϣ
	{
		cout << "Error opening file";				//���������Ϣ
		exit(-1);									//�����˳�
	}

	unsigned long i = 0;							//���ݸ���ͳ��
	while (!ifs.eof())								//���ļ��ж�ȡPOI��Ϣ����POI��Ϣд��POI�б���
	{
		DataPoint tempDP;							//��ʱ���ݵ����
		double tempDimData[DIME_NUM];				//��ʱ���ݵ�ά����Ϣ
		for (int j = 0; j<DIME_NUM; j++)			//���ļ�����ȡÿһά����
		{
			ifs >> tempDimData[j];
			//this->temp_.push_back(tempDimData[j]);
		}
		tempDP.SetDimension(tempDimData);			//��ά����Ϣ�������ݵ������

		//char date[20]="";
		//char time[20]="";
		////double type;    //������Ϣ
		//ifs >> date;
		//ifs >> time;    //������Ϣ����

		tempDP.SetDpId(i);							//�����ݵ����ID����Ϊi
		tempDP.SetVisited(false);					//���ݵ����isVisited����Ϊfalse
		tempDP.SetClusterId(-1);					//����Ĭ�ϴ�IDΪ-1
		dadaSets_.push_back(tempDP);				//������ѹ�����ݼ�������
		i++;										//����+1
	}
	ifs.close();									//�ر��ļ���
	dataNum_ = i;									//�������ݶ��󼯺ϴ�СΪi
	for (unsigned long i = 0; i<dataNum_; i++)
	{
		SetArrivalPoints(dadaSets_[i]);				//�������ݵ������ڶ���
	}
	return true;									//����
}

/***********************************************************
*��������		InitData(vector<double> data,double radius, int minPTs)
*����			data:vector����,radius���ð뾶,minPTs������С���ݸ���
*����ֵ			void
*��������		��ȡ�ӿ��ṩ��vector<double>����
************************************************************/
void DBSCAN::InitData(vector<double>& data, double radius, int minPTs,int N)
{
	this->radius_ = radius;							//���ð뾶
	this->minPTs_ = minPTs;							//����������С���ݸ���
	this->dimNum_ = N;								//��������ά��
	
	unsigned long i = 0;							//���ݸ���ͳ��
	
	//��ʼ������
	double tempDimData[DIME_NUM];
	for (int n = 0; n < DIME_NUM;n++)
	{
		tempDimData[n] = 0.0;
	}
	try
	{
		//��������
		int len = data.size();
		int index;
		for (int n = 0; n < len / N; n++)
		{
			DataPoint tempDP;
			for (int i = 0; i < N; i++)
			{
				index = n*N + i;
				tempDimData[i] = data[index];
			}

			tempDP.SetDimension(tempDimData);			//��ά����Ϣ�������ݵ������
			tempDP.SetDpId(n);							//�����ݵ����ID����Ϊi
			tempDP.SetVisited(false);					//���ݵ����isVisited����Ϊfalse
			tempDP.SetClusterId(-1);					//����Ĭ�ϴ�IDΪ-1
			this->dadaSets_.push_back(tempDP);				//������ѹ�����ݼ�������
		}
	}
	catch (exception* ex)
	{
		throw ex;
	}

	this->dataNum_ = data.size() / N;							//�������ݶ��󼯺ϴ�СΪ data.size() / N
	for (unsigned long i = 0; i<dataNum_; i++)
	{
		SetArrivalPoints(this->dadaSets_[i]);				//�������ݵ������ڶ���
	}
}

/*
���������Ѿ��������㷨��������ݼ���д���ļ�
˵�������Ѿ���������д���ļ�
������
char* fileName;    //Ҫд����ļ���
����ֵ�� true    */
bool DBSCAN::WriteToFile(char* fileName)
{
	ofstream of1(fileName);                                   //��ʼ���ļ������
	for (unsigned long i = 0; i<dataNum_; i++)                //�Դ������ÿ�����ݵ�д���ļ�
	{
		for (int d = 0; d<DIME_NUM; d++)                    //��ά����Ϣд���ļ�
			of1 << dadaSets_[i].GetDimension()[d] << '\t';
		of1 << dadaSets_[i].GetClusterId() << endl;        //��������IDд���ļ�
	}
	of1.close();    //�ر�����ļ���
	return true;    //����
}

// Add by Alxor
bool DBSCAN::WriteToFile(fstream& out)
{
	try
	{
		for (unsigned long i = 0; i < dataNum_; i++)                //�Դ������ÿ�����ݵ�д���ļ�
		{
			for (int d = 0; d < DIME_NUM; d++)                    //��ά����Ϣд���ļ�
				out << dadaSets_[i].GetDimension()[d] << '\t';
			out << endl;
			out << "["<<dadaSets_[i].GetClusterId() <<"]"<< endl;        //��������IDд���ļ�
		}
		out.close();    //�ر�����ļ���
		return true;    //����
	}
	catch (exception* ex)
	{
		out << ex->what() << endl;
		out.close();
		return false;
	}
}

/*
 *�������������ݵ��������б�
 *˵�����������ݵ��������б�
 *������
 *����ֵ�� true;    
 */
void DBSCAN::SetArrivalPoints(DataPoint& dp)
{
	for (unsigned long i = 0; i < dataNum_; i++)                //��ÿ�����ݵ�ִ��
	{
		double distance = GetDistance(dadaSets_[i], dp);    //��ȡ���ض���֮��ľ���
		if (distance <= radius_ && i != dp.GetDpId())        //������С�ڰ뾶�������ض����id��dp��id��ִͬ��
			dp.GetArrivalPoints().push_back(i);            //���ض���idѹ��dp�������б���
	}
	if (dp.GetArrivalPoints().size() >= minPTs_)            //��dp���������ݵ�������> minPTsִ��
	{
		dp.SetKey(true);    //��dp���Ķ����־λ��Ϊtrue
		return;                //����
	}
	dp.SetKey(false);    //���Ǻ��Ķ�����dp���Ķ����־λ��Ϊfalse
}


/*
������ִ�о������
˵����ִ�о������
������
����ֵ�� true;    */
bool DBSCAN::DoDBSCANRecursive()
{
	unsigned long clusterId = 0;							//����id��������ʼ��Ϊ0
	for (unsigned long i = 0; i<dataNum_; i++)				//��ÿһ�����ݵ�ִ��
	{
		DataPoint& dp = dadaSets_[i];						//ȡ����i�����ݵ����
		if (!dp.isVisited() && dp.IsKey())					//������û�����ʹ��������Ǻ��Ķ���ִ��
		{
			dp.SetClusterId(clusterId);						//���øö���������IDΪclusterId
			dp.SetVisited(true);							//���øö����ѱ����ʹ�
			KeyPointCluster(i, clusterId);					//�Ըö��������ڵ���о���
			clusterId++;									//clusterId����1
		}
		//cout << "������\T" << i << endl;
	}
	cout << "������" << clusterId << "��" << endl;			//�㷨��ɺ�����������
	return true;    //����
}

/**
	�����������ݵ������ڵĵ�ִ�о������
	˵�������õݹ�ķ�����������Ⱦ�������
	������
	unsigned long dpID;            //���ݵ�id
	unsigned long clusterId;    //���ݵ�������id
	����ֵ�� void;    
 **/
void DBSCAN::KeyPointCluster(unsigned long dpID, unsigned long clusterId)
{
	DataPoint& srcDp = dadaSets_[dpID];        //��ȡ���ݵ����
	if (!srcDp.IsKey())    return;
	vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //��ȡ���������ڵ�ID�б�
	for (unsigned long i = 0; i<arrvalPoints.size(); i++)
	{
		DataPoint& desDp = dadaSets_[arrvalPoints[i]];    //��ȡ�����ڵ����ݵ�
		if (!desDp.isVisited())                            //���ö���û�б����ʹ�ִ��
		{
			//cout << "���ݵ�\t"<< desDp.GetDpId()<<"����IDΪ\t" <<clusterId << endl;
			desDp.SetClusterId(clusterId);        //���øö��������ص�IDΪclusterId�������ö����������
			desDp.SetVisited(true);                //���øö����ѱ�����
			if (desDp.IsKey())                    //���ö����Ǻ��Ķ���
			{
				KeyPointCluster(desDp.GetDpId(), clusterId);    //�ݹ�ضԸ���������ݵ������ڵĵ�ִ�о������������������ȷ���
			}
		}
	}
}

//�����ݵ�֮�����
/*
��������ȡ�����ݵ�֮�����
˵������ȡ�����ݵ�֮���ŷʽ����
������
DataPoint& dp1;        //���ݵ�1
DataPoint& dp2;        //���ݵ�2
����ֵ�� double;    //����֮��ľ���        */
double DBSCAN::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance = 0;        //��ʼ������Ϊ0
	for (int i = 0; i<DIME_NUM; i++)    //������ÿһά����ִ��
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i], 2);    //����+ÿһά���ƽ��
	}
	return pow(distance, 0.5);        //���������ؾ���
}
