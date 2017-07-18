#include <iostream>
#include <string>
using namespace std;

class mediaProcessing {
public:
    mediaProcessing(string aname = "media"): name(aname) {}
 
    void run() {
	this->startPlay();
    }

    virtual void startPlay() {
	cout << "mediaProcessing" << endl;
	cout << name <<endl;
    }

private:
    string name;
};

class MsrpCommonProcedure : public mediaProcessing {
public:
    void startPlay() {
	cout << "MsrpCommonProcedure" << endl;
    }

private:
    string name;
};

class Msrp : public MsrpCommonProcedure {
public:
    Msrp(string aname = "Ms"): name(aname) {}

    void startPlay() {
	cout << "Msrp" << endl;
	cout << name << endl;
    }
private:
    string name;
};

int main() {
    Msrp M;
    M.run();
}
    
    
