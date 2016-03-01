/**
 * alxor
 */
#ifndef _DISTANCE_H_
#define _DISTANCE_H_
#include <vector>
#include <boost\shared_array.hpp>
#include <boost\ptr_container\ptr_vector.hpp>

/**
*	���뺯��
**/
class distance
{
public:
	distance() :n_(0)
	{
		
	}
	distance(double x[], double y[], int n);
	~distance()
	{
		delete x_;
		delete y_;
	}
public:
	static double getEuclidean(double x[], double y[], int n);			//ŷʽ����
	static double getManhattan(double x[], double y[], int n);			//����ֵ����
	static double getChebyshev(double x[], double y[], int n);			//���Ͼ���
	static double getMinkowski(double x[], double y[], int n, int m);	//���Ͼ���
	static double getCamberra(double x[], double y[], int n);			//Camberra����
	static double getEuclidean2(double *x, double *y, int n);			//ŷʽ���� ָ�����
	static double getEuclidean2(boost::ptr_vector<double>& v1, boost::ptr_vector<double>& v2);
	static double getEuclidean2(std::vector<double>& v1, std::vector<double>& v2);
	double getEuclidean();			//ŷʽ����
	double getManhattan();			//����ֵ����
	double getChebyshev();			//���Ͼ���
	double getMinkowski(int m);	//���Ͼ���
	double getCamberra();			//Camberra����
private:
	double* x_;			//X����
	double* y_;			//Y����
	int n_;
};

#endif // _DISTANCE_H_
