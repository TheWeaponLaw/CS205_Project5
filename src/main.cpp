#include <iostream>
#include <exception>
#include "Matrix.h"
using namespace std;
int main()
{
    try
    {
        Matrix<int> matrix0(1, 1, 2);
        Matrix<int> matrix1(matrix0);
        cout << (matrix0 == matrix1) << endl;
    }
    catch (const char *msg)
    {
        cout << msg << endl;
    }
}