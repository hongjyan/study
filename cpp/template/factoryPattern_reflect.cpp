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

template<typename T, typename ...Targs>
class DynamicCreator
{
public:
    struct Register
    {
        Register()
        {
            std::cout << "DynamicCreator.Register construct" << std::endl;
            char* szDemangleName = nullptr;
            std::string strTypeName;
#ifdef __GNUC__
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
#else
            //in this format?:     szDemangleName =  typeid(T).name();
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
#endif
            if (nullptr != szDemangleName)
            {
                strTypeName = szDemangleName;
                free(szDemangleName);
            }

            ActorFactory<Targs...>::Instance()->Regist(strTypeName, CreateObject);
        }
        inline void do_nothing()const { };
    };


    DynamicCreator()
    {
        std::cout << "DynamicCreator construct" << std::endl;
        //构造函数里使用static变量， 促使static变量生成。 否则static变量不会被生成，那么register的构造函数不会被执行，
        // 即ActorFactory<Targs...>::Instance()->Regist不会被调用
        m_oRegister.do_nothing(); 
    }
    virtual ~DynamicCreator(){
       m_oRegister.do_nothing();
    };
    

    static T* CreateObject(Targs&&... args)
    {
        std::cout << "static Actor* DynamicCreator::CreateObject(Targs... args)" << std::endl;
        return new T(std::forward<Targs>(args)...);
    }

    virtual void Say()
    {
        std::cout << "DynamicCreator say" << std::endl;
    }
    static Register m_oRegister;
};

template<typename T, typename ...Targs>
typename DynamicCreator<T, Targs...>::Register DynamicCreator<T, Targs...>::m_oRegister;


// 通过继承:Cmd: DynamicCreate<类名>， 确定Dynamic::Register<Cmd>， 
// 从而调用Dynamic::Register<Cmd>的构造函数里的ActorFactory<Targs...>::Instance()->Regist(strTypeName, CreateObject);
// 从而注册Cmd* DynamicCreator<Cmd>::createObject()。
class Cmd: public Actor, public DynamicCreator<Cmd>
{
public:
    Cmd(){std::cout << "Create Cmd " << std::endl;}
    virtual void Say()
    {
        std::cout << "I am Cmd" << std::endl;
    }
};

class Procedure: public Actor, public DynamicCreator<Procedure>
{
public:
    Procedure(){std::cout << "Create Procedure " << std::endl;}
    virtual void Say()
    {
        std::cout << "I am Procedure" << std::endl;
    }
};

class Step: public Actor, DynamicCreator<Step>, DynamicCreator<Step, std::string, int>
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
