#include "NamespaceProxy.hpp"
#include "NamespaceProxyImpl.hpp"

struct NamespaceProxy::impl : public NamespaceProxyImpl
{
    static NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp)
    {
        return *nsp.impl_;
    }
    friend NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp)
    {
        return impl::toNamespaceProxyImpl(nsp);
    }
};

NamespaceProxy::NamespaceProxy() : impl_(new impl()) {}
NamespaceProxy::~NamespaceProxy() {}


NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp);

NamespaceProxyImpl& NamespaceProxyImpl::fromNamespaceProxy(NamespaceProxy& nsp)
{
    return toNamespaceProxyImpl(nsp);
}

std::string NamespaceProxyImpl::getNamespaceProxyName(NamespaceProxy& nsp) {
        return fromNamespaceProxy(nsp).name;
}

void NamespaceProxyImpl::setNamespaceProxyName(NamespaceProxy& nsp, const std::string& name) {
        fromNamespaceProxy(nsp).name = name;
}
