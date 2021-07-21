#include <iostream>
#include <set>

using namespace std;

class Message
{
public:
    virtual ~Message() {}

    virtual void foo()
    {

    }
};

template<typename Ret> 
struct factory
{
    template<typename... Args> 
    static Message* produce(Args... args )
    {
        return new Ret(args...);
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
};


class MessageA : public Message
{
public:

    MessageA() : A(0)
    {
        std::cout << "MessageA" << std::endl;
    }

    MessageA(int a) : A(a)
    {
        std::cout << "MessageA with int: " << A << std::endl;
    }

    ~MessageA()
    {
    }

    void foo() override
    {
        std::cout << "MessageA: " << "foo " << A  <<  std::endl;
    }
public:
    int A;
};


int main()
{  
    auto p = factory<MessageA>::produce(1);
    p->foo();
    auto p2 = factory<MessageA>::produce();
    p2->foo();
    return 0;
}

