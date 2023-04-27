#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Cordinates
{
    int x;
    int y;
    static bool isValidOps(const string& s)
    {
        if (s.size() != 2 && s.size() != 3 ) return false;
        if (s[0]!='A' && s[0]!='D' && s[0]!='W' && s[0]!='S') return false;
        if (s[1] <= '0' && s[1] >= '9') return false;
        if (s.size() == 3) return (s[2] >= '0' && s[2] <= '9');
        return true;
    }
};

int main() {
    string str;
    cin >> str;

    vector<string> validOps;
    size_t curr = 0;
    size_t next = 0;
    size_t pos = 0;
    do {
        curr = next;
        pos = (curr == 0) ? curr : curr+1;
        next = str.find_first_of(';', pos);
        auto size = (std::string::npos == next) ? str.size()-pos : next-pos;
        auto sstr = str.substr(pos, size);
        if (Cordinates::isValidOps(sstr)) validOps.push_back(sstr);
    } while (next != std::string::npos);

    Cordinates cord;
    for (auto e : validOps) {
        int num = atoi(e.substr(1).c_str());
        switch (e[0]) {
            case 'A':
                cord.x += -1 * num; break;
            case 'D':
                cord.x += num; break;
            case 'W':
                cord.y += num; break;
            case 'S':
                cord.y += -1*num; break;
            default:
                cout << "throw exception" << endl;
                break;
        }
    }

    cout << cord.x << "," << cord.y << endl;
    return 0;
}
