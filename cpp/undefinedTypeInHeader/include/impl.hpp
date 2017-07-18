#ifndef _A_TEST_HPP_
#define _A_TEST_HPP_
//#include "namespaceProxy.hpp"
void showName(NamespaceProxy& nsp); //unknown type can be used in header file as long as the header file:namesapceProxy.hpp which include NamespaceProxy declaration and myown header:impl.hpp both are included in xxx.cpp file. And namespaceProxy.hpp must be inccluded before impl.hpp in .cpp file. 
//Since order need be guranteed so it is not recommended to do like this. Normal practice is including all header files which defines symbols which used by own. 
//undefined symbol can be used in header file
#endif
