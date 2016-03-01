/**
 * alxor
 */
#ifndef _KMEDOIDS_H_
#define _KMEDOIDS_H_

#include "K-Result.h"
using namespace std;

class MCluster
{
public:
	struct Pattern					//�����ṹ
	{
		int			index;			//�������
		int			category;		//������ģ���������
		double		*feature;		//����ֵ
		double		distance;		// ��Ʒ�������ĵľ���
	};

	struct Medoids					//�������Ľṹ
	{
		int 		index;			//���ı��
		double		*feature;		//��������ֵ
		int 		patternnum;		//�þ������İ�������Ʒ��Ŀ
	};
public:
	MCluster();
	~MCluster();
private:
	int N;
	Pattern *m_pattern;
	Medoids *m_mediods;
};


#endif // _KMEDOIDS_H_
