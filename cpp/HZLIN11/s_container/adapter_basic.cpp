#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <list>
using namespace std;

int main () {
    int a[] = {1, 3, 2, 9, 5, 4, 8, 7, 6};
    deque<int> i_deq(a, a+9);
    vector<int> i_vec(a, a+9);
    list<int> i_lst(a, a+9);

    stack<int> stk(i_deq);
    stack<int, vector<int> > stk_vec(i_vec);
    stack<int, list<int> > stk_lst(i_lst);

    queue<int> que(i_deq);
//attention, error won't be detected if set vector as underlying container for queue during compile and running,//but it will throw error if queue.pop() will be called.
    queue<int, vector<int> > que_vec(i_vec); //wrong
    cout << que_vec.size() << endl;
    cout << que_vec.front() << endl;
//    que_vec.pop();
//
    queue<int, list<int> > que_lst(i_lst);
    que_lst.push(100);
    que_lst.push(101);
    cout << "The first element of que_lst is " << que_lst.front() << endl;
    cout << "The last element of que_lst is " << que_lst.back() << endl;
    

//prority_queue is heap, it is sorted already.
//    priority_queue<int> p_que(i_vec);  //can't use container to intialize priority_queue.see http://www.cpl\                                       usplus.com/reference/queue/priority_queue/priority_queue/
    priority_queue<int> p_que(i_vec.begin(), i_vec.end() );
    cout << p_que.top() << endl;
    while ( !p_que.empty() ) {
	cout << p_que.top() << endl;
	p_que.pop();
    }
    priority_queue<int, deque<int> > p_que_deq(i_deq.begin(), i_deq.end() );
//    priority_queue<int, list<int> > p_que_list(i_lst.begin(), i_lst.end() ); //wrong, will compile failed.
}
