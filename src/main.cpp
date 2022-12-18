#include <iostream>
#include <exception>
#include "Matrix.h"

using namespace std;

int main() {
    try {
        int p[] = {1, 2, 3, 4, 5, 6, 7, 8};
        Matrix<int> matrix0(2, 2, 2, p);
        Matrix<int> matrix1(matrix0);
        cout << matrix1 * matrix0 << endl;
        Matrix<int> matrix2(1, 1, 2);
        matrix2 = matrix1.getROI(0, 0, 0, 0);
        cout << matrix2 << endl;
    }
    catch (const char *msg) {
        cout << msg << endl;
    }
}