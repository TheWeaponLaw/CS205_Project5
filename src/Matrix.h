#pragma once
#include <memory>
#include <iostream>
#include "Data_I.h"
using namespace std;

template <typename T>
class Matrix
{
private:
    size_t row;
    size_t col;
    size_t channel;
    Data<T> *data;

public:
    Matrix(size_t row, size_t col, size_t channel);
    Matrix(size_t row, size_t col, size_t channel, T *data);

    Matrix<T> &operator=(const Matrix<T> &matrix);
    Matrix<T> &operator+(const Matrix<T> &matrix);
    bool operator==(const Matrix<T> &matrix) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
    {
        if (matrix.data == nullptr)
        {
            cout << "The matrix data is empty!" << endl;
        }
        else
        {
            os << "The matrix is " << endl;
            for (size_t i = 0; i < matrix.channel; ++i)
            {
                string str;
                T *part_pointer = matrix.data->getData() + i * matrix.row * matrix.col;
                str += "The No." + to_string(i + 1) + " channel is\n";
                for (size_t k = 0; k < matrix.row; ++k)
                {
                    for (size_t j = 0; j < matrix.col; ++j)
                    {
                        str += to_string(*(part_pointer + k * matrix.col + j)) + " ";
                    }
                    str += "\n";
                }
                str += "\n";
                os << str;
            }
        }
        return os;
    }

    ~Matrix();
};

template <typename T>
Matrix<T>::Matrix(size_t row, size_t col, size_t channel)
{
    try
    {
        if (row == 0 || col == 0 || channel == 0) // judge the valid before construct
        {
            throw "The data of matrix is invalid.";
        }
        else
        {
            this->row = row;
            this->col = col;
            this->channel = channel;
            this->data = new Data<T>(row, col, channel);
        }
    }
    catch (const bad_alloc &e)
    {
        delete data;
        data = nullptr;
        throw "Memory allocated failed!";
    }
    catch (const char *msg) // catch the expectation especially in valid data and memory allocated
    {
        throw msg;
    }
    catch (...)
    {
        throw "Unknown exception!";
    }
}

template <typename T>
Matrix<T>::Matrix(size_t row, size_t col, size_t channel, T *data)
{
    try
    {
        if (data == nullptr)
        {
            throw "The data is empty!";
        }
        else if (row == 0 || col == 0 || channel == 0) // judge the valid before construct
        {
            throw "The data of matrix is invalid.";
        }
        else
        {
            this->row = row;
            this->col = col;
            this->channel = channel;
            this->data = new Data<T>(row, col, channel, data);
        }
    }
    catch (const bad_alloc &e)
    {
        delete data;
        data = nullptr;
        throw "Memory allocated failed!";
    }
    catch (const char *msg) // catch the expectation especially in valid data and memory allocated
    {
        throw msg;
    }
    catch (...)
    {
        throw "Unknown exception!";
    }
}

template <typename T>
Matrix<T>::~Matrix()
{
    delete data;
    data = nullptr;
    cout << "Deconstructed!" << endl;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &matrix)
{
    if (matrix.data == nullptr)
    {
        cout << "The equal matrix is empty!";
    }
    else if (this == &matrix) // Judge same
    {
        return *this;
    }
    else if (matrix.row == 0 || matrix.col == 0 || matrix.channel == 0 || matrix.data == nullptr)
    {
        cout << "The equal matrix is invalid!" << endl;
        return *this;
    }
    else if (matrix.row != this->row || matrix.col != this->col || matrix.channel != this->channel)
    {
        cout << "Two matrix have different size!" << endl;
        return *this;
    }
    else
    {
        *(this->data) = *(matrix.data);
        this->channel = matrix.channel;
        this->row = matrix.row;
        this->col = matrix.col;
        return *this;
    }
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &matrix) const
{
    if (this->data == nullptr || matrix->data == nullptr)
    {
        cout << "The matrix data is empty!" << endl;
        return false;
    }
    else if (this->col != matrix.col || this->row != matrix.row || this->channel != matrix.channel)
    {
        return false;
    }
    else if (this->data == matrix.data)
    {
        return ture;
    }
    else
    {
        if (memcmp(this->data, matrix.data, this->col * this->row * this->channel * sizeof(T)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
