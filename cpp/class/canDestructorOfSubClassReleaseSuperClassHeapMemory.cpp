#include <stdio.h>

class father {
public:
    father(const char* name): mName(new const char(*name)) {}; //new const char is not correct here, should something like int cosnt char [300]; 
    virtual void shutoff() = 0;
    virtual void shut() { 
        if (mName)  {
           delete mName;
           printf("shut() in father to release mName\n"); 
        }
    }
    virtual void pureVirtual() = 0;
    void stop() {
         //can not free sub-class resource from super class.
        //this->shutoff();  //The idea: super calss free subclass's resource by calling virtual-function who will release resource of subclass is good, but will crash since during constructor of base class, pure virtual funcation is not ready, during deconstructor of base class pure virutal funcation was released then.
        this->shut();  //will call father->shut() since stop is called in father-deconstrcutor.
    }
    
    virtual void virtualF_toCallPureVirtual() {
         printf("virtualF_toCallPureVirtual() in father\n");
         pureVirtual();
    }
    
    virtual ~father() {
        printf("father destructing\n");
        stop();
        printf("father destructing finised\n");
    }

    const char* mName;
};

class son : public father {
public:
    son(int i, const char* name, const char* str): father(name), mpack(new int(i)), mstr(new const char(*str)) { 
        printf("son constructing\n"); 
    }

    virtual void shutoff() { 
        printf("About to release mpack,mstr in son\n"); 
        if (mpack) delete mpack; mpack = NULL;
        if (mstr) delete mstr; mstr = NULL;
    }

    virtual void shut() { 
        if (mName) {
             delete mName; mName = NULL; //can release super-class resource from sub-class. set mName to NULL to prevent double free.
             printf("shut() in son to release mName\n");
        } 
    }
 
    virtual void pureVirtual() { printf("mstr is %s\n", mstr); printf("pureVirtual() in son\n"); }

    virtual void virtualF_toCallPureVirtual() {
        printf("virtualF_toCallPureVirual() in son\n");
        pureVirtual();
        father::virtualF_toCallPureVirtual();
    }

    virtual ~son() {
        printf("son destructing\n"); 
        shutoff();
        shut();
        printf("son destructing finished\n"); 
    }

private:
   int* mpack;
   const char* mstr;
};


int main() {
    son s1(1, "Li", "foo");
    s1.virtualF_toCallPureVirtual();
}
