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
	CustomExcept( const std::string& file, int line, const std::string& s ) noexcept;
	virtual const char* GetType() const noexcept;
	const char* what() const noexcept override;
private:
	mutable std::string buffer;
	std::string _file;
	int _line;
};

