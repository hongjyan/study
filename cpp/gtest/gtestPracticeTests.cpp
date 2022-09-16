#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <vector>

using namespace ::testing;
using namespace std;

struct gtestPracticeTests : public Test {
  void SetUp() {}

  void TearDown() {}
};

/***************************
测试返回值无默认构造函数，但EXPECT_CALL不带action的行为。
***************************/
class Foo {
public:
  Foo(int x) {}
  virtual ~Foo() {}

  virtual inline int sum(int x, int y) {
    return x + y;
  }

  virtual inline Foo clone(int x) {
    return Foo(x);
  }

  inline void nonVitrualfunc() {}  //MockFoo可以不mock某函数
};

class MockFoo : public Foo {
public:
  MockFoo(int x): Foo(x) {}
  ~MockFoo() {}
  MOCK_METHOD2(sum, int(int, int));
  MOCK_METHOD1(clone, Foo(int));
};


TEST_F(gtestPracticeTests, OKWithoutDefineAction_sinceReturnTypeOfSumIsIntWhichWillBeZeroInDefault) {
  MockFoo foo{10};
  // EXPECT_CALL(foo, sum(_, _))
  EXPECT_CALL(foo, sum(_, _)).Times(1);
  foo.sum(1, 2);
}

TEST_F(gtestPracticeTests, NOKSinceFooDoesnotHaveDefaultValue) {
  MockFoo foo{10};
  EXPECT_CALL(foo, clone(_)).Times(1);
  foo.clone(7);
}

TEST_F(gtestPracticeTests, OKWhenSpecifyAction) {
  MockFoo foo{10};
  EXPECT_CALL(foo, clone(_)).WillOnce(Return(Foo(7)));
  foo.clone(7);
}

TEST_F(gtestPracticeTests, OKWhenSpecifyAction2) {
  MockFoo foo{10};
  EXPECT_CALL(foo, clone(_)).WillOnce(Invoke(
    [](int x) { return Foo(7);}
  ));
  foo.clone(7);
}
/***************************
 * 可以看出上面的mock直接mock了具体函数。
 * 但是并不推荐这样。因为
 * 1. 如果mock具体类， 那么被mock函数必须是virtual的， 否则不能mock对象的mock函数不能相当于被mock对象的函数，也不会被调用。 所以可能
 * 需要将非virtual 函数变为virtual
 * 2. 如果这个具体类有了派生类， 那么测试其他派生类时，先前的mock类还能使用吗？
 * A: 如果先前使用派生类或者基类都是通过基类指针或引用，那是可以的。？？那么这样还不如直接抽象为接口
 ****************************/



/***************************
最近的expect_call优先级最高
***************************/
struct MockBar {
  MOCK_METHOD1(func1, void(int));
};

TEST_F(gtestPracticeTests, NotMatchFirstExpectSinceLastExpectHasHigherPriority) {
  MockBar bar;
  //此处期望func1以小于5的参数被调用一次，然后以任意参数被调用一次。
  //但是由于最近的EXPECT_CALL优先级最高， 所以随后的2次bar.func1都匹配到了EXPECT_CALL(bar, func1(lt(5)))
  //导致  EXPECT_CALL(bar, func1(_))无法被匹配。
  EXPECT_CALL(bar, func1(_));
  EXPECT_CALL(bar, func1(Lt(5)));
  bar.func1(1);
  bar.func1(2);
}

TEST_F(gtestPracticeTests, theFirstExpectWillMatch) {
  MockBar bar;
  EXPECT_CALL(bar, func1(_));
  EXPECT_CALL(bar, func1(Lt(5))).RetiresOnSaturation();   //把expect想象为海绵， match想象为浸水。那么如果匹配到了就意味着海绵饱和了， 那么就可以退休了。
  bar.func1(1);
  bar.func1(2);
}



/***************************
 * mock 通过on_call使用fake
***************************/
class Tar {
 public:
  virtual ~Tar() {}
  virtual char DoThis(int n) = 0;
  virtual void DoThat(const char* s, int* p) = 0;
};

class FakeTar : public Tar {
 public:
  char DoThis(int n) override {
    return (n > 0) ? '+' :
           (n < 0) ? '-' : '0';
  }

  void DoThat(const char* s, int* p) override {
    *p = strlen(s);
  }
};

class MockTar : public Tar {
 public:
  // Normal mock method definitions using gMock.
  MOCK_METHOD1(DoThis, char(int));
  MOCK_METHOD2(DoThat, void(const char*, int*));

  // Delegates the default actions of the methods to a FakeTar object.
  // This must be called *before* the custom ON_CALL() statements.
  void DelegateToFake() {
    ON_CALL(*this, DoThis(_)).WillByDefault(Invoke([this](int n) {
      return fake_.DoThis(n);
    }));
    ON_CALL(*this, DoThat(_, _)).WillByDefault(Invoke([this](const char* s, int* p) {
      fake_.DoThat(s, p);
    }));
  }

 private:
  FakeTar fake_;  // Keeps an instance of the fake in the mock.
};

TEST_F(gtestPracticeTests, shouldDelegateSucceed) {
  MockTar tar;
  tar.DelegateToFake();  // Enables the fake for delegation.

  // Put your ON_CALL(tar, ...)s here, if any.

  // No action specified, meaning to use the default action.
  EXPECT_CALL(tar, DoThis(5));
  EXPECT_CALL(tar, DoThat(_, _));

  int n = 0;
  EXPECT_EQ('+', tar.DoThis(5));  // Faketar::DoThis() is invoked.
  tar.DoThat("Hi", &n);  // Faketar::DoThat() is invoked.
  EXPECT_EQ(2, n);
} 



/***************************
leak test
In order EXPECT 会让后面expect通过保留指向前一个expect的指针来维持order
另外测试了对expect的检查是在析构时进行
https://stackoverflow.com/questions/10286514/why-is-googlemock-leaking-my-shared-ptr
***************************/
struct MyParameter
{
    virtual ~MyParameter() {}
    virtual void myMethod() = 0;
};
struct MyParameterMock : public MyParameter
{
    MOCK_METHOD0(myMethod, void());
    ~MyParameterMock() { std::cout << __func__ << std::endl; }
};

struct MyClass
{
    virtual ~MyClass() {}
    virtual void myMethod(shared_ptr<MyParameter> p) {}
};
struct MyClassMock : public MyClass
{
    MOCK_METHOD1(myMethod, void(shared_ptr<MyParameter>));
    ~MyClassMock() { std::cout << __func__ << std::endl; }
};

TEST_F(gtestPracticeTests, leaktest)
{
    shared_ptr<MyClassMock> c = make_shared<MyClassMock>();
    shared_ptr<MyParameterMock> p = make_shared<MyParameterMock>();
    //因为如果EXPECT_CALL是要按顺序执行，那么gmock需要维护EXPECT_CALL的顺序， 而gmock靠holding指向前面EXPECT_CALL的指针来达到,
    //且对EXPECT_CALL检查是发生在mock对象被析构时。 故此时就死循环了， 析构要等share_ptr计数为0， 而由于in order检查又需要hold share_ptr，
    //这将造成mock对象不能被析构。
    //这将导致gmock准备析构p以检查EXPECT_CALL(*p, myMethod())时，仍然有指针指向前一个EXPECT_CALL， 而该EXPECT_CALL(*c, myMethod(Eq(p)))
    //有使用了p， 所以p的引用计数依然不为0，所以导致了p不能被析构
    {
        InSequence dummy;
        EXPECT_CALL(*c, myMethod(Eq(p)));
        EXPECT_CALL(*p, myMethod());
    }
    c->myMethod(p);
    p->myMethod();

    //此句话将清除对p的expectation， 其作用将会让p的析构函数被调用，从而释放p指向内存
    EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(p.get()));
}



/***************************
 * 证明对expectation的检查是在销毁mock对象时
 * https://stackoverflow.com/questions/33125953/how-can-i-tell-googlemock-to-stop-checking-an-expectation-after-the-test-finishe
    Forcing a Verification

    When it's being destoyed, your friendly mock object will automatically verify 
    that all expectations on it have been satisfied, and will generate Google Test failures if not. 
    This is convenient as it leaves you with one less thing to worry about. That is, unless you are
    not sure if your mock object will be destoyed.

    How could it be that your mock object won't eventually be destroyed? Well, it might be created 
    on the heap and owned by the code you are testing. Suppose there's a bug in that code and it 
    doesn't delete the mock object properly - you could end up with a passing test when there's 
    actually a bug.
***************************/
struct MockSubscriber {
  MOCK_METHOD0(func1, void());
};

vector<shared_ptr<MockSubscriber>> g_subscribers;

void subscribe(shared_ptr<MockSubscriber>& p) {
  g_subscribers.push_back(p);
}

void notify() {
  for (auto s : g_subscribers) {
    s->func1();
  }
}

TEST_F(gtestPracticeTests, verifyWhenDtor_failSinceFirstWillBeCallInNextCase)
{
    shared_ptr<MockSubscriber> first(make_shared<MockSubscriber>());
    subscribe(first);
    EXPECT_CALL(*first, func1());
    notify(); 
}

TEST_F(gtestPracticeTests, verifyWhenDtor_shouldSuccess)
{
    shared_ptr<MockSubscriber> second(make_shared<MockSubscriber>());
    subscribe(second);
    EXPECT_CALL(*second, func1());
    notify();
}



/***************************
测试被mock接口有默认实现， mock函数是否继承该默认实现. A: 并不会
***************************/
void AaUnixTimeGet(int, int);

struct ImsArchiver {
    void buildIms()
    {
      AaUnixTimeGet(1, 2);
    }
};

//该类文件里有个本类型的全局指针， 该全局指针指向最近一次创建的本类实例
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

TEST_F(gtestPracticeTests, AaUnixTimeGetOfccsMockWillnotBeCall) {
  CcsServiceMock ccsSvc;
  ccsMock ccs;
  EXPECT_CALL(ccs, AaUnixTimeGet(_, _)).Times(0); 
  EXPECT_CALL(ccsSvc, serviceFunc1(_, _)).Times(1);
  ccsSvc.serviceFunc1(1, 2); //ccsSvc类型为mock类，但是其并不会调用被mock类（ccsService）的对应函数。故于CcsService::serviceFunc1并不会被调用

  EXPECT_CALL(ccs, AaUnixTimeGet(_, _)).Times(1); //AaUnixTimeGet被imsArchiver.buildIms调用
  ImsArchiver imsArchiver;
  imsArchiver.buildIms();
}
