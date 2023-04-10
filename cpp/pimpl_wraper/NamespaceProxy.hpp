#include <memory>

struct NamespaceProxy
{
    NamespaceProxy();
    ~NamespaceProxy();
    struct Impl;
private:
    std::unique_ptr<Impl> Impl_;
};

