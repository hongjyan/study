#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main() {
    std::string text("abc abd");
    boost::regex regex("ab.");

    boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
    boost::sregex_token_iterator end;

    for( ; iter != end; ++iter ) {
        std::cout<<*iter<<'\n';
    }

    return 0;
}
