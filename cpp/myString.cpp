#include <cstdio>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class String {
public:
    typedef size_t size_type;
    typedef char *iterator;
    typedef const char *const_iterator;
    String(): first_elem(nullptr), first_free(nullptr), cap(nullptr) {}
    String(const_iterator str);

    String(const String &rhs);
    String &operator=(const String &rhs);
    String(String &&rhs) noexcept;
    String &operator=(String &&rhs) noexcept;
    ~String() { free(); }
    
    bool empty() { return first_elem == nullptr; }
    size_type size() const { return first_free - first_elem; }
    iterator begin() { return first_elem; }
    const_iterator cbegin() const { return first_elem; }
    iterator end() { return first_free; }
    const_iterator cend() const { return first_free; }
    void push_back(const char c);

    std::string str() const { return std::string(cbegin(), cend()); }

private:
    iterator first_elem;
    iterator first_free;
    iterator cap;
    std::allocator<char> alloc;
    void reallocate();
    void free();
};


String::String(const_iterator str) {
    while ('\0' != *str) {
        push_back(*str++);
    }
}

void String::push_back(const char c) {
    if (cap == first_free) reallocate();
    alloc.construct(first_free++, c);
}

void String::reallocate() {
    size_type newCapacity = size() ? size()*2 : 1;
    iterator new_first_elem = alloc.allocate(newCapacity);
    iterator new_first_free = uninitialized_copy(first_elem, first_free, new_first_elem);
    free();
    first_elem = new_first_elem;
    first_free = new_first_free;
    cap = first_elem + newCapacity;
}

void String::free() {
    if (!empty()) {
        while (first_elem != first_free) {
            alloc.destroy(first_free--);
        }
        alloc.deallocate(first_elem, cap-first_elem);
    }
} 

String::String(const String &rhs) {
    std::cout << "String::String(const String &rhs)" << std::endl;
    first_elem = alloc.allocate(rhs.size());
    cap = first_free = std::uninitialized_copy(rhs.cbegin(), rhs.cend(), first_elem);
}



String &String::operator=(const String &rhs) {
    std::cout << "String::operator=(const String &rhs)" << std::endl;
    if (this != &rhs) {
        free();
        first_elem = alloc.allocate(rhs.size());
        cap = first_free = uninitialized_copy(rhs.cbegin(), rhs.cend(), first_elem);
    }
    return *this;
}

String::String(String &&rhs) noexcept: first_elem(rhs.first_elem), first_free(rhs.first_free), cap(rhs.cap) {
    std::cout << "String::String(String &&rhs)" << std::endl;
    rhs.first_elem = rhs.first_free = rhs.cap = nullptr;
}

String &String::operator=(String &&rhs) noexcept {
    std::cout << "String::operator=(String &&rhs)" << std::endl;
    if (this != &rhs) {
        free();
        first_elem = rhs.first_elem;
        first_free = rhs.first_free;
        cap = rhs.cap;
        rhs.first_elem = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}


void print(vector<String> &vs) {
    cout << "vs.size() is " << vs.size() << ", vs.capacity() is " << vs.capacity() << endl;
    for (auto &e: vs) {
        cout << e.str() << " ";
    }
    cout << endl;
}

int main() {
     String s1("happy");
     String s2 = s1;
     String s3;
     s3 = s1;
     cout << "s1 is " << s1.str() << ", s3 is " << s3.str() << endl;
     String s4(std::move(s3));
     cout << "s4 is " << s4.str() << ", s3 is " << s3.str() << endl;
     String s5 = std::move(s4);
     cout << "s1 is " << s1.str() << ", s4 is " << s4.str() << ", s5 is " << s5.str() << endl;
     s2 = std::move(s5);
     cout << "s2 is " << s2.str() << ", s5 is " << s5.str() << endl;

     cout << "-----------------------------------------" << endl;
     vector<String> vs;
     vs.push_back(s1); print(vs);
     vs.push_back(s2); print(vs);
     String s6("fun"); vs.push_back(s6); print(vs);

     return 0;
}
