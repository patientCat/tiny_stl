#ifndef _Vector_hh_
#define _Vector_hh_

#include <iostream> 
#include <algorithm> 

#define SPARE_CAPACITY 3
template <typename T>
class Vector 
{ 
public: 
    explicit Vector (size_t initSize = 0)
    : _size{initSize}
    , _capacity{initSize + SPARE_CAPACITY}
    {
        _data = new T[_capacity]{};
    }

    ~Vector ()
    {
        if(_data)
            delete []_data;
    }

    Vector(const Vector &rhs); // 这里要清楚const和&缺一不可
    Vector(Vector && rhs);

    Vector & operator=(const Vector &rhs); // 这里返回值Vector<T>可以省略
    Vector & operator=(Vector &rhs);
     
    void resize(int newSize)
    {
        if(newSize > _size)
            reserve(2 * newSize);
        _size = newSize;
    }

    void reserve(int newCapacity)
    {
        if(newCapacity < _size)  // 错误判断
            return;
        
        T * newData = new T[newCapacity]; // 源数据移动到新的更大的连续内存块上
        for(int i = 0; i < newCapacity; ++i)
            newData[i] = std::move(_data[i]); 

        std::swap(newData, _data); // 移花接木
        _capacity = newCapacity;
        delete []newData; 
    }

    T & operator[](size_t index)
    { return _data[index]; }

    const T & operator[](size_t index) const
    {   return _data[index];    }

    bool empty()const  {   return !_size;  }
    size_t size()const {   return _size;    }
    size_t capacity()const  {   return _capacity;   }

    void push_back(const T &x)
    {
        if(_size > _capacity)
            reserve(2 * _capacity);
        _data[_size++] = x;
    }

    void push_back(T && x)
    {
        if(_size > _capacity)
            reserve(2 * _capacity);
        _data[_size++] = std::move(x);
    }

    void pop_back()
    {
        --_size;
    }

    using iterator = T *;
    using const_iterator = const T *;

    iterator begin()    {   return &_data[0];   }
    const_iterator begin() const    {   return &_data[0];   }

    iterator end()  {   return &_data[size()];  }
    const_iterator end() const  {   return &_data[size()];  }
private: 
    void copyFrom(const T *rhs);
private: 
    T * _data;
    size_t _capacity;
    size_t _size; 
}; 

// 拷贝构造函数
template <typename T>
Vector<T>::Vector(const Vector &rhs)
: _data{nullptr}
, _capacity{rhs._capacity}
, _size{rhs._size}
{
    _data = new T[_capacity];
    copyFrom(rhs._data);
}

// 移动拷贝
template <typename T>
Vector<T>::Vector(Vector && rhs)
: _data {rhs._data}
, _capacity {rhs._capacity}
, _size {rhs._size}
{
    rhs._data = nullptr;
    rhs._capacity = 0;
    rhs._size = 0;
}

template <typename T>
Vector<T> & Vector<T>::operator=(const Vector &rhs)
{
    if(this != this)
    {
        auto removed = _data;
        _data = new T[rhs._capacity];
        copyFrom(rhs._data);
        _size = rhs._size;
        _capacity = rhs._capacity;
        delete []removed;
    }
    return *this;
}

template <typename T>
Vector<T> & Vector<T>::operator=(Vector &rhs)
{
    std::swap(rhs._data, _data); 
    std::swap(rhs._capacity, _capacity); 
    std::swap(rhs._size, _size); 
    return *this;
}
#endif // Vector.hh
