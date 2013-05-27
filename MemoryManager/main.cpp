#include <cstdlib>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "Header.h"
//#include "global.cpp"
const char* __file__ = "unknown";
size_t __line__ = 0;

#define new (__file__=__FILE__,__line__=__LINE__) & 0 ? NULL : new
//#define delete (__file__=__FILE__,__line__=__LINE__) & 0 ? NULL : delete

using namespace std;
class A
{  public:
    A(){a=1;}
    int   a;
};
class B: public A
{  public:
    int   b;
};

int main() {
    int*     pi1 = new int;
    char*    pc1 = new char('a');
    int*     pi2 = new int[100];
    char*    pc2 = new char[100];
    A*       pA1 = new A();
    A*       pA2 = new A[100];
    B*       pB1 = new B();
    B*       pB2 = new B[100];
    string*  pS1 = new string("a");
    string*  pS2 = new string[100];
    delete pi1;
    delete pc1;
    delete pc2; // << wrong delete
    delete pB1;
    delete [] pB2;
    //delete pA1;
    delete [] pA2;
    delete pS1;
    //delete [] pS2;
    
    std::cout << __file__ << ":" << __line__;

    return 0;
}
