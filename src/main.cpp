#include <iostream>
#include <exception>
#include "Matrix.h"
using namespace std;
int main()
{
    // try
    // {
    //     Matrix<int> matrix0(0, 1, 2);
    // }
    // catch (const char *msg)
    // {
    //     cout << msg << endl;
    // }

    int *p = nullptr;
    delete p;
    delete p;
}