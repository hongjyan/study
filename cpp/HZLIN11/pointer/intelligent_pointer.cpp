#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

class Screen {
public:
    Screen(int length=0, int height=0): len(length), hei(height), p(NULL) {
	p = (char*) malloc(sizeof(char) * len * hei);
	memset(p,'0', sizeof(char)*len*hei); 
    }
    
    void Output() {
	for(int i=0; i<len; i++) {
	    for(int j=0; j<hei; j++) {
		cout<<*(p + i*len*sizeof(char) + j*sizeof(char));
	    }
	    cout<<endl;
	} 
    }
    
    ~Screen() { cout<<"delete memory used in Screen"<<endl; delete p; }

private:
    int len, hei;
    char *p;
};



class I_screenPtr;
//private class to count how many pointer:*Screen are being in use.
class ScrPtr {
    friend class I_screenPtr;
    ScrPtr(Screen *p): sp(p), use(1) { cout << "ScrPtr.use is " << use << " in ScrPtr(Screen *p)" << endl; }
    Screen *sp;
    int use;
    ~ScrPtr() { cout << "destructing ScrPtr now" << endl;}
};


//API to user
class I_screenPtr {
public:
    I_screenPtr(Screen *p): scrptr(new ScrPtr(p)) {};
    I_screenPtr(I_screenPtr &orig): scrptr(orig.scrptr) { 
	++scrptr->use; 
	cout<<"scrptr->use is "<<scrptr->use<<" in I_screenPtr(I_screenPtr &)"<<endl; 
    }
    I_screenPtr& operator=(const I_screenPtr &orig) {
	if(--scrptr->use == 0) { delete scrptr; }
	scrptr = orig.scrptr;
	++scrptr->use;
	cout<<"scrptr->use is "<<scrptr->use<<" in I_screenPtr& operator=(const I_screenPtr &orig)"<<endl; 
	return *this;
    }
    ~I_screenPtr() { 
	cout<<"scrpt->use is "<<scrptr->use<<" in destructor"<<endl;
	if(--scrptr->use == 0) { cout<<"scrpt->use is "<<scrptr->use<<" in destructor"<<endl; delete scrptr; } 
    }
private:
    ScrPtr *scrptr;
};


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$intelligent pointer$$$$$$$$$$$$$$$$$$$$$$$
class IntelligentScreenPtr;
//private class to count how many pointer:*Screen are being in use.
class count_scrPtr {
    friend class IntelligentScreenPtr;
    count_scrPtr(Screen *p): sp(p), use(1) { cout << "count_scrPtr.use is " << use << " in count_scrPtr(Screen *p)" << endl; }
    Screen *sp;
    int use;
    ~count_scrPtr() { cout << "destructing count_scrPtr now" << endl; delete sp;}  //intelligent pointer violate normal user defined destructor.\
						    //release memory though there isn't memory allocate or special operation here.
		    //The pointer parameter of contructor of intelligent pointer shall be memory allocated by new, should not be address of real object.
};


//API to user
class IntelligentScreenPtr {
public:
    IntelligentScreenPtr(Screen *p): p_count_scrPtr(new count_scrPtr(p)) {};
    IntelligentScreenPtr(IntelligentScreenPtr &orig): p_count_scrPtr(orig.p_count_scrPtr) { 
	++p_count_scrPtr->use; 
	cout<<"p_count_scrPtr->use is "<<p_count_scrPtr->use<<" in IntelligentScreenPtr(IntelligentScreenPtr &)"<<endl; 
    }
    IntelligentScreenPtr& operator=(const IntelligentScreenPtr &orig) {
	if(--p_count_scrPtr->use == 0) { delete p_count_scrPtr; }
	p_count_scrPtr = orig.p_count_scrPtr;
	++p_count_scrPtr->use;
	cout<<"p_count_scrPtr->use is "<<p_count_scrPtr->use<<" in IntelligentScreenPtr& operator=(const IntelligentScreenPtr &orig)"<<endl; 
	return *this;
    }
    ~IntelligentScreenPtr() { 
	cout<<"p_count_scrPtr->use is "<<p_count_scrPtr->use<<" in destructor"<<endl;
	if(--p_count_scrPtr->use == 0) { cout<<"scrpt->use is "<<p_count_scrPtr->use<<" in destructor"<<endl; delete p_count_scrPtr; } 
    }
private:
    count_scrPtr *p_count_scrPtr;
};



class In_screenPtr {
public:
    In_screenPtr(Screen *sp): m_sp(sp), count(1) {}

    In_screenPtr(In_screenPtr &isp) {
	m_sp = isp.m_sp;
	++isp.count;
	count = isp.count;
	cout << "count is " << count << " in In_screenPtr(In_screenPtr &isp)" << endl;
    }
    
    In_screenPtr& operator=(In_screenPtr &orig) {
	if (m_sp == orig.m_sp) {
	    return *this;
	}

	if (0 == --count) {
	    delete m_sp;
	}
	
	m_sp = orig.m_sp;
	++orig.count;
	count = orig.count;
	cout << "count is " << count << " in In_screenptr& operator=(In_screenPtr &orig)" << endl;
	return *this;
    }
    
    Screen operator*() {
	return *m_sp;
    }
    
    
    Screen* operator->() {
	return m_sp;
    }


    ~In_screenPtr() {
	if (--count  == 0) { cout << "count is " << count << " in ~In_screenPtr()" << endl; delete m_sp; }
	cout << "count is " << count << " in ~In_screenPtr()" << endl;
    }

private:
    Screen *m_sp;
    int count;
};




int main() {
///*
    class Screen s(5, 6);
    s.Output();
    I_screenPtr sp(&s);
    I_screenPtr sp1(sp);
    I_screenPtr sp2(sp1);
    sp2 = sp;
//*/
    Screen *ps = new Screen(1,2);
    I_screenPtr d_sp(ps);
    I_screenPtr d_sp1(d_sp);
    I_screenPtr d_sp2(d_sp1);
    d_sp2 = d_sp;
    delete ps; //still need delete since I_screenPtr is not real intelligent pointer. 

//real intelligent pointer
///*
    Screen *p = new Screen(7, 8);
    p->Output();
    IntelligentScreenPtr dynamic_sp(p);
    IntelligentScreenPtr dynamic_sp1(dynamic_sp);
    IntelligentScreenPtr dynamic_sp2(dynamic_sp1);
    IntelligentScreenPtr dynamic_sp3(dynamic_sp);
    dynamic_sp3 = dynamic_sp2;
//    IntelligentScreenPtr sp_I(&s);  //wrong, can't use address of existing object to create intelligent pointer, since it will destruct this object before\
				      it out of scope.
//*/

    
    
//from the output of In_screenPtr, we can see there must be 3 layers class.
//The uppest layer: intelligentPointer as interface provided to user.
//middleLayer: class includes "pointer to base",  "count  to record how many upperlayer instances are sharing same base pointer".
//The lowest layer: base pointer which pointer to a object that reserve memory via new/malloc.  
//essence why 2 layers can't work is that no place sharing "countto to record how many upperlayer intances are sharing same base pointer". 
///*
    In_screenPtr in_sp(&s);
    in_sp->Output();
    In_screenPtr in_sp1 = in_sp;
    In_screenPtr in_sp2 = in_sp;
    In_screenPtr in_sp3 = in_sp2;
//*/


    cout << "************END**********" << endl;
}
