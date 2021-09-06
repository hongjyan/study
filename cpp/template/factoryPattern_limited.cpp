#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <memory>

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
    template<typename T>
    struct register_t
    {
        register_t(const std::string& key)
        {
            factory::get().map_.emplace(key, [] { return new T(); });
        }

        template<typename... Args>
        register_t(const std::string& key, Args... args)
        {
            factory::get().map_.emplace(key, [=]() { return new T(args...); });
        }
    };

    static Message* produce(const std::string& key)
    {
        if (map_.find(key) == map_.end())
            throw std::invalid_argument("the message key is not exist!");

        return map_[key]();
    }

    static std::unique_ptr<Message> produce_unique(const std::string& key)
    {
        return std::unique_ptr<Message>(produce(key));
    }

    static std::shared_ptr<Message> produce_shared(const std::string& key)
    {
        return std::shared_ptr<Message>(produce(key));
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

    static std::map<std::string, std::function<Message*()>> map_;
};

std::map<std::string, std::function<Message*(Args&&...)>> factory::map_;


#define REGISTER_MESSAGE_VNAME(T) reg_msg_##T##_
#define REGISTER_MESSAGE(T, key, ...) static factory::register_t<T> REGISTER_MESSAGE_VNAME(T)(key, ##__VA_ARGS__);
//#define REGISTER_MESSAGE_VNAME(T, key) reg_msg_##T##_##key
//#define REGISTER_MESSAGE(T, key, ...) static factory::register_t<T> REGISTER_MESSAGE_VNAME(T, key)(key, ##__VA_ARGS__);


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

//这个工厂里， 某个类只能注册一次， 且只能注册一次实例， 太不灵活了。
//NREGISTER_MESSAGE(Message1, "message1");
REGISTER_MESSAGE(Message1, "message1_withParameter", 1)


int main()
{
    /*
    Message* p = factory::produce("message1");
    p->foo(); 

    auto p2 = factory::produce_unique("message1");
    p2->foo();
    */

    auto p3 = factory::produce("message1_withParameter");
    p3->foo();

    auto p4 = factory::produce_unique("message1_withParameter");
    p4->foo();
}
