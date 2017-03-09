#include "Cstring.hpp"
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;


custom_string::custom_string()
{
    int actual_size = 0;

    this->size = DEFAULT_SIZE;
    actual_size = this->size + 1;
    this->pstr = std::unique_ptr<char>(new char[actual_size]);
    memset(this->c_str(), '\0', actual_size);

}

custom_string::custom_string(const char *pstr)
{
    int actual_size = 0;

    if (pstr != NULL)
    {
        this->size = strlen(pstr);
        actual_size = this->size + 1;
        this->pstr = std::unique_ptr<char>(new char[actual_size]);
        memset(this->c_str(), '\0', actual_size);
        strncpy(this->c_str(), pstr, this->size);
    }
    else
    {
        return;
    }
}

custom_string::custom_string(custom_string& cs)
{
    int actual_size = 0;

    this->size = strlen(cs.c_str());
    actual_size = this->size + 1;
    this->pstr = std::unique_ptr<char>(new char[actual_size]);
    memset(this->c_str(), '\0', actual_size);
    strncpy(this->c_str(), cs.c_str(), this->size);

}

custom_string::~custom_string()
{

}

int custom_string::find(const char *p)
{
    int pos = -1;

    if (p != NULL)
    {
        pos = strstr(this->c_str(), p) - this->c_str();
    }

    return pos;
}

unsigned int custom_string::length()
{
    return strlen(this->c_str());
}

char *custom_string::c_str()
{
    return this->pstr.get();
}

int main()
{
    custom_string cs1;
    custom_string cs2("cs2");
    custom_string cs3(cs2);

    if (sizeof(cs1.c_str()) == DEFAULT_SIZE + 1)
    {
        cout<<"Test default constructor success!"<<" cs1.c_str()="<<static_cast<const void *>(cs1.c_str())<<", cs1.length()="<<cs1.length()<<endl; 
    }

    if ((cs2.length() == strlen(cs2.c_str())) && (!strcmp(cs2.c_str(), "cs2")))
    {
        if ((cs2.find("2") == 2) && (cs2.find("1") < 0))
        {
            cout<<"Test constructor with string paramter success!"<<" cs2.c_str()="<<static_cast<const void *>(cs2.c_str())<<", cs2.length()="<<cs2.length()<<endl;
        }
    }

    if ((cs3.length() == strlen(cs2.c_str())) && (!strcmp(cs3.c_str(), cs2.c_str())))
    {
        cout<<"Test copy constructor success!"<<" cs3.c_str()="<<static_cast<const void *>(cs3.c_str())<<", cs3.length()="<<cs3.length()<<endl;
    }

    return 1;

}