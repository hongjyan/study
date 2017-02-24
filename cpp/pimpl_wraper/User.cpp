#include <iostream>
#include "NamespaceProxy.hpp"
#include "NamespaceProxyImpl.hpp"

int main()
{
    NamespaceProxy nsp;
    std::cout << NamespaceProxyImpl::getNamespaceProxyName(nsp) << std::endl;    
    NamespaceProxyImpl::setNamespaceProxyName(nsp, "xiaoming");
    std::cout << NamespaceProxyImpl::getNamespaceProxyName(nsp) << std::endl;    
    NamespaceProxyImpl::setNamespaceProxyName(nsp, "xiaoli");
    std::cout << NamespaceProxyImpl::getNamespaceProxyName(nsp) << std::endl;    
}
