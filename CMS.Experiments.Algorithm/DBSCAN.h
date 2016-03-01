/**
 * alxor
 */
#ifndef _DBSCAN_H_
#define _DBSCAN_H_

#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

class DataPoint;
//�����ܶȾ����㷨
class DBSCAN
{
private:
	std::vector<DataPoint> dadaSets_;        //���ݼ���
	unsigned int dimNum_;				//ά��
	double radius_;						//�뾶
	unsigned int dataNum_;				//��������
	unsigned int minPTs_;				//������С���ݸ���
	std::vector<double> temp_;				//Add by Alxor
	//fstream& out_;						//Add by Alxor

	double GetDistance(DataPoint& dp1, DataPoint& dp2);						//���뺯��
	void SetArrivalPoints(DataPoint& dp);									//�������ݵ��������б�
	void KeyPointCluster(unsigned long i, unsigned long clusterId);			//�����ݵ������ڵĵ�ִ�о������
public:

	DBSCAN();						//Ĭ�Ϲ��캯��
	~DBSCAN();						//Ĭ����������
	bool Init(char* fileName, double radius, int minPTs);    //��ʼ������ ��Ҫ��д�ӿ�
	void InitData(vector<double>& data, double radius, int minPTs, int N);
	bool DoDBSCANRecursive();            //DBSCAN�ݹ��㷨
	bool WriteToFile(char* fileName);    //��������д���ļ�
	bool WriteToFile(fstream& out);
};


#endif // _DBSCAN_H_

