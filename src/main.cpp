#include <iostream>
#include <exception>
#include "Matrix.h"

using namespace std;

int main()
{
    try
    {
        int p[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
        int p1[] = {3, 4, 7, 9, 10, 11, 12, 15, 17, 2, 1, 5, 9, 3, 2, 4, 17, 18};
        Matrix<int> matrix0(3, 3, 2, p);
        Matrix<int> matrix1(3, 3, 2, p1);
        cout << matrix0;
        cout << matrix1;
        cout << matrix0 + matrix1;
    }
    catch (const char *msg)
    {
        cout << msg << endl;
    }
}