#include <memory>

struct Ctx
{
    Ctx();
    ~Ctx();
private:
    struct impl;
    std::unique_ptr<impl> impl_;
};

