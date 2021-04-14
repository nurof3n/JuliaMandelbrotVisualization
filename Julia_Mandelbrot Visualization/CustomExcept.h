#pragma once
#include <exception>
#include <string>
#include <sstream>

// macro to insert the filename and the code line where the exception originated from
#define EXCEPT(s) (CustomExcept(__FILE__, __LINE__, s))

// custom exception class derived from the standard exception class
class CustomExcept : public std::exception {
public:
	CustomExcept() = delete;
	CustomExcept( const std::string& file, int line, const std::string& s );
	virtual const char* getType() const;
	const char* what() const override;
private:
	mutable std::string buffer;
	std::string _file;
	int _line;
};

