#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

using namespace ::testing;
using namespace std;


//测试返回值无默认构造函数，但EXPECT_CALL不带action的行为。
class Foo {
public:
  Foo(int x) {}
  virtual ~Foo() {}

  inline int sum(int x, int y) {
    return x + y;
  }

  inline Foo clone(int x) {
    return Foo(x);
  }
};

class MockFoo : public Foo {
public:
  MockFoo(int x): Foo(x) {}
  virtual ~MockFoo() {}
  MOCK_METHOD2(sum, int(int, int));
  MOCK_METHOD1(clone, Foo(int));
};


struct gtestPracticeTests : public Test {
  void SetUp() {}

  void TearDown() {}

  MockFoo foo{10};
};

TEST_F(gtestPracticeTests, test1) {
  // EXPECT_CALL(foo, sum(_, _))
  EXPECT_CALL(foo, sum(_, _)).Times(1);
  foo.sum(1, 2);
}

TEST_F(gtestPracticeTests, test2) {
  EXPECT_CALL(foo, clone(_)).Times(1);
  foo.clone(7);
}

TEST_F(gtestPracticeTests, test3) {
  EXPECT_CALL(foo, clone(_)).WillOnce(Return(Foo(7)));
  foo.clone(7);
}

TEST_F(gtestPracticeTests, test4) {
  EXPECT_CALL(foo, clone(_)).WillOnce(Invoke(
    [](int x) { return Foo(7);}
  ));
  foo.clone(7);
}


//leak test
//https://stackoverflow.com/questions/10286514/why-is-googlemock-leaking-my-shared-ptr
struct MyParameter
{
    virtual ~MyParameter() {}
    virtual void myMethod() = 0;
};

struct MyParameterMock : public MyParameter
{
    MOCK_METHOD0(myMethod, void());
    // ~MyParameterMock() { std::cout << __func__ << std::endl; }
};

struct MyClass
{
    virtual ~MyClass() {}
    virtual void myMethod(shared_ptr<MyParameter> p) {}
};

struct MyClassMock : public MyClass
{
    MOCK_METHOD1(myMethod, void(shared_ptr<MyParameter>));
    // ~MyClassMock() { std::cout << __func__ << std::endl; }
};

TEST_F(gtestPracticeTests, leaktest)
{
    shared_ptr<MyClassMock> c = make_shared<MyClassMock>();
    shared_ptr<MyParameterMock> p = make_shared<MyParameterMock>();
    //因为如果EXPECT_CALL是要按顺序执行，那么gmock需要维护EXPECT_CALL的顺序， 而gmock靠holding指向前面EXPECT_CALL的指针来达到,
    //且对EXPECT_CALL检查是发生在mock对象被析构时。 故此时就死循环了， 析构要等share_ptr计数为0， 而由于in order检查又需要hold share_ptr，
    //造成mock对象不能被析构。
    //这将导致代码运行到EXPECT_CALL(*p, myMethod());时，仍然有指针指向前一个EXPECT_CALL， 即EXPECT_CALL(*c, myMethod(Eq(p)));
    //这将导致p.use_count不能从1-->0, 所以p不会被系统销毁。（share_ptr只有use_count为0时才会被销毁）。
    //?? 有指针指向前一个EXPECT_CALL意味着该EXPECT_CALL执行完毕后，它使用的智能指针的计数依然不会减一？
    {
        InSequence dummy;
        auto q = p;
        EXPECT_CALL(*c, myMethod(Eq(p)));
        EXPECT_CALL(*p, myMethod());
    }
    c->myMethod(p);
    p->myMethod();

    //此句话将清除对p的expectation， 其作用将会让p的析构函数被调用，从而释放p指向内存
    EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(p.get()));
}



//测试被mock接口有默认实现， mock函数是否继承该默认实现. A: 并不会
void AaUnixTimeGet(int, int);

struct ImsArchiver {
    void buildIms()
    {
      AaUnixTimeGet(1, 2);
    }
};

struct ccsMock {
  MOCK_METHOD2(AaUnixTimeGet, void(int x, int y));

  ccsMock();

  ~ccsMock();
};
ccsMock* g_ccs;
ccsMock::ccsMock() {
  assert(nullptr == g_ccs);
  g_ccs = this;
  std::cout << __func__ << std::endl;
}
ccsMock::~ccsMock() {
  g_ccs = nullptr;
}

void AaUnixTimeGet(int x, int y) {
  std::cout << __func__ << std::endl;
  g_ccs->AaUnixTimeGet(x, y);
}

struct CcsService {
  CcsService() {}
  virtual void serviceFunc1(int x, int y) {
    std::cout << __func__ << std::endl;
    AaUnixTimeGet(x, y);
  }
  virtual ~CcsService() {}
};
struct CcsServiceMock : public CcsService {
  MOCK_METHOD2(serviceFunc1, void(int, int));
};

TEST_F(gtestPracticeTests, willccsMockBeenCall) {
  CcsServiceMock ccsSvc;
  ccsMock ccs;
  EXPECT_CALL(ccs, AaUnixTimeGet(_, _)).Times(0);
  EXPECT_CALL(ccsSvc, serviceFunc1(_, _)).Times(1);
  ccsSvc.serviceFunc1(1, 2);
  EXPECT_CALL(ccs, AaUnixTimeGet(_, _)).Times(1);
  ImsArchiver imsArchiver;
  imsArchiver.buildIms();
}
