#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

class strVec {
private:
    string *elem;
    string *firstFree;
    string *cap;
    typedef pair<string*, string*> range;
    range allocate_n_copy(string *beg, string *end);
    allocator<std::string> alloc;
    void free();
    void reallocate();

public:
    strVec(): elem(nullptr), firstFree(nullptr), cap(nullptr) {}
    strVec(const strVec&);
    strVec& operator=(const strVec&);
    ~strVec();
    void push_back(string s);
    string *begin() const { return elem; }
    string *end() const { return firstFree; }
    size_t size() const { return firstFree - elem; }
    size_t capcity() const { return cap - elem; }
};

strVec::range strVec::allocate_n_copy(string *beg, string* end) {
    string *start = alloc.allocate(end - beg);
    return {start, uninitialized_copy(beg, end, start)};
}     

strVec::strVec(const strVec& sv) {
    range posPair = allocate_n_copy(sv.begin(), sv.end());
    elem = posPair.first;
    cap = firstFree = posPair.second;
}

void strVec::free() {
    //deallocate in sequence
    if (elem) {  //deallocate nullptr will crash
        for (auto p=elem; p!=firstFree;) {
            alloc.destroy(p++);
        }
        alloc.deallocate(elem, cap-elem);
    }
/*
    if (elem) {  //deallocate nullptr will crash
        for (auto p=firstFree; p!=elem;) {
            alloc.destroy(--p);
        }
        alloc.deallocate(elem, cap-elem);
    }
*/
}
    
strVec &strVec::operator=(const strVec &src) {
    if (this == &src) return *this;
    range rg = allocate_n_copy(src.begin(), src.end());
    free();
    elem = rg.first;
    cap = firstFree = rg.second;
    return *this;
}

strVec::~strVec() {
    free();
}

void strVec::reallocate() {
    size_t oldCapacity = size();
    size_t newCapacity = oldCapacity ? 2*oldCapacity : 1;
    string *newElem = alloc.allocate(newCapacity);
    string *dest = newElem;
    string *src = elem;
    for (size_t i=0; i!=oldCapacity; ++i) {
        alloc.construct(dest++, std::move(*src++));
    }
    free();
    elem = newElem;
    firstFree = dest;
    cap = elem + newCapacity;
}

void strVec::push_back(string s) {
    if (firstFree == cap) reallocate();
    alloc.construct(firstFree++, s);
}


int main() {
     strVec sv;
     string s;
     cout << "input something delimted by space or enter(ctrl+d to terminate)" << endl;
     while (cin >> s) {
         sv.push_back(s);
     }
     strVec sv2(sv);

     for_each(sv.begin(), sv.end(), [] (const string &s) { cout << s << " "; });
     cout << endl;
     for_each(sv2.begin(), sv2.end(), [] (const string &s) { cout << s << " "; });
     cout << endl;
     sv2 = sv2;
     for_each(sv2.begin(), sv2.end(), [] (const string &s) { cout << s << " "; });
     cout << endl;
}
