#ifndef _CMSEXCEPTION_H_
#define __CMSEXCEPTION_H_

#include <exception>
#include <string>

class CMSException : public std::exception
{
public:
	explicit CMSException(const char* message) : message_(message)
	{
		FillStackTrace();
	}
	virtual ~CMSException() throw()									//���׳��쳣
	{

	}
	virtual const char* what() throw();
	const char* StackTrace() const throw();
private:
	void FillStackTrace();
	std::string message_;
	std::string stackTrace_;
};

class SyntaxError : public CMSException
{
public:
	explicit SyntaxError(const char* message) :CMSException(message)
	{

	}

	virtual ~SyntaxError() throw()
	{

	}
};

#endif // _EXCEPTION_H_