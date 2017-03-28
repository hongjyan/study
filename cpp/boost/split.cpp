#include <cstdio>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <string>

//need -lboost_system for link
int main()
{
    const char* ids = "1,2,3 ";
    std::vector<std::string> idVec;
    boost::algorithm::split(idVec, ids, boost::is_any_of(","));
    for (auto it=idVec.begin(); it!=idVec.end(); ++it) {
        printf("%s\n", it->c_str());
    }
    return 0;
}
