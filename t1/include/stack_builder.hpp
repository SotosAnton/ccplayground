#ifndef MAZE_BUILDER_STACK_H
#define MAZE_BUILDER_STACK_H

#include <maze.hpp>
#include <stack>
#include <cmath>
#include <algorithm>




struct PointDir
{   
    PointDir(int i,int j,int x_dir,int y_dir):
    i(i),
    j(j),
    x_dir(x_dir),
    y_dir(y_dir)
    {};

    int i,j,x_dir,y_dir;

};


void mazeBuildStack(Maze * maze)
{
    std::stack<PointDir> st;

    st.push(PointDir(0,0,0,0));

    size_t iter = 1;  

    while(!st.empty())
    {   
        // std::cout << " st : " << st.size() << "\n";
        PointDir cur_point = st.top(); st.pop();       
     
        int i = cur_point.i;
        int j = cur_point.j;     

        if(maze->path->get(i,j) != 0) 
            continue;

        maze->array->set(i*2+1,j*2+1,maze->empty_cell);          
        maze->array->set(i*2+1-cur_point.x_dir,j*2+1-cur_point.y_dir,maze->empty_cell);            

        if(iter == maze->path->elements)
            maze->array->set(i*2+1,j*2+1,maze->goal_cell);
            
        maze->path->set(i,j,iter);

        iter++;    
        unsigned free_count = 0;
        
        int free_idx[4] = { 0, 0, 0,0};

        for(unsigned k=0;k<4;k++)
        {   
            int x_tmp = i + maze->dir.x[k];
            int y_tmp = j + maze->dir.y[k];

            if(maze->path->valid(x_tmp , y_tmp) &&
                ( maze->path->get(x_tmp , y_tmp)) == 0)
                {                

                free_idx[free_count] = k;
                free_count++;
                }
        }

        if(free_count > 0)
        {
            if(free_count > 1)
                shuffleArray(free_idx,free_count);

            for(unsigned k=0;k<free_count;k++)
            {   
                size_t dir_idx = free_idx[k];   
                int x_tmp = i + maze->dir.x[dir_idx];
                int y_tmp = j + maze->dir.y[dir_idx];           
                
                if(maze->path->get( x_tmp,y_tmp) == 0) 
                {
                    st.push(PointDir(x_tmp,y_tmp,maze->dir.x[dir_idx],maze->dir.y[dir_idx]));
                }
                
            }
        }

        
       
        // maze->array->print();
        // char xdd;
        // std::cin >> xdd;

        }    
}

void mazeBuildVector(Maze * maze)
{
    std::vector<PointDir> st;

    st.push_back(PointDir(0,0,0,0));
    st.push_back(PointDir(maze->path->rows-1,maze->path->cols-1,0,0));

    size_t iter = 1;

    while(!st.empty())
    {   
        // std::cout << " st : " << st.size() << "\n";

        int random_start = rng() % (st.size());
        std::swap(st.at(random_start),st.back());

        PointDir cur_point = st.back();
        st.pop_back();
     
        int i = cur_point.i;
        int j = cur_point.j;     

        if(maze->path->get(i,j) != 0) 
            continue;

        maze->array->set(i*2+1,j*2+1,maze->empty_cell);          
        maze->array->set(i*2+1-cur_point.x_dir,j*2+1-cur_point.y_dir,maze->empty_cell);            

        if(iter == maze->path->elements)
            maze->array->set(i*2+1,j*2+1,maze->goal_cell);

        
        maze->path->inc(i,j);

        if(maze->path->get(i,j) > 1) 
            continue;

        iter++;    
        unsigned free_count = 0;
        
        int free_idx[4] = { 0, 0, 0,0};

        for(unsigned k=0;k<4;k++)
        {   
            int x_tmp = i + maze->dir.x[k];
            int y_tmp = j + maze->dir.y[k];        

            if(maze->path->valid(x_tmp , y_tmp))
            {   
                if( maze->path->get(x_tmp , y_tmp) == 0)
                {                

                free_idx[free_count] = k;
                free_count++;
                }
            }
        }

        if(free_count > 0)
        {
            if(free_count > 1)
                shuffleArray(free_idx,free_count);

            for(unsigned k=0;k<free_count;k++)
            {   
                size_t dir_idx = free_idx[k];   
                int x_tmp = i + maze->dir.x[dir_idx];
                int y_tmp = j + maze->dir.y[dir_idx];           
                
                if(maze->path->get( x_tmp,y_tmp) == 0) 
                {
                    st.push_back(PointDir(x_tmp,y_tmp,maze->dir.x[dir_idx],maze->dir.y[dir_idx]));
                }
                
            }
            

        }        
       
        // maze->array->print();
        // char xdd;
        // std::cin >> xdd;

        }    
}

#endif