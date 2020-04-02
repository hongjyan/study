#include <cstdio>
#include <string>
//default normal-constructor/copy-constructor/assign-operator/de-constructor by compiler

class Point {
public:
    Point(int x, int y): x_(x), y_(y) {};
private:
    int x_;
    int y_;
};

class Point2 {
public:
    Point2(const Point2& src): x_(src.x_), y_(src.y_) {}
private:
    int x_;
    int y_;
};

class Point3 {
public:
    int setX(int x) { x_ = x; return x_; }
    int setY(int y) { y_ = y; return y_; }
    int getX() { return x_; }
    int getY() { return y_; }
private:
    int x_;
    int y_;
};

class Point4 {
public:
    Point4() = default;  //compiller create default construct even we defined own constructer or copy-constructor
    Point4(int x, int y): x_(x), y_(y) {}
    Point4(const Point4& src): x_(src.x_), y_(src.y_) {}
private:
    int x_;
    int y_;
};

class Point5 {
public:
    Point5() { x_ = 0; y_ = 0;};
private:
    int x_;
    int y_;
};

//test default ctr is delete if class's member is const && no initialize && default ctr lacking
class Points {
private:
    Point3 p1;
    //const Point3 cp2; //ERROR when neigther initialization nor explicit default ctr
    const Point3 cp2{}; //OK when do initiaizetio even no explicit default ctr
    const Point5 cp3;   //OK when has explicit default ctr even without initialization
    //const Point4 cp3; //ERROR. sythesized ctr pointed via "= default"  is not explicit still.
};


int main() {
    //if we create one normal-constructor, compiler won't create more normal one, but will create one copy-constructor/assign-operator/de-constor for each one.
    //Point d; //error.
    Point a(1,2);
    Point c(3,4);
    Point b = a; //ok. 
    b = c; //ok.


    //if we create one copy-constructor, compiler won't create normal-constructor. and of course, won't create one more copy-constrctor since maximum number of copy-constructor is 1.
    //meanningless since this kind class cann't be instantiated at all !! But We can learn here we must create our own normal-constructor if we create our own copy-constructor 
    //Point2 a2; //Error.
    //Point2 b2 = a2; //ok
    Point4 a4; //ok

    //if we create nothing, compiler will create one normal-constructor/copy-constructor/assign-operator/de-constructor for each 
    Point3 a3; 
    a3.setX(1);
    a3.setY(2);
    Point3 b3 = a3;
    Point3 c3;
    a3 = c3; 

    //class default constructor will be  = delete if its member's default constructor do not exist
    Points ps;

    return 0;
}
