#pragma once

#include <exception>
#include <sstream>
#include <string>

// Inserts the filename and the code line where the exception originated from
#define EXCEPT(s) (CustomExcept(__FILE__, __LINE__, s))

/**
 * @brief Custom exception class
 */
class CustomExcept : public std::exception {
public:
    CustomExcept() = delete;
    CustomExcept(const std::string& file, int line, const std::string& s)
        : buffer(s), line(line), file(file)
    {
    }

    /**
     * @brief Outputs the file name and line where the exception occured, together with the message
     * thrown
     *
     * @return message of the exception
     */
    const char* what() const override;

private:
    mutable std::string buffer;  // holds the message thrown
    std::string         file;    // holds the file name
    int                 line;    // holds the line
};
