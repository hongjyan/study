#ifndef __NAMESPACEPROXY_HPP__
#define __NAMESPACEPROXY_HPP__
#include <string>

class NamespaceProxy {
public:
    NamespaceProxy(const std::string& nsp);
    std::string nsp_;
};

#endif
