#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>

struct LastWordLength
{
    static unsigned int calculate()
    {
        std::string line;
        std::string word;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        while(iss >> word) {}
        return word.size(); 
    }
};



struct IO
{
    void getline()
    {
        std::getline(std::cin, line_);
    }

    std::string putline() { return line_; }

    void getword()
    {
        std::cin >> word_;
    }

    std::string putword() { return word_; }

    void getchar()
    {
        // 不知道怎么只允许用户输入一个字符。
        // char_ = ::getchar();
        // scanf(" %c", &char_);
        std::cin >> char_;
        // std::cin.clear();
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // std::cin.clear();
        // std::cin.get(char_);
    }
    char putchar() { return char_; }

private:
    std::string line_;
    std::string  word_;
    char char_;
};


struct NumOfSameChar
{
    static unsigned int calculate()
    {
        unsigned int ret = 0;
        IO io;
        io.getline();
        io.getchar();
        const std::string& line = io.putline();
        const char& c = io.putchar();
        // std::cout << "line is:" << line;
        // std::cout << "c is:" << c;
        int step = 'a' - 'A';
        int negetiveStep = 'A' - 'a';
        bool isNum = ('0' <= c <= '9');
        for (auto it=line.begin(); it!=line.end(); ++it) {
            if (isNum) {
                if (*it == c) { ++ret; }
            } else {
                int diff = *it - c;
                if (0==diff || step==diff || negetiveStep==diff) { ++ret; }
            }
        }
        return ret;
    }
};

int main()
{
    // std::cout << LastWordLength::calculate();
    std::cout << NumOfSameChar::calculate();
    return 0;
}
