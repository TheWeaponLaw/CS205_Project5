#pragma once
#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class Matrix
{
    size_t row;
    size_t column;
    size_t channel;

    std::shared_ptr<T> data = nullptr;

public:
    Matrix(size_t row = 0, size_t column = 0, size_t channel = 0);
    ~Matrix()
    {
        cout << "Deconstructed!" << endl;
    }
};
