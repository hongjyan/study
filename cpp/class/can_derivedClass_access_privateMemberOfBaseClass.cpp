#include <iostream>
using namespace std;

class base {
public:
    int get_value() {
	    return value;
    }
private:
    void output_value() {
        cout << value << endl;
    }
    int value;
};

class derived : public base {
public:
    void output_value() {
	    // cout << value << endl;
        // base::output_value();
	    cout << get_value() << endl;
    }
};

int main() {
    derived d;
    d.output_value();
}
