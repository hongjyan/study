#include <stdio.h>

class Foo {
public:
    void pseudo_modify() const 
    { 
       printf("just print, no modify\n"); 
    }
};

void f(const Foo **p) {
   printf("I am f(const Foo **p)\n");
}   
 
void g(const Foo * const *p) {
   printf("I am g(const Foo * const *p)\n");
}   
 
int main()
{
   Foo F;
   Foo *p_F = &F;
   Foo **pp_test = &p_F;
//   f(pp_test);  //Error: it is ilegal and immoral to convert Foo** to const Foo**
   g(pp_test); //Okay: it is legal and moral to convert Foo** to const Foo* const *

//Reason explained.
   const Foo X = Foo();
   Foo Y;
   const Foo *p = &Y; 
   //const Foo *pFoo;    //correct modify.    
   Foo *pFoo;  //Wrong here.       
   const Foo **pp = &pFoo;  //Seems this OK since it is OK to assign const by non-const
   *pp = &X;            //Right. *pp is the value of pp, which is pFoo here.
   (**pp).pseudo_modify();     //obvious Wrong since **pp is "const Foo", can not modify const.
   (*pp)->pseudo_modify();     //seems OK, *pp is pFoo, pFoo is non-const, so can call modify.
//summary: Why 
//const Foo *const_p = &Y;---- OK. 
//But const Foo **pp = &pFoo---NOK.
//The reason is that const Foo **pp means **pp is const Foo, so can not use (**pp).modify(),
//But does not gurantee *pp points to const Foo, so can call *pp->modify() theretically. 
}
