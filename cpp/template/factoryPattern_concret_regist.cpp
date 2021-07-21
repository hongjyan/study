#include <iostream>
#include <set>
#include <string>
#include <functional>
#include <memory>
#include <cxxabi.h>

using namespace std;

class Message
{
public:
    virtual ~Message() {}

    virtual void foo()
    {

    }
};


struct factory
{
    static void register_message(const std::string& key)
    {
        factory::get().set_.emplace(key);
    }


    template<typename Ret, typename... Args>
    static Message* produce(Args... args)
    {
        int status;
        char * demangled = abi::__cxa_demangle(typeid(Ret).name(),0,0,&status);
        std::cout << demangled << std::endl;
        if (set_.find(std::string(demangled)) == set_.end())
            throw std::invalid_argument("the message key is not exist!");

        return new Ret(args...);
    }

    template<typename Ret, typename... Args>
    static std::unique_ptr<Message> produce_unique(Args... args)
    {
        return std::unique_ptr<Message>(produce(args...));
    }

    template<typename Ret, typename... Args>
    static std::shared_ptr<Message> produce_shared(Args... args)
    {
        return std::shared_ptr<Message>(produce(args...));
    }

private:
    factory() {};
    factory(const factory&) = delete;
    factory(factory&&) = delete;

    static factory& get()
    {
        static factory instance;
        return instance;
    }

    static std::set<std::string> set_;
};

std::set<std::string> factory::set_;



class Message1 : public Message
{
public:

    Message1() : A(0)
    {
        std::cout << "message1" << std::endl;
    }

    Message1(int a) : A(a)
    {
        std::cout << "message1 with int: " << A << std::endl;
    }

    ~Message1()
    {
    }

    void foo() override
    {
        std::cout << "message1: " << "foo " << A  <<  std::endl;
    }
public:
    int A;
};


int main()
{

    factory::register_message("Message1");

    auto p = factory::produce<Message1>(1);
    p->foo();

    auto p2 = factory::produce<Message1>();
    p2->foo();

}
