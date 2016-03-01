/**
** �������㷨 BP
** �㷨ԭ��:http://www.cnblogs.com/wentingtu/archive/2012/06/05/2536425.html
** ����ʵ�ֲο�: http://blog.csdn.net/luxiaoxun/article/details/7649945
** BP������ο���http://blog.csdn.net/han6771306/article/details/25476301
**/
#ifndef _ANN_NETWORK_H_
#define _ANN_NETWORK_H_

#include <vector>
#include <map>
#include <cmath>

#define LEARN_RATE 0.9
#define Pi 3.1415926

class DataPoint;

class AnnNetwork
{
public:
	AnnNetwork();
	~AnnNetwork();
	inline int status() const;
	void InitData(std::vector<double>& data, int N);	//��ʼ������
	void InitData(std::vector<DataPoint*> data);
	void InitW(std::vector<double>& w);					//��ʼ��Ȩֵ
	void InitW(double w[], int len);					//��ʼ��Ȩֵ ������ʽ
	int train(const std::vector<DataPoint*> input);		//ѵ������
	void train();
	inline int getLearnTimes() const;

	inline void setLearnTimes(int times);

private:
	std::vector<DataPoint*> x_inputv_;					//��������
	std::vector<DataPoint*> hiden_inputv_;				//��������������
	std::vector<DataPoint*> hiden_outputv_;				//�������������
	std::vector<DataPoint*> y_inputv_;					//�������������
	std::vector<DataPoint*> y_outputv_;					//������������
	std::map<DataPoint*, double> expect_;					//��������
	int learning_times_;
private:
	size_t size_;										//��������
	std::vector<double> w_input_hiden_;					//������������������Ȩֵ
	std::vector<double> w_hiden_output_;				//������������������Ȩֵ
	std::vector<double> rate_hiden_;					//���������Ԫ��ֵ
	std::vector<double> rate_output_;					//��������Ԫ��ֵ

	double _err;										//�����
	int samples_num_;
	int m_status_;

private:
	void normalize(DataPoint **dp, int size);			//��һ������  
	void unnormalize(DataPoint **dp, int size);			//����һ������
	void activatedfunc();								//�����
	void errorfunc();									//����
	void InitW();
	void sampleExpect(std::vector<DataPoint*> inputs, int k);
	void calcHidenLayer(std::vector<DataPoint*> inputs, std::map<DataPoint*, double>& outputs, int k);
	double hiddenFunc(DataPoint* dp, double weight, double threshold);

};

inline int AnnNetwork::getLearnTimes() const
{
	return this->learning_times_;
}

inline void AnnNetwork::setLearnTimes(int times)
{
	this->learning_times_ = times;
}

inline int AnnNetwork::status() const
{
	return this->m_status_;
}
#endif //_ANN_NETWORK_H_
