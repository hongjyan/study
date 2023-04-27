#include <cctype>
#include <ios>
#include <iostream>
#include <bitset>
#include <utility>
#include <vector>
using namespace std;

vector<string> splitStr(const string& str, char delim = '.')
{
    size_t pos = 0;
    size_t delimPos = 0;
    vector<string> ret;
    do {
        delimPos = str.find_first_of(delim, pos);
        auto size = (delimPos == string::npos) ? string::npos : delimPos - pos;
        ret.push_back(str.substr(pos, size));
        pos = delimPos + 1;
    } while (delimPos != string::npos);
    return ret;
}

void removeEmpty(vector<string>& svec)
{
    for (auto it=svec.begin(); it!=svec.end();)
    {
        if (it->empty()) {
            it = svec.erase(it);
        } else {
            ++it;
        }
    }
}

pair<char, bool> getIpInfo(const string& ip)
{
    auto ipFragements = splitStr(ip);
    removeEmpty(ipFragements);

    if (!ipFragements.empty()) {
        int a = strtol(ipFragements[0].c_str(), nullptr, 0);
        if (0 == a || 127 == a) { return make_pair('I', false); }
    }

    char* p;
    bool isNotDigit = false;

    if (4 != ipFragements.size()) return make_pair('F', 0);
    int a = strtol(ipFragements[0].c_str(), &p, 0); isNotDigit |= (*p);
    int b = strtol(ipFragements[1].c_str(), &p, 0); isNotDigit |= (*p);
    int c = strtol(ipFragements[2].c_str(), &p, 0); isNotDigit |= (*p);
    int d = strtol(ipFragements[3].c_str(), &p, 0); isNotDigit |= (*p);
    if (isNotDigit) { return make_pair('F', false); }

    pair<char, bool> ret{'F', false};
    if (a >= 1 && a <= 126
        && b >= 0 && b <= 255
        && c >= 0 && c <= 255
        && d >= 0 && d <= 255)
    {
        ret.first = 'A';
        if (10 == a) ret.second = true;
    }

    if (a >= 128 && a <= 191
        && b >= 0 && b <= 255
        && c >= 0 && c <=255
        && d >=0 && d <= 255)
    {
        ret.first = 'B';
        if (172 == a
            && b >= 16 && b <= 31) { ret.second = true; }
    }

    if (a >= 192 && a <= 223
        && b >= 0 && b <= 255
        && c >= 0 && c <= 255
        && d >= 0 && d <= 255)
    {
        ret.first = 'C';
        if (a == 192 && b == 168) { ret.second = true; }
    }

    if (a >= 224 && a <= 239
        && b >=0 && b <= 255
        && c >= 0 && c <= 255
        && d >= 0 && d <= 255)
    {
        ret.first = 'D';
    }

    if (a >= 240 && a <= 255
        && b >= 0 && b <= 255
        && c >= 0 && c <= 255
        && d >= 0 && d <= 255)
    {
        ret.first = 'E';
    }

    return ret;
}

bool isValidMask(const string& mask)
{
    auto maskFragements = splitStr(mask);
    removeEmpty(maskFragements);
    if (4 != maskFragements.size()) return false;
    char *p;

    vector<int> unique01;
    for (int i=0; i!=4; ++i) {
        auto num = strtol(maskFragements[i].c_str(), &p, 0);
        if (*p) return false;
        for (int j=0; j!=8; ++j) {
            auto val = (0 == (num & (0b10000000 >> j))) ? 0 : 1;
            if (unique01.empty()) {
                unique01.push_back(val);
            } else {
                if (unique01.back() != val) { unique01.push_back(val); }
            }
        }
    }

    return unique01 == vector<int>{1, 0};
}

int main() {
    vector<pair<string, string>> ipMasks;
    string line;
    size_t a, b, c, d, e, f, p;
    a = b = c = d = e = f = p = 0;
    while (std::getline(cin, line)) {
        auto delimPos = line.find_first_of('~');
        auto ip = line.substr(0, delimPos);
        string mask = (string::npos==delimPos) ? "" : line.substr(delimPos+1);
        ipMasks.emplace_back(ip, mask);
    }

    for (auto ipMask : ipMasks) {
        auto ip = ipMask.first; auto mask = ipMask.second;
        auto categoryAndPrivate = getIpInfo(ip);
        if (categoryAndPrivate.first == 'I') { continue; }
        
        if (!isValidMask(mask)) {
            ++f;
        }
        else {
            if (categoryAndPrivate.second) { ++p; }
            switch (categoryAndPrivate.first) {
                case 'A':
                    ++a; break;
                case 'B':
                    ++b; break;
                case 'C':
                    ++c; break;
                case 'D':
                    ++d; break;
                case 'E':
                    ++e; break;
                case 'F':
                    ++f; break;
                default:
                    cout << "throw exception";
                    break;
            }
        }
    }
    cout << a << " " << b << " " << c << " " << d << " " << e << " "
        << f << " " << p << endl;

    return 0;   
}