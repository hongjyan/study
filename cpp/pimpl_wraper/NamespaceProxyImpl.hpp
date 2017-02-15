#include <string>

struct NamespaceProxyImpl
{
    static std::string getNamespaceProxyName(NamespaceProxy& nsp);
    static void setNamespaceProxyName(NamespaceProxy& nsp, const std::string& name);
private:
    static NamespaceProxyImpl& fromNamespaceProxy(NamespaceProxy& nsp);
    std::string name;
};

