/*****************************************************************************
 * @brief header file
 * @module custom string
 * @owner Lu Jingjing
 *
 * Copyright 2016 Nokia. All rights reserved.
 *****************************************************************************/
#ifndef CUSTOM_STRING_HPP_
#define CUSTOM_STRING_HPP_

#include <memory>

#define DEFAULT_SIZE 7

class custom_string
{
    public:
        custom_string();
        explicit custom_string(const char *pstr);
        custom_string(custom_string& cs);
        ~custom_string();
        int find(const char *p);
        unsigned int length();
        char *c_str();

    private:
        std::unique_ptr<char> pstr;
        int size;

};
#endif
