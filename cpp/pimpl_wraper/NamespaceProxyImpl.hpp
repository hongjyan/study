#include <string>

// NamespaceProxy的细节放在对外不可见的NamespaceProxy::Impl里
// 又由于NamespaceProxy::Impl继承自NamespaceProxyImpl，所以可以统一通过NamespaceProxyImpl
// 访问这个不可见的NamespaceProxy::Impl。
// 这种方式实现了NamespaceProxy(接口?数据？）和实现NamespaceProxy::Impl(实现)的分离。 同时让实现(即
// NamespaceProxy::Impl)对外不可见但又可以访问。
// 好处就是： 各NamespaceProxyA， NamespaceProxyB均可以通过继承NamespaceProxy获取这么一套封装name的功能
struct NamespaceProxyImpl
{
    static std::string getNamespaceProxyName(NamespaceProxy& nsp);
    static void setNamespaceProxyName(NamespaceProxy& nsp, const std::string& name);
private:
    static NamespaceProxyImpl& fromNamespaceProxy(NamespaceProxy& nsp);
    std::string name;
};

// 但是相较于上面的实现， 我也没看出下面实现有什么坏处啊。
// struct Namespace : public INamespace
// {
//     std::string getName() { return impl_.getName(); }
//     void setName() { return impl_.setName(); }
// private:
//     struct NamespaceImpl : public INamespace
//     {
//         std::string getName();
//         void setName();
//     private:
//         std::string name;
//     };
//     NamespaceImpl impl_;
// };

