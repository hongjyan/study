#include <string>
#include <cstdio>

struct Ctx
{
    Ctx(){}
    ~Ctx(){}
private:
    struct impl;
    impl* impl_;
};

struct CtxImpl
{
    /// actual implementation, internally (in lib) accessible

    static CtxImpl& fromCtx(Ctx& ctx);
};

struct Ctx::impl : public CtxImpl
{
    static CtxImpl& toCtxImpl(Ctx& ctx)
    {
        printf("1\n");
        return *ctx.impl_;
    }
    
    friend CtxImpl& CtxImpl::fromCtx(Ctx& ctx) {
        return toCtxImpl(ctx);
    }   
};


int main()
{
    Ctx ctx;
    CtxImpl& impl = CtxImpl::fromCtx(ctx);
}
