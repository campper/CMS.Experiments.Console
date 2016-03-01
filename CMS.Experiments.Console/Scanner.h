#ifndef _SCANNER_H_
#define _SCANNER_H_
#include<string>

enum EToken
{
	TOKEN_END,			//������ʶ��
	TOKEN_ERROR,		//�����ʶ��
	TOKEN_NUMBER,		//���ֱ�ʶ��
	TOKEN_PLUS,			//�ӷ���ʶ��
	TOKEN_MINUS,		//������ʶ��
	TOKEN_MULTIPLY,		//�˷���ʶ��
	TOKEN_DIVIDE,		//������ʶ��
	TOKEN_LPARENTHESIS,	//�����ű�ʶ��
	TOKEN_RPARENTHESIS,	//�����ű�ʶ��
	TOKEN_IDENTIFIER,	//������ʶ��
	TOKEN_ASSIGN		//��ֵ��ʶ��
};
class Scanner
{
public:
	explicit Scanner(const std::string& buf);
	void Accept();
	double Number() const;
	std::string GetSymbol() const;
	EToken Token() const;
	bool IsEmpty() const;
	bool IsDone() const;
private:
	void SkipWhite();
	std::string buf_;
	unsigned int curPos_;
	EToken token_;
	double number_;
	std::string symbol_;
	bool isEmpty_;

};


#endif //_SCANNER_H_
