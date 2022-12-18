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
    size_t start = 0;
    size_t step = 0;
    size_t par_size = 0;
    Data<T> *data = nullptr;

public:
    Matrix(size_t row, size_t col, size_t channel);

    Matrix(size_t row, size_t col, size_t channel, T *data);

    Matrix(Matrix &matrix);

    Matrix<T> &operator=(const Matrix<T> &matrix);

    Matrix<T> &operator+(const Matrix<T> &matrix);

    Matrix<T> &operator-(const Matrix<T> &matrix);

    Matrix<T> &operator*(const Matrix<T> &matrix);

    Matrix<T> &getROI(size_t dest1_x, size_t dest1_y, size_t dest2_x, size_t dest2_y);

    bool operator==(const Matrix<T> &matrix);

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
                T *part_pointer = matrix.data->getData() + i * matrix.par_size + matrix.start;
                str += "The No." + to_string(i + 1) + " channel is\n";
                for (size_t k = 0; k < matrix.row; ++k)
                {
                    for (size_t j = 0; j < matrix.col; ++j)
                    {
                        str += to_string(*(part_pointer++)) + " ";
                    }
                    str += "\n";
                    part_pointer += matrix.step;
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
            this->par_size = row * col;
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
            this->par_size = row * col;
            this->data = new Data<T>(row, col, channel, data);
        }
    }
    catch (const bad_alloc &e)
    {
        delete this->data;
        this->data = nullptr;
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
Matrix<T>::Matrix(Matrix<T> &matrix)
{
    try
    {
        if (matrix.data == nullptr)
        {
            throw "The data is empty!";
        }
        else if (matrix.row == 0 || matrix.col == 0 || matrix.channel == 0) // judge the valid before construct
        {
            throw "The data of matrix is invalid.";
        }
        else
        {
            this->row = matrix.row;
            this->col = matrix.col;
            this->channel = matrix.channel;
            this->par_size = matrix.par_size;
            this->start = matrix.start;
            this->step = matrix.step;
            this->data = new Data<T>(*matrix.data);
        }
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
    delete (data - start);
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
    else
    {
        *(this->data) = *(matrix.data);
        this->channel = matrix.channel;
        this->row = matrix.row;
        this->col = matrix.col;
        this->start = matrix.start;
        this->step = matrix.step;
        this->par_size = matrix.par_size;
        return *this;
    }
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &matrix)
{
    if (this->data == nullptr || matrix.data == nullptr)
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
        return true;
    }
    else
    {
        if (memcmp(this->data - start, matrix.data - start, this->col * this->row * this->channel * sizeof(T)) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator+(const Matrix<T> &matrix)
{
    if (matrix.data == nullptr || this->data == nullptr)
    {
        cout << "The data is empty!" << endl;
        return *this;
    }
    else if (this->row != matrix.row || this->col != matrix.col || this->channel != matrix.channel)
    {
        cout << "The size of matrix are not same!" << endl;
        return *this;
    }
    else
    {
        Matrix<T> *matrix_res = new Matrix<T>(this->row, this->col, this->channel);
        T *data_result = matrix_res->data->getData();
        T *data1 = this->data->getData() + this->start;
        T *data2 = matrix.data->getData() + matrix.start;
        for (size_t i = 0; i < this->channel; ++i)
        {
            for (size_t j = 0; j < this->row; ++j)
            {
                for (size_t k = 0; k < this->col; ++k)
                {
                    (*(data_result++)) = (*(data1++)) + (*(data2++));
                }
                data1 += this->step;
                data2 += matrix.step;
            }
            if (step != 0)
            {
                data1 += this->par_size;
                data2 += matrix.par_size;
            }
        }
        return *matrix_res;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator-(const Matrix<T> &matrix)
{
    if (matrix.data == nullptr || this->data == nullptr)
    {
        cout << "The data is empty!" << endl;
        return *this;
    }
    else if (this->row != matrix.row || this->col != matrix.col || this->channel != matrix.channel)
    {
        cout << "The size of matrix are not same!" << endl;
        return *this;
    }
    else
    {
        Matrix<T> *matrix_res = new Matrix<T>(this->row, this->col, this->channel);
        T *data_result = matrix_res->data->getData();
        T *data1 = this->data->getData() + start;
        T *data2 = matrix.data->getData() + start;
        for (size_t i = 0; i < this->channel; ++i)
        {
            for (size_t j = 0; j < this->row; ++j)
            {
                for (size_t k = 0; k < this->col; ++k)
                {
                    (*(data_result++)) = (*(data1++)) - (*(data2++));
                }
                data1 += this->step;
                data2 += matrix->step;
            }
            if (step != 0)
            {
                data1 += this->par_size;
                data2 += matrix.par_size;
            }
        }
        return *matrix_res;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(const Matrix &matrix)
{
    if (matrix.data == nullptr || this->data == nullptr)
    {
        cout << "The multiple matrix are empty!" << endl;
        return *this;
    }
    else if (this->col != matrix.row || this->channel != matrix.channel)
    {
        cout << "The size of matrix are not match!" << endl;
        return *this;
    }
    else
    {
        Matrix<T> *matrix_res = new Matrix<T>(this->row, matrix.col, this->channel);
        T *data_res = matrix_res->data->getData();
        T *data1 = this->data->getData() + start;
        T *data2 = matrix.data->getData() + start;
#pragma omp parallel for
        for (size_t c = 0; c < this->channel; ++c)
        {
            for (size_t i = 0; i < this->row; ++i)
            {
                for (size_t k = 0; k < this->col; ++k)
                {
                    for (size_t j = 0; j < matrix.col; ++j)
                    {
                        *(data_res + i * matrix_res->col + j) +=
                            *(data1 + i * (this->col + step) + k) * *(data2 + k * (matrix.col + step) + j);
                    }
                }
            }
            data_res += matrix_res->col * matrix_res->row;
            data1 += this->col * this->row;
            data2 += matrix.col * matrix.row;
        }
        return *matrix_res;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::getROI(size_t dest1_x, size_t dest1_y, size_t dest2_x, size_t dest2_y)
{
    if (this->data == nullptr)
    {
        cout << "The matrix is invalid!" << endl;
        return *this;
    }
    else if (dest1_x < 0 || dest2_x > this->col || dest1_y < 0 || dest2_y > this->row || dest1_x > dest2_x ||
             dest1_y > dest2_y)
    {
        cout << "The matrix data is invalid!" << endl;
        return *this;
    }
    else
    {
        Matrix<T> *matrix = this;
        matrix->start = dest1_y * this->col + dest2_x;
        matrix->par_size = this->col * this->row;
        matrix->row = dest2_y - dest1_y + 1;
        matrix->col = dest2_x - dest1_x + 1;
        matrix->step = this->row - matrix->row;
        return *matrix;
    }
}
