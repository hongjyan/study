#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <unordered_map>
#include <functional>
#include <cxxabi.h>

namespace neb
{

class Actor
{
public:
    Actor(){std::cout << "Actor construct" << std::endl;}
    virtual ~Actor(){};
    virtual void Say()
    {
        std::cout << "Actor" << std::endl;
    }
};

template<typename ...Targs>
class ActorFactory
{
public:
    //typedef Actor* (*ActorCreateFunction)();
    //std::function< Actor*(Targs...args) > pp;

    static ActorFactory* Instance()
    {
        std::cout << "static ActorFactory* Instance()" << std::endl;
        if (nullptr == m_pActorFactory)
        {
            m_pActorFactory = new ActorFactory();
            std::cout << "new m_pActorFactory  " << m_pActorFactory << std::endl;
        }
	else
	{
           std::cout << "m_pActorFactory already exist " << m_pActorFactory << std::endl;
	}
        return(m_pActorFactory);
    }

    virtual ~ActorFactory(){};

    //Lambda: static std::string ReadTypeName(const char * name)

    //bool Regist(const std::string& strTypeName, ActorCreateFunction pFunc)
    //bool Regist(const std::string& strTypeName, std::function<Actor*()> pFunc)
    bool Regist(const std::string& strTypeName, std::function<Actor*(Targs&&... args)> pFunc)
    {
        std::cout << "bool ActorFactory::Regist(const std::string& strTypeName, std::function<Actor*(Targs... args)> pFunc)" << std::endl;
        if (nullptr == pFunc)
        {
            return(false);
        }
        std::string strRealTypeName = strTypeName;
        //[&strTypeName, &strRealTypeName]{int iPos = strTypeName.rfind(' '); strRealTypeName = std::move(strTypeName.substr(iPos+1, strTypeName.length() - (iPos + 1)));};

        bool bReg = m_mapCreateFunction.insert(std::make_pair(strRealTypeName, pFunc)).second;
        std::cout << "m_mapCreateFunction.size() = " << m_mapCreateFunction.size() << std::endl;
        return(bReg);
    }

    Actor* Create(const std::string& strTypeName, Targs&&... args)
    {
        std::cout << "Actor* ActorFactory::Create(const std::string& strTypeName, Targs... args)" << std::endl;
        auto iter = m_mapCreateFunction.find(strTypeName);
        if (iter == m_mapCreateFunction.end())
        {
            return(nullptr);
        }
        else
        {
            //return(iter->second());
            return(iter->second(std::forward<Targs>(args)...));
        }
    }

private:
    ActorFactory(){std::cout << "ActorFactory construct" << std::endl;};
    static ActorFactory<Targs...>* m_pActorFactory;   
    std::unordered_map<std::string, std::function<Actor*(Targs&&...)> > m_mapCreateFunction;
};

template<typename ...Targs>
ActorFactory<Targs...>* ActorFactory<Targs...>::m_pActorFactory = nullptr;


//此版本相较于外围包装一层dynamicCreator， 注册会被执行2遍原因是：
//regist不仅仅由于static register成员被执行一遍，还由于继承又执行一遍（子类自动执行父类的构造函数）
template<typename T, typename ...Targs>
class Register
{
public:
        Register()
        {
            std::cout << "Register construct" << std::endl;
            char* szDemangleName = nullptr;
            std::string strTypeName;
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
            if (nullptr != szDemangleName)
            {
                strTypeName = szDemangleName;
                free(szDemangleName);
            }

            p_Register->do_nothing(); //必须在构造函数里使用下m_oRegister成员，否则segmentfault。原因未知。

            ActorFactory<Targs...>::Instance()->Regist(strTypeName, CreateObject);
        }

    
    static Register* Instance()
    {
        std::cout << "static Register* Instance()" << std::endl;
        if (nullptr == p_Register)
        {
            p_Register = new Register();
            std::cout << "new Register  " << p_Register << std::endl;
        }
	    else
	    {
           std::cout << "p_register already exist " << p_Register << std::endl;
	    }
        return(p_Register);
    }

    static T* CreateObject(Targs&&... args)
    {
        std::cout << "static Actor* Register::CreateObject(Targs... args)" << std::endl;
        return new T(std::forward<Targs>(args)...);
    }

    inline void do_nothing() const {}

    virtual ~Register(){ p_Register->do_nothing(); };

    static Register* p_Register;
};

template<typename T, typename ...Targs>
Register<T, Targs...>* Register<T, Targs...>::p_Register = Register::Instance();

class Cmd: public Actor, public Register<Cmd>
{
public:
    Cmd(){std::cout << "Create Cmd " << std::endl;}
    virtual void Say()
    {
        std::cout << "I am Cmd" << std::endl;
    }
};

class Procedure: public Actor, public Register<Procedure>
{
public:
    Procedure(){std::cout << "Create Procedure " << std::endl;}
    virtual void Say()
    {
        std::cout << "I am Procedure" << std::endl;
    }
};

class Step: public Actor, Register<Step>, Register<Step, std::string, int>
{
public:
    Step(){std::cout << "Create Step " << std::endl;}
    Step(const std::string& strType, int iSeq){std::cout << "Create Step " << strType << " with seq " << iSeq << std::endl;}
    virtual void Say()
    {
        std::cout << "I am Step" << std::endl;
    }
};

class Worker
{
public:
    template<typename ...Targs>
    Actor* CreateActor(const std::string& strTypeName, Targs&&... args)
    {
        Actor* p = ActorFactory<Targs...>::Instance()->Create(strTypeName, std::forward<Targs>(args)...);
        return(p);
    }
};

}

int main()
{
    //Actor* p1 = ActorFactory<std::string, int>::Instance()->Create(std::string("Cmd"), std::string("neb::Cmd"), 1001);
    //Actor* p3 = ActorFactory<>::Instance()->Create(std::string("Cmd"));
    neb::Worker W;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    neb::Actor* p1 = W.CreateActor(std::string("neb::Cmd"));
    p1->Say();
    //std::cout << abi::__cxa_demangle(typeid(Worker).name(), nullptr, nullptr, nullptr) << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    neb::Actor* p2 = W.CreateActor(std::string("neb::Step"), std::string("neb::Step"), 1002);
    p2->Say();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    neb::Actor* p3 = W.CreateActor(std::string("neb::Procedure"));
    p3->Say();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    neb::Actor* p4 = W.CreateActor(std::string("neb::Step"));
    p4->Say();
    return(0);
}
