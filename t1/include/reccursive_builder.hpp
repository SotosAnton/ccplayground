#ifndef MAZE_BUILDER_REC_H
#define MAZE_BUILDER_REC_H

#include <maze.hpp>

void random_depth_search(Maze * maze,size_t *iter,int i,int j)
{   
   
    maze->path->set(i,j,*iter);

    (*iter)++;    
    unsigned free_count = 0;
    
    int free_idx[4] = { 0, 0, 0,0};

    for(unsigned k=0;k<4;k++)
    {   
        int x_tmp = i + maze->dir.x[k];
        int y_tmp = j + maze->dir.y[k];

        if(maze->path->valid(x_tmp , y_tmp) &&
            ( maze->path->get(x_tmp , y_tmp)) == 0)
            {
            
            // std::cout << "test i: " << x_tmp << " , J;" << y_tmp << " v: " <<  maze->path->get( x_tmp , y_tmp) << '\n';

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
            
            if(maze->path->get(x_tmp,y_tmp) == 0) 
            {   
                maze->array->set(i*2+1,j*2+1,maze->empty_cell);
                maze->array->set(i*2+1+maze->dir.x[dir_idx],j*2+1+maze->dir.y[dir_idx],maze->empty_cell);
                maze->array->set(x_tmp*2+1,y_tmp*2+1,maze->empty_cell);
                
                if(*iter == maze->path->elements)
                {
                    maze->array->set(x_tmp*2+1,y_tmp*2+1,maze->goal_cell);                                    
                    maze->goal.i = x_tmp*2+1;
                    maze->goal.j = y_tmp*2+1;
                    
                }

                random_depth_search(maze,iter,x_tmp,y_tmp);
            }
            
        }
    }
}


void mazeBuildReccursive(Maze * maze)
{   
    size_t iter = 1;
    random_depth_search(maze,&iter,0,0);
}


#endif