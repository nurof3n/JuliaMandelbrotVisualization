#include "CustomExcept.h"

CustomExcept::CustomExcept( const std::string& file, int line, const std::string& s )
	:
	buffer( s ),
	_line( line ),
	_file( file ) {}

const char* CustomExcept::GetType() const {
	return "Custom Exception";
}
// override function with custom message
const char* CustomExcept::what() const {
	std::ostringstream output;
	output << "[File]: " << _file << "\n[Line]: " << _line << "\n[Exception]: " << buffer;
	buffer = output.str();
	return buffer.c_str();
}
