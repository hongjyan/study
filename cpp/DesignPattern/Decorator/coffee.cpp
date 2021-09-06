#include <string>
#include <iostream>


class ICoffee {
public:
    virtual ~ICoffee() = default;
    virtual unsigned getPrice() const = 0;
    virtual std::string getName() const = 0;
    virtual void setPrice(unsigned price) = 0;
};


class Coffee : public ICoffee {
public:
    Coffee(std::string name, unsigned price): name_(name), price_(price) {}

    unsigned getPrice() const { return price_; }
    std::string getName() const { return name_; }

    void setPrice(unsigned price) { price_ = price; }
private:
    const std::string name_;
    unsigned price_;
};


/*
class CondimentDecorator: public ICoffee {
public:
    unsigned getPrice() { return 0; }
    virtual ~CondimentDecorator() = default;
};

class CoffeeDecorator: public CondimentDecorator {
public:
    CoffeeDecorator(ICoffee coffee, unsigned price=5): coffee_(coffee), price_(price) {};

    unsigned getPrice() { return coffee_.getPrice() + price_; }
private:
    ICoffee coffee_;
    unsigned price_;
    const std::string name_ = "CoffeeDecorator";
};
*/

class CoffeeDecorator: public ICoffee {
public:
    CoffeeDecorator(ICoffee* coffee): coffee_(coffee) {};

    unsigned getPrice() const { return coffee_->getPrice(); }
    std::string getName() const { return " " + coffee_->getName(); }
    //void setPrice(unsigned price) {}  //注释掉这一行使本类也是个虚类。

private:
    //类型是指针而非引用的原因是： 在main函数里，一层层的CoffeeDecorator/Coffee可以统一用ICoffee*类型
    //否则， 使用引用的话， 创建不同类型的CoffeeDecorator/Coffee时， 需要声明不同类型，看起来没指针好看
    ICoffee* coffee_; 
};

class CoffeeDecoratedWithMilk: public CoffeeDecorator {
public:
    CoffeeDecoratedWithMilk(ICoffee* coffee): CoffeeDecorator(coffee) {}
    unsigned getPrice() const { return CoffeeDecorator::getPrice() + price_; }
    std::string getName() const { return name_ + CoffeeDecorator::getName(); }
    void setPrice(unsigned price) { price_ = price; }
private:
    std::string name_{"milk"};
    unsigned price_{2};
};


class CoffeeDecoratedWithWhip: public CoffeeDecorator {
public:
    CoffeeDecoratedWithWhip(ICoffee* coffee): CoffeeDecorator(coffee) {}
    unsigned getPrice() const { return CoffeeDecorator::getPrice() + price_; }
    std::string getName() const { return  name_ + CoffeeDecorator::getName(); }
    void setPrice(unsigned price) { price_ = price; }
private:
    std::string name_{"whip"};
    unsigned price_{1};
};

int main() {
    ICoffee* coffee = new Coffee("standard Coffee", 12);
    ICoffee* coffeePlusMilk = new CoffeeDecoratedWithMilk(coffee);
    ICoffee* coffeePlusMilkPlusWhip = new CoffeeDecoratedWithWhip(coffeePlusMilk);
    std::cout << coffeePlusMilkPlusWhip->getPrice() << std::endl;
    std::cout << coffeePlusMilkPlusWhip->getName() << std::endl;
    delete coffee;
    delete coffeePlusMilk;
    delete coffeePlusMilkPlusWhip;
    return 0;
}

