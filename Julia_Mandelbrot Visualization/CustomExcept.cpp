#include "CustomExcept.h"

const char* CustomExcept::what() const
{
    std::ostringstream output;
    output << "[File]: " << file << "\n[Line]: " << line << "\n[Exception]: " << buffer;
    return output.str().c_str();
}
