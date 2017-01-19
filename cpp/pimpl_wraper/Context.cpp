#include "Context.hpp"
#include "CtxImpl.hpp"

struct Ctx::impl : public CtxImpl
{
    static CtxImpl& toCtxImpl(Ctx& ctx)
    {
        return *ctx.impl_;
    }
    friend CtxImpl& toCtxImpl(Ctx& ctx)
    {
        return impl::toCtxImpl(ctx);
    }
};

Ctx::Ctx() : impl_(new impl()) {}
Ctx::~Ctx() {}


CtxImpl& toCtxImpl(Ctx& ctx);

CtxImpl& CtxImpl::fromCtx(Ctx& ctx)
{
    return toCtxImpl(ctx);
}

std::string CtxImpl::getCtxName(Ctx& ctx) {
        return fromCtx(ctx).name;
}

void CtxImpl::setCtxName(Ctx& ctx, const std::string& name) {
        fromCtx(ctx).name = name;
}
