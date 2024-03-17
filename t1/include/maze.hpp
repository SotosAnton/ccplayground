#ifndef MAZE_H
#define MAZE_H


#include <array_2d.hpp>
#include <utils.hpp>
#include <cstdlib>


struct Maze { 
    Maze(size_t cols,size_t rows)
    {        
        array = new Array2d<char>(cols*2+1,rows*2+1,wall_cell);
        path = new Array2d<u_int16_t>(cols,rows);
        
    }
    

    void print() const
    {
        array->print();
    }   


    ~Maze()
    {   
        // std::cout << "Maze destructor\n";
        if(array) delete array;
        if(path) delete path;
    }
   
    Array2d<char> *array;
    
    const char empty_cell = ' ';
    const char wall_cell = '@';
    const char goal_cell = 'X';

    Array2d<u_int16_t> *path;

    struct
    {
    const int16_t x[4] = { 0, 0, 1,-1};
    const int16_t y[4] = { 1,-1, 0, 0};
    } dir;

    size_t iter;

    struct 
    {
        int i;
        int j;
    } goal;
    


}; 

#endif