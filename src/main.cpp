#include <iostream>
#include <exception>
#include "Matrix.h"

using namespace std;

int main()
{
    try
    {

        Matrix<int> matrix2(0, 3, 1);
        cout << matrix2;
    }
    catch (const char *msg)
    {
        cout << msg << endl;
    }
}