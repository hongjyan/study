//g++ -Wall -lboost_regex -std=c++11 regex_match.cpp 
//
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <iostream>

int main() {
    const char *input = "24 bytes from 212.212.212.212: icmp_seq=4 ttl=64 time=0.989 ms";
    boost::regex expression(".*(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms");

    boost::cmatch matches;

    std::cout << "------------Basic usage char---------------" << std::endl;
    if (boost::regex_match(input, matches, expression)) { 
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    {
        std::cout << "------------Basic usage string---------------" << std::endl;
        std::string inputstring(input);
        boost::smatch what;
        if (boost::regex_match(inputstring, what, expression)) {
             for (auto it=what.begin(); it!=what.end(); ++it)
                  std::cout << *it << std::endl;
        }

    }

    
    std::cout << "------------matches can re-use since it will be clear nex time---------------" << std::endl;
    if (boost::regex_match(input, matches, expression)) {  //seems matches will be cleared when entering regex_match 
        for (auto it=matches.begin()+1; it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    //boost::regex_match only matches the whole string, boost::regex_search can match sub-string.
    std::cout << "------------Test regex_search---------------" << std::endl;
    boost::regex e("(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms");
    if (boost::regex_search(input, matches, e)) { 
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }


    //^ vs A, $ vs Z
    std::cout << "---------------Test ^ vs A, $ vs Z--------------------" << std::endl;
    boost::regex expression1("^\\d+.*(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms$");
    boost::regex expression2("\\A\\d+.*(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms\\Z");
    const char *input3 = "game\n24 bytes from 212.212.212.212: icmp_seq=2 ttl=67 time=0.994 ms";
    if (boost::regex_search(input3, matches, expression1)) {      //Can match
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
    std::cout << "\\A \\Z" << std::endl;    //can not match
    if (boost::regex_search(input3, matches, expression2)) {
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }


    //seems default match is not greedy. return the last:0.994, seems 0.994 match better than 0.989
    std::cout << "-------------match_default is not greedy----------------------" << std::endl;
    const char *input1 = "24 bytes from 212.212.212.212: icmp_seq=4 ttl=64 time=0.989 ms 24 bytes from 212.212.212.212: icmp_seq=2 ttl=67 time=0.994 ms";
    if (boost::regex_search(input1, matches, expression)) { 
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    //match all sub-string
    std::cout << "--------------get all matches------------" << std::endl;
    const char *input4 = "24 bytes from 212.212.212.212: icmp_seq=4 ttl=64 time=0.989 ms 24 bytes from 212.212.212.212: icmp_seq=2 ttl=67 time=0.994 ms\n 24 bytes from 212.212.212.212: icmp_seq=5 ttl=64 time=1.99 ms";
    std::string inputstring(input4);
    boost::regex expression3("(icmp_seq=\\d+)\\s+(ttl=\\d+)\\s+(time=\\d+\\.\\d+)\\s+ms");
    
    {
        std::cout << ">>>>>>>>>Way1" << std::endl;
        boost::sregex_token_iterator iter(inputstring.begin(), inputstring.end(), expression3, 0);
        boost::sregex_token_iterator end;
        while (iter != end) {
            std::cout << *iter << std::endl;
            boost::cmatch what;
            std::string entry(*iter);
            regex_match(entry.c_str(), what, expression3);
            for (auto it=what.begin()+1; it!=what.end(); ++it)
                std::cout << *it << " ";
            std::cout <<  std::endl;
            ++iter;
        }
   }


   {
       std::cout << ">>>>>>>>>Way2" << std::endl;
       std::string::const_iterator start = inputstring.begin(); //must const_iterator. iterator is wrong. 
       std::string::const_iterator end = inputstring.end();
       boost::smatch what;
       std::vector<boost::smatch> matches;
       while (start != end) {
           if (boost::regex_search(start, end, what, expression3))
               matches.push_back(what);
           start = what[0].second;
       }

       int i = 0;
       for (auto it=matches.begin(); it!=matches.end(); ++it) {
           std::cout << "item" << i  << std::endl;
           for (auto mit=it->begin(); mit!=it->end(); ++mit) {
               std::cout << *mit << std::endl;
           }
           std::cout << std::endl;
           ++i;
       }    
   }
       

   std::cout << ">>>>>>>>>Way3" << std::endl;
   const int submatches[3] = {1, 2, 3};
   boost::sregex_token_iterator iter(inputstring.begin(), inputstring.end(), expression3, submatches);
   boost::sregex_token_iterator end;
   while (iter != end) {
       std::cout << *iter << std::endl;
       ++iter;
   }
   
    //\n do not stop regex match.
    std::cout << "--------------\\n do not stop regex match---------------------" << std::endl;
    const char *input2 = "game\n\
24 bytes from 212.212.212.212: icmp_seq=2 ttl=67 time=0.994 ms";
    if (boost::regex_match(input2, matches, expression)) {
        for (auto it=matches.begin(); it!=matches.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    return 0;
}
