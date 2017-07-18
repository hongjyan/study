#include <memory>

struct NamespaceProxy
{
    NamespaceProxy();
    ~NamespaceProxy();
private:
    struct impl;
    std::unique_ptr<impl> impl_;
};

