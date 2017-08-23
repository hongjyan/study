#include <iostream>
using namespace std;

void hanoi(int n,char p1,char p2,char p3)
{
	if(1==n)
		cout << "move plate:top1 from " << p1 <<" to " << p3 <<endl;
	else
	{
		hanoi(n-1,p1,p3,p2);
		cout<<"move plate top" << n << " from "<<p1<<" to "<<p3<<endl;
		hanoi(n-1,p2,p1,p3);
	}
}

int main(int argc, char* argv[]) {
    hanoi(5, 'a', 'b', 'c');
    return 0;
}
