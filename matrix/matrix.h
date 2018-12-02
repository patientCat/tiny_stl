#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>

// 练习 实现矩阵类
using namespace std;

template <typename T>

class Matrix 
{ 
public: 
    Matrix (int rows, int cols)
    : _array{ rows }
    {
        for(auto &x : _array)
            x.resize(cols);
    }

    Matrix (vector<vector<T>> v)
    : _array{ v }
    {}

    Matrix (vector<vector<T>> &&v)
    : _array{ v }
    {}

    const vector<T> & operator[](int row) const
    {
        return _array(row);
    };

    vector<T> & operator[](int row) 
    {
        return _array(row);
    };

    int numOfRows() const   {   return _array.size();   }
    int numOfCols() const   {   return numOfRows() ? _array[0].size() : 0;  }

    ~Matrix () = default;
     
private: 
    vector<vector<T>> _array;
}; 
#endif // matrix.h

