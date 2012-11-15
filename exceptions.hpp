#ifndef __EXCEPTIONS_HPP
#define __EXCEPTIONS_HPP

#include <exception>
#include <stdexcept>


namespace ecell4
{

class not_found
    : public std::exception
{
public:

    not_found(std::string const& str)
        : str_(str)
    {
        ;
    }

    virtual ~not_found() throw()
    {
        ;
    }

    virtual const char* what() const throw()
    {
        return str_.c_str();
    }

private:

    std::string str_;
};

class already_exists
    : public std::exception
{
public:

    already_exists(std::string const& str)
        : str_(str)
    {
        ;
    }

    virtual ~already_exists() throw()
    {
        ;
    }

    virtual const char* what() const throw()
    {
        return str_.c_str();
    }

private:

    std::string str_;
};

}

#endif /* __EXCEPTIONS_HPP */
