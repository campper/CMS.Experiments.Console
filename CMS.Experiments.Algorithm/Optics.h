/**
 * alxor
 */
#ifndef _OPTICS_H_
#define _OPTICS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <exception>
#include <cassert>

#include "distance.h"

class DataPoint;
//OPTICS cluster aglorithm Based on density 
class Optics
{
private:
	/*
	�������ܣ�
	��ڲ���:
	dataPoint:���ݶ���
	dataPoints:���ݶ��󼯺�
	radius: epsilon
	ObjectNum: minPts
	&tmpList: �������������ݶ��� push_back��tmpList
	���ڲ�����
	*/
	void isKeyAndReturnObjects(DataPoint* dataPoint, std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum, std::vector<DataPoint*>& tmpList);

	bool isKeyObjects(DataPoint& dataPoint, std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum);	//Is core object

	double getDistance(DataPoint& dp1, DataPoint& dp2);
	int isContainedInList(DataPoint& dp, std::vector<DataPoint*>& dpList) const;
																			//���������ܶȾ������,���ⲿ����
	int compare_byclusterId(const DataPoint& dp1, const DataPoint& dp2);						//list���������

public:
	void InitData(std::vector<double>& data, int N);											//��ʼ������
	void InitData(std::vector<DataPoint*> data);
	void startAnalysis(std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum);		//���������ܶȾ������
	void startAnalysis(double, int);															//���������ܶȾ������,�ṩ����뾶�����ݶ�����Ŀ
	void startAnalysis();
	void displayDataPoints() const;
	std::vector<DataPoint*> getSeedList() const;													//��ȡ���������Ĵؼ��ϣ����������
	void setParameters(int epsilon, int minPts);
	int getMinPts() const;
	void setMinPts(int);
	int getEpsilon() const;
	void setEpsilon(int);
	void Normalization(DataPoint& m_datapoint);
public:
	Optics() :datanum_(0), epsilon_(0),minPts_(0), dataSets_(NULL), arrivableObjects_(NULL)
	{

	}

	~Optics()
	{

	}
private:
	std::vector<DataPoint*> dataSets_;			//ԭʼ���ݼ���
	std::vector<DataPoint*> seedList_;			//���ľ������
	std::vector<DataPoint*> arrivableObjects_;	//�ɴ�������
	double epsilon_;							//���ݰ뾶 �� 
	int minPts_;								// minPts
	int datanum_;								// ������
	std::ofstream log_;							// log
};


#endif // _OPTICS_H_
