#include "NamespaceProxy.hpp"
#include "NamespaceProxyImpl.hpp"

struct NamespaceProxy::Impl : public NamespaceProxyImpl
{
    friend NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp)
    {
        return Impl::toNamespaceProxyImpl(nsp);
    }
private:
    static NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp)
    {
        return *nsp.Impl_;
    }
};

NamespaceProxy::NamespaceProxy() : Impl_(new Impl()) {}
NamespaceProxy::~NamespaceProxy() {}


NamespaceProxyImpl& toNamespaceProxyImpl(NamespaceProxy& nsp);

// 获取namespaceProxy里的impl_
// 由于NamespaceProxy.impl_是private的， 所以不能直接nsp.impl_
// 又由于NamespaceProxy::impl::toNamespaceProxyImpl是private的，
// 所以通过中介（全局toNamespaceProxyImpl为NamespaceProxy::impl:toNamespaceProxyImpl的friend）去调用
// 上面的两个private目的是使NamespaceProxy::impl所有内容都对外不可见， 即封装实现细节。
NamespaceProxyImpl& NamespaceProxyImpl::fromNamespaceProxy(NamespaceProxy& nsp)
{
    return toNamespaceProxyImpl(nsp);
}

// 下面的set/get是NamespaceProxyImpl暴露的接口。
// 这两个接口通过调用fromNamespaceProxy获得对它不可见的Impl(虽然是自己的派生类)。
// 然后获取Impl的私有成员name（由于将派生类转换为自身了， 所以得以直接访问name)
std::string NamespaceProxyImpl::getNamespaceProxyName(NamespaceProxy& nsp) {
        return fromNamespaceProxy(nsp).name;
}

void NamespaceProxyImpl::setNamespaceProxyName(NamespaceProxy& nsp, const std::string& name) {
        fromNamespaceProxy(nsp).name = name;
}
