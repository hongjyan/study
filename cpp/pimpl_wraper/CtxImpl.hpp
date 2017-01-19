#include <string>

struct CtxImpl
{
    static CtxImpl& fromCtx(Ctx& ctx);
    static std::string getCtxName(Ctx& ctx);
    static void setCtxName(Ctx& ctx, const std::string& name);
private:
    std::string name;
};

