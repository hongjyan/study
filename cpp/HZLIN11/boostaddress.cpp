#include <boost/asio/ip/address.hpp>
#include <boost/system/error_code.hpp> 
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define IPV4 "255.255.255.155"

int main() {
    printf("sizeof(unsigned long) is %lu\n", sizeof(unsigned long));
    boost::system::error_code ec;
    boost::asio::ip::address_v4 addr4 = boost::asio::ip::address_v4::from_string(IPV4, ec);
    if (0 == ec) {
        printf("addr4.to_string() is %s, unsigned long for this address is %lu\n", 
                addr4.to_string().c_str(), addr4.to_ulong());
    }
    else {
        printf("failed, errorcode is %d\n", ec.value());
    }

    boost::asio::ip::address addr = boost::asio::ip::address::from_string(IPV4, ec);
    if (0 == ec) {
        printf("addr.to_string is %s\n", addr.to_string().c_str());
    } else {
        printf("failed, error is %d\n", ec.value());
    }

    boost::asio::ip::address_v4 nulladdr4;
    printf("nulladdr4.to_string is %s\n", nulladdr4.to_string().c_str());

    boost::asio::ip::address nulladdr(nulladdr4);
    printf("nulladdr.to_string is %s\n", nulladdr.to_string().c_str());

    boost::asio::ip::address_v4 wrongaddr4(14294967295);
    printf("wrongaddr4.to_string() is %s, unsigned long for this address is %lu\n",
            wrongaddr4.to_string().c_str(), wrongaddr4.to_ulong());


    sockaddr_in sockaddr;
    char addrarray[INET_ADDRSTRLEN] = {0};
    int ret = inet_pton(AF_INET, "255.255.255.255", &sockaddr.sin_addr);
    if (1 == ret) {
        printf("the address is %s\n", inet_ntop(AF_INET, &sockaddr.sin_addr, addrarray, INET_ADDRSTRLEN));
    } 
    else if (0 == ret) {
        printf("Invalid address\n");
    }
    else {
        printf("error\n");
    }
    
    return 0;
}
