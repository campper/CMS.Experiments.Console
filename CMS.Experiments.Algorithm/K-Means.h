// Cluster.h: interface for the CCluster class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_CLUSTER_H__738A144B_4EA4_45B0_87CF_E11C2A1B634C__INCLUDED_)
//#define AFX_CLUSTER_H__738A144B_4EA4_45B0_87CF_E11C2A1B634C__INCLUDED_
/**
 * alxor
 */
#ifndef _KMEANS_H_
#define _KMEANS_H_
#define FIELDMAX 50

#include <vector>

#include <memory>
#include "K-Result.h"
using namespace std;


class CCluster
{
public:

	struct Pattern					//�����ṹ
	{
		int			index;			//�������
		int			category;		//������ģ���������
		double		*feature;		//����ֵ
		double		distance;		// ��Ʒ�������ĵľ���
	};

	struct Center					//�������Ľṹ
	{
		int 		index;			//���ı��
		double		*feature;		//��������ֵ
		int 		patternnum;		//�þ������İ�������Ʒ��Ŀ
	};

	struct Result					// Add by Alxor
	{
		int			id;
		int			category;
		double		feature[FIELDMAX];
		bool operator<(const Result& other)
		{
			return category < other.category;
		}
	};

private:
	int         N;					//����ά��
	int			patternnum;			//��������
	int 		centernum;			//����������Ŀ��Ҳ�������Ŀ
	Pattern* m_pattern;				//std::auto_ptr<Pattern>		m_pattern;			//ָ��������ָ��
	Center* m_center;				//std::auto_ptr<Center>		m_center;			//ָ�����ĵ�ָ��
	std::vector<double> data_;		//����Nά���ݼ�
	std::vector<Result*>		result_;			//���������
	KResult* pcenter_;				// std::auto_ptr<KResult> pcenter_;
public:
	CCluster();
	virtual ~CCluster();
private:
	double GetDistance(CCluster::Pattern  pattern, CCluster::Center center);		//������Ʒ�;������ļ�ľ���
	void CalCenter(CCluster::Center* pcenter);
	//void CalCenter(std::auto_ptr<CCluster::Center>& pcenter);						//��������pcenter������ֵ������������Ʒ�ľ�ֵ��������Ʒ����
	void K_means();																	//k��ֵ�㷨����
	void K_means(KResult* kresult);
	//void K_means(std::auto_ptr<KResult>& kresult);								//k��ֵ�㷨���� ���ر�����
	void Normalization(CCluster& m_cluster);										//�������ݹ�һ������
public:
	void InitData(std::vector<double>& data);									//���ⲿ��ȡ�������������
	std::vector<Result*> GetResult() const;
	void startAnalysis(int patternNum, int N, int K, KResult* kresult);
	//void startAnalysis(int patternNum, int N, int K, std::auto_ptr<KResult>& kresult);			//����ӿ� Add by Alxor ����ָ��й¶����
	void startAnalysis(int patternNum, int N, int K);
	void SetCenter(KResult* pcenter);
	//void SetCenter(std::auto_ptr<KResult>& pcenter);
	void SetCenter();
};


#endif // _KMEANS_H_
