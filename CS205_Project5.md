# CS205 C/ C++ Programming - Project5

**Name：王启帆（WangQifan）**</br>
**SID: 12110206**

## Part-1 Analysis

&emsp;&emsp;在此project中，我们需要用C++语言设计一个矩阵运算的库，矩阵用类来构建，所类成员函数包括，构造函数，复制构造函数，析构函数，加法重载，减法重载，赋值重载，乘法重载与优化，ROI区域，和一个输出流的重载。</br>
&emsp;&emsp;这次project的方法比较相似，一开始进行传入数据合法性的判断，根据不同的运算要求做相应的处理，最后输出数据。</br>
&emsp;&emsp;这次project中最主要的一点就是构造了新的一个Data类来辅助进行soft copy，其中最重要的是有一个代表引用次数的计数器和所指的数据。</br>
&emsp;&emsp;这次project中另一点是对于异常处理的应用，在几个构造函数中用到了异常处理，并不断向上抛出，保证每一级可以处理数据，以防出现数据泄露。</br>

## Part-2 Code

**<font color = red>github网址：</font>** git@github.com:TheWeaponLaw/CS205_Project5.git

### Data类

&emsp;&emsp; Data类中有两个private成员变量，ref计数器和data所储存的数据，同时含有构造函数，析构函数，复制函数，和一个取数据地址的函数。取数据地址的函数是为了方面进行矩阵之间的算数操作。

```Cpp
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
```

&emsp;&emsp;其中在每次做好计数器的加与减并在每次减后，判断是否为0，为零则释放储存数据的空间，以赋值函数为例

```CPP
template <typename T>
Data<T> &Data<T>::operator=(const Data<T> &other)
{
    if (other.data == nullptr)
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

```

### 异常处理

&emsp;&emsp; 关于异常处理部分，主要分为抛出异常和不抛出异常处理，因为抛出异常并不是越多越好，抛出异常意味着强制退出当前函数，并要上一级处理，还会降低运行速度。有些没有必要的地方可以不抛出异常，所以我在此project中选择部分地方输出错误信息。主要抛出异常的地方在于构造器部分，因为构造器如果不用抛出异常打断的话，程序会默认已经生成了一个对象，这与用户所希望的不同，如果创建对象的值是非法值或创建过程中出错，如列为0，数据指针为空，申请空间失败，则抛出异常，并在每次抛出时进行数据处理，把每一级申请的空间释放等。最后会输出到主函数中，主函数打印出错误。至于其它部分，抛出异常后程序会自动启动逆析构函数，将其它部分析构掉。这样，当有异常值输入的时候，创建对象无法生成。</br>
&emsp;&emsp; 至于其它部分合法性的处理则与Project3判断条件差不多。</br>
&emsp;&emsp; 以矩阵的创建为例,调用了Data类的创建，矩阵的创建

```Cpp
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

int main() {
    try {
        
    }
    catch (const char *msg) {
        cout << msg << endl;
    }
}
```

### ROI

&emsp;&emsp; ROI部分是作为取一部分的父矩阵作为新的矩阵，进行数据的操作。在原有的矩阵基础上加入了start小矩阵取大矩阵的第一位，step大矩阵列数和小矩阵的差，方便作换行操作，par_size，大矩阵一个channel的大小，方便处理完一个channel后的换channel。

```Cpp
private:
    size_t row;
    size_t col;
    size_t channel;
    size_t start = 0;
    size_t step = 0;
    size_t par_size = 0;
    Data<T> *data = nullptr;
```

&emsp;&emsp;取ROI的操作是在原有的基础上，取两个点作为小图片的左上角和右下角，并算出小图片的各个值，包括row,col,step,par_size,start。在方法重载时也将这些值带入，对于原矩阵来说step，start皆为0；

```Cpp
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
```

### 乘法的转化

&emsp;&emsp;因为Project4中做的SIMD只适用于double,float和int，所以为了满足所有数据类型，只做openmp和ikj的转换

```Cpp

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
#pragma omp parallel for
        T *data_res = matrix_res->data->getData();
        T *data1 = this->data->getData() + start;
        T *data2 = matrix.data->getData() + start;
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

```

### 其它的重载运算符

&emsp;&emsp;“=”的重载主要是合法性判断和成员变量的赋值，主要的Data数据的变化，在Data类内部实现

```Cpp
template <typename T>//Data 类中的实现
Data<T> &Data<T>::operator=(const Data<T> &other)
{
    if (other.data == nullptr)
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
```

## 结果验证

