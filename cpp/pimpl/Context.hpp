
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <memory>
#include <string>

class Context
{
public:
    // It's important to have this declaration public.
    // This allows simple access without inline friend and inheritance.
    //class impl;

    Context();
    ~Context();

    std::string getNamespace();

    void setNamespace(const std::string&);

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

#endif // CONTEXT_HPP_
