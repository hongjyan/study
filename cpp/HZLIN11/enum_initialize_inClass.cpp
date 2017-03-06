#include <iostream>
#include <cstdio>
using namespace std;

enum mediaType {
    WAV,
    AUDIO,
    VIDEO
};

class enum_test {
public:
    enum_test(mediaType t): track(SOLO), type(t) {};

    void output() {
	cout << type << endl;
    }
    
    enum mono {
	SOLO,
	DOUBLE,
	TRIBLE
    };

    mono track;
	
private:
    mediaType type;
};

int main() {
//    enum_test et;
//    et.output();

    enum_test et2(WAV);
    printf("The start number of enum is %d\n", WAV);
    et2.track = enum_test::SOLO;
    et2.output();
}
