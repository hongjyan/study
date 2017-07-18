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
    friend CtxImpl& toCtxImpl(Ctx& ctx)
    {
        printf("2\n");
        return toCtxImpl(ctx);
    }
};

CtxImpl& toCtxImpl(Ctx& ctx);

CtxImpl& CtxImpl::fromCtx(Ctx& ctx)
{
    printf("3\n");
    return toCtxImpl(ctx);
}


int main()
{
    Ctx ctx;
    CtxImpl& impl = CtxImpl::fromCtx(ctx);
}
