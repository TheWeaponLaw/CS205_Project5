#pragma once

#include <iostream>
#include <cstring>

using namespace std;

template <typename T>
class Data
{
    int *ref = new int(0);
    T *data = nullptr;

public:
    Data(size_t row, size_t col, size_t channel);
    Data(size_t row, size_t col, size_t channel, T *data);
    Data(Data<T> &data);
    T *getData()
    {
        return this->data;
    }
    Data<T> &operator=(const Data<T> &data);
    ~Data();
};

template <typename T>
Data<T>::Data(size_t row, size_t col, size_t channel)
{
    try
    {
        data = new T[row * col * channel]();
        *ref = 1;
    }
    catch (const bad_alloc &e)
    {
        try
        {
            delete ref;
            ref = nullptr;
            delete[] data;
            data = nullptr;
            throw "Memory allocated fail";
        }
        catch (...)
        {
            throw "Unknown exceptions";
        }
    }
    catch (...)
    {
        delete ref;
        ref = nullptr;
        delete[] data;
        data = nullptr;
        throw "Unknown exceptions";
    }
}

template <typename T>
Data<T>::Data(size_t row, size_t col, size_t channel, T *data)
{
    try
    {
        this->data = new T[row * col * channel];
        *ref = 1;
        memcpy(this->data, data, row * col * channel * sizeof(T));
    }
    catch (const bad_alloc &e)
    {
        try
        {
            delete ref;
            ref = nullptr;
            delete[] data;
            data = nullptr;
            throw "Memory allocated fail";
        }
        catch (...)
        {
            throw "Unknown exceptions";
        }
    }
    catch (...)
    {
        delete ref;
        ref = nullptr;
        delete[] data;
        data = nullptr;
        throw "Unknown exceptions";
    }
}

template <typename T>
Data<T>::Data(Data<T> &other)
{
    try
    {
        this->data = other.data;
        this->ref = other.ref;
        (*this->ref)++;
    }
    catch (...)
    {
        delete ref;
        ref = nullptr;
        delete[] data;
        data = nullptr;
        throw "Unknown exceptions";
    }
}

template <typename T>
Data<T> &Data<T>::operator=(const Data<T> &other)
{
    if (other == nullptr)
    {
        cout << "The equal data is empty!" << endl;
        return *this;
    }
    else if (other.data == nullptr)
    {
        cout << "The equal data is empty!" << endl;
        return *this;
    }
    else if (this == &other)
    {
        return *this;
    }
    else if (this == nullptr)
    {
        this->data = other.data;
        this->ref = other.ref;
        (*this->ref)++;
        return *this;
    }
    else
    {
        (*this->ref)--;
        if (*this->ref == 0)
        {
            try
            {
                delete[] this->data;
                this->data = nullptr;
                delete ref;
                this->ref = nullptr;
                cout << "Delete data successfully!" << endl;
            }
            catch (...)
            {
                cout << "delete error" << endl;
            }
        }
        this->data = other.data;
        this->ref = other.ref;
        (*this->ref)++;
        return *this;
    }
}

template <typename T>
Data<T>::~Data()
{
    (*this->ref)--;
    if (*this->ref == 0)
    {
        try
        {
            delete[] this->data;
            this->data = nullptr;
            delete ref;
            this->ref = nullptr;
            cout << "Delete data successfully!" << endl;
        }
        catch (...)
        {
            cout << "delete error" << endl;
        }
    }
    cout << "Data deconstructed!" << endl;
}