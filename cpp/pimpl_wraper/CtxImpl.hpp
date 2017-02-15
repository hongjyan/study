#include <string>

struct CtxImpl
{
    static std::string getCtxName(Ctx& ctx);
    static void setCtxName(Ctx& ctx, const std::string& name);
private:
    static CtxImpl& fromCtx(Ctx& ctx);
    std::string name;
};

