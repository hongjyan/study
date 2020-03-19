#include <iostream>
#include <vector>
#include <string>
using namespace std;


//simple class to record only one kind item's count and price, interface is total money of this item.
class Item_base {
public:
    Item_base(int count = 0,  double price = 0.0, string isbn = ""): m_count(count), m_price(price), m_isbn(isbn)  {}
    
    virtual double total_price() {
	return m_count * m_price;
    }

    virtual Item_base* clone() const {
	Item_base *p = new Item_base(*this);
	return p;
    }

    const string& get_isbn() {
	return m_isbn;
    }

    virtual void output() {
	cout << "ISBN:" << m_isbn << ", COUNT:" << m_count << ",PRICE:" << m_price << endl;
	cout << "\tTOTAL_PRICE:" << total_price() << endl;
    }
    
    virtual ~Item_base() {} //need define de-constructor explicitly, otherwise, when polymophsim, for example: "Item_base *p = new Discount_item(...); delete p;" de-ctor of derived class:Discount_item won't be called. 

protected:
    int m_count;
    double m_price;
    string m_isbn;
};

//derived class from Item_base, discount added.
class Discount_item : public Item_base {
public:
    Discount_item(int count = 0, double price = 0.0, string isbn = "", double discount = 1.0): Item_base(count, price, isbn), m_discount(discount) {}
    
    virtual double total_price() {
	return (m_count * m_price * m_discount);
    }

    virtual Discount_item* clone() const {
	Discount_item *p = new Discount_item(*this);
	return p;
    }
    
    virtual void output() {
	cout << "ISBN:" << m_isbn << ", COUNT:" << m_count << ",PRICE:" << m_price << ",DISCOUNT:" << m_discount << endl;
	cout << "\tTOTAL_PRICE:" << total_price() << endl;
    }
private:
    double m_discount;
};


//handler class to wrap pointer to Item_base
class handler {
public:
    handler(const Item_base &item);

    handler(const handler &orig) {
	m_p = orig.m_p;
	p_use = orig.p_use;
	++*p_use;
    }

    handler& operator=(handler &other);

    virtual ~handler() {
	if (0 == --*p_use) { delete m_p; delete p_use; }
    }
   
    const Item_base& operator*() {
	return *m_p;
    }

    Item_base* operator->() {
	return m_p;
    }
    
private:
    Item_base *m_p;
    int *p_use;
};

handler::handler(const Item_base &item) {
    m_p = item.clone();
    p_use = new int(1);
}

handler& handler::operator=(handler &other) {
    ++*other.p_use;

    if (0 == --*p_use) { 
	delete m_p; 
	delete p_use; 
    }

    m_p = other.m_p;
    p_use = other.p_use;
    return *this;
}


//general handler class template including 3 features.
template <typename T>
class General_handler {
public:
    General_handler(): m_pitem(NULL), use(NULL) {}

    General_handler(const T&);

    General_handler(const General_handler&);

    General_handler& operator=(const General_handler&);

    ~General_handler() {
	decr();
    }

    T& operator*();

    T* operator->();
private:
    T* m_pitem;
    int *use;
    void decr() {
	if (0 == --*use) { delete m_pitem; delete use; }
    }
};

template <typename T> 
General_handler<T>::General_handler(const T& item): m_pitem(item.clone() ), use(new int(1) ) {}

template <typename T>
General_handler<T>::General_handler(const General_handler& orig) {
    m_pitem = orig.m_pitem;
    use = orig.use;
    ++*use;
}

template <typename T>
General_handler<T>& General_handler<T>::operator=(const General_handler& other) {
    //++*use; //?
    decr();
    m_pitem = other.m_pitem;
    use = other.use;
    ++*use;
    return *this;
}

template <typename T>
T& General_handler<T>::operator*() {
    return *m_pitem;
}

template <typename T>
T* General_handler<T>::operator->() {
    return m_pitem;
}
//END of General_handler

//user: Sale_item wrap General_handler
class Sale_item {
public:
    Sale_item(const Item_base& item): Item_base_handler(item) {}
    
    Item_base& operator*() {
	return *Item_base_handler;
    }

    Item_base* operator->() {
	return Item_base_handler.operator->();
    }

private:
    General_handler<Item_base> Item_base_handler;
};

int main() {
    vector<handler> basket;
    {
        Item_base item(11, 22.9, "C++");
        Discount_item discount(11, 22.9, "JAVA", 0.8);
        Item_base item2(3, 30, "C");
        Discount_item discount2(23, 24, "PYTHON", 0.7);
        Item_base *p = new Item_base;

        basket.push_back(handler(item) );
        basket.push_back(handler(item2) );
        basket.push_back(handler(discount) );
        basket.push_back(handler(discount2) );
        basket.push_back(handler(*p) );
        delete p;
    }

    typedef vector<handler>::iterator iter;
    double sum = 0;
    for (iter it=basket.begin(); it!=basket.end(); it++) {
	(*it)->output();
	sum += (*it)->total_price();
    }
    cout << "sum is " << sum << endl;        

//  test General_handler
    vector<Sale_item>  basket2;
    {
        Item_base item(11, 22.9, "C++");
        Discount_item discount(11, 22.9, "JAVA", 0.8);
        Item_base item2(3, 30, "C");
        Discount_item discount2(23, 24, "PYTHON", 0.7);
        Sale_item s1(item);
        Sale_item s2(discount);
        Sale_item s3(item2);
        Sale_item s4(discount2);
        Item_base *dp = new Item_base(2, 32, "PERL");
        Sale_item s5(*dp);
        delete dp;

        basket2.push_back(s1);
        basket2.push_back(s2);
        basket2.push_back(s3);
        basket2.push_back(s4);
        basket2.push_back(s5);
    }

    typedef vector<Sale_item>::iterator g_iter;
    double g_sum = 0;
    for (g_iter it = basket2.begin(); it != basket2.end(); it++) {
	(*it)->output();
	g_sum += (*it)->total_price();
    }    
    cout << "g_sum is " << g_sum << endl;
}
    

    
 

    
