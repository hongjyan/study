#include <iostream>
#include <cstdlib>
using namespace std;

class Obj {
public:
    Obj(void) { cout<<"construct Obj"<<endl; }
    ~Obj(void) { cout<<"destruct Obj"<<endl; }
    void Initialize(void) { cout<<"Initialize Obj"<<endl; }
    void Destroy(void) { cout<<"Destroy Obj"<<endl; }
};

//the output of this funcation shows construct/desconstruct won't be executed for malloc/free.
void UseMallocFree(void) {
    Obj *a = (Obj *) malloc(sizeof(Obj));
    a->Initialize();
    a->Destroy();
    free(a);
}

void UseNewDelete(void) {
    Obj *a = new(Obj);
    delete a;
}

int main() {
    UseMallocFree();
    UseNewDelete();
}
