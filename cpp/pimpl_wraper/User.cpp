#include <iostream>
#include "Context.hpp"
#include "CtxImpl.hpp"

int main()
{
    Ctx ctx;
    CtxImpl::setCtxName(ctx, "xiaoming");
    std::cout << CtxImpl::getCtxName(ctx) << std::endl;    
    CtxImpl::setCtxName(ctx, "xiaoli");
    std::cout << CtxImpl::getCtxName(ctx) << std::endl;    
}
