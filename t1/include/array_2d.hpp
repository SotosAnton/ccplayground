#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <iostream>
#include <memory>


template <typename T> 
struct Array2d { 
    Array2d(size_t rows,size_t cols,T fill = 0)
    {
        this->cols = cols;
        this->rows = rows;
        elements = cols*rows;
        array_ptr = (T*) malloc(sizeof(T)*elements);
        // std::cout << "Array2d constructor\n";

        for(size_t i=0; i < elements;i++)
            array_ptr[i] = fill;

    }

    Array2d(Array2d& t)
    {
        this->cols = t.cols;
        this->rows = t.rows;
        this->elements = cols*rows;
        array_ptr = (T*) malloc(sizeof(T)*elements);
        
        for(size_t i=0; i < elements;i++)
            array_ptr[i] = t.array_ptr[i];

    }

    T get(size_t i,size_t j) const
    {
        return array_ptr[ i * cols + j ];
    }
    
    void set(size_t i,size_t j,T value)
    {
        array_ptr[ i * cols + j ] = value;
    }
    
    void inc(size_t i,size_t j)
    {
        array_ptr[ i * cols + j ]++;
    }

    void print() const
    {
        for(size_t i=0;i<rows;i++)
        {
            for(size_t j=0;j<cols;j++)
            {
                std::cout << get(i,j) << " ";
            }
            std::cout << "\b \n";
        }
    }

    bool valid(int i,int j) const
    {
        return( i >= 0 && j>=0 && i < rows && j < cols);
    }




    ~Array2d()
    {   
        // std::cout << "Array2d destructor\n";
        if(array_ptr) free(array_ptr);
    }
    size_t cols,rows,elements; 
    T *array_ptr; 
}; 

#endif