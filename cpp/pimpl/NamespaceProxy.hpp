
#ifndef NAMESPACEPROXY_HPP_
#define NAMESPACEPROXY_HPP_

#include <memory>
#include <string>

class NamespaceProxy
{
public:
    // It's important to have this declaration public.
    // This allows simple access without inline friend and inheritance.
    //class impl;

    NamespaceProxy();
    ~NamespaceProxy();

    std::string getNamespace();

    void setNamespace(const std::string&);

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

#endif // NAMESPACEPROXY_HPP_
