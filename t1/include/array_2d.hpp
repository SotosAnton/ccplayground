#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <iostream>
#include <memory>


template <typename T> 
struct Array2d { 
    
    Array2d(size_t rows, size_t cols, T fill) 
        : Array2d(rows, cols) // Delegate to the other constructor
    {
        // Initialize elements with the fill value
        for (size_t i = 0; i < elements; i++)
            array_ptr[i] = fill;
    }

    // Constructor without fill
    Array2d(size_t rows, size_t cols) {
        this->cols = cols;
        this->rows = rows;
        elements = cols * rows;
        array_ptr = static_cast<T*>(malloc(sizeof(T) * elements));
    }

    T& operator()(unsigned i, unsigned j)
    {
        if(!valid(i,j)) throw std::runtime_error("Array access out of bounds");
        return array_ptr[ map(i, j) ];
    }
    
    T& operator()(unsigned i, unsigned j) const
    {
        if(!valid(i,j)) throw std::runtime_error("Array access out of bounds");
        return array_ptr[ map(i, j) ];
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

    size_t map(size_t i,size_t j) const
    {
        return i * cols + j;
    }

    std::pair<size_t, size_t> unmap(size_t i) const
    {
        return std::pair<size_t, size_t>( i / cols, i % cols); 
    }

    T get(size_t i,size_t j) const
    {
        return array_ptr[ map(i, j) ];
    }
    
    void set(size_t i,size_t j,T value)
    {
        array_ptr[ map(i, j) ] = value;
    }
    
    void inc(size_t i,size_t j)
    {
        array_ptr[ map(i, j) ]++;
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

    std::pair<size_t,size_t> min() const
    {   
        T val;
        unsigned index;
        val = array_ptr[0];
        index = 0;
        for(unsigned i=0;i<rows;i++)
        {
            if(array_ptr[i] < val)
            {
                val = array_ptr[i];
                index = 1;
            }
        }

        return std::pair<size_t,size_t>(index/ cols, index % cols);
 
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