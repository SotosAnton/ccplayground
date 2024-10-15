#ifndef MAZE_BUILDER_MULTI_H
#define MAZE_BUILDER_MULTI_H

#include <maze.hpp>
#include <stack>
#include <cmath>
#include <algorithm>

double rand_float()  {
    return ( ( double )rng() ) / ( double )rng.max() ;
}

void mazeBuildMulti(Maze * maze , unsigned n_starts = 4)
{
    srand(rng()^time(NULL));

    std::vector<std::vector<PointDir>> st;
    std::vector<std::vector<int>> contact_lists;
    

    Array2d<u_int16_t> * path = maze->path;
    Array2d<char> * array = maze->array;

    

    size_t n_starts_e =  (size_t) (std::ceil( std::sqrt((double) n_starts)));
    // int n_starts_square =  std::pow(n_starts_e,2);

    double step_x = path->rows / n_starts_e;
    double step_y = path->cols / n_starts_e;

    // std::cout << "n_starts_square : " << n_starts_square << '\n';

   
    for(unsigned i=0;i<n_starts;i++)
    {   
        int grid_x = i / n_starts_e;
        int grid_y = i % n_starts_e;



        size_t x_rand = grid_x * step_x + rand_float() * step_x;
        size_t y_rand = grid_y * step_y + rand_float() * step_y;

        contact_lists.push_back( std::vector<int>(n_starts,0) );
        contact_lists.back().at(i) = 1;
        st.push_back(std::vector<PointDir>());
        st.at(i).push_back( PointDir(x_rand,y_rand,0,0));
        // std::cout << "grid  : " << grid_x * step_x <<'<'<<x_rand  << "<"<< (grid_x+1) * step_x << "  , "
                                // << grid_y * step_y << "<" << y_rand << '<' << (grid_y+1) * step_y << '\n';
    }

    unsigned iter = 0;
    unsigned conctacts = 0;

    while(true)
    {   
        bool all_empty = true;
        for(size_t start_idx=0;start_idx<n_starts;start_idx++)
        {   
            std::vector<PointDir> &sub_path = st.at(start_idx);
            std::vector<int> &contact_list = contact_lists.at(start_idx);

            all_empty = all_empty && sub_path.empty();
            if(!sub_path.empty())
            {
                PointDir cur_point = sub_path.back();
                sub_path.pop_back();

                int i = cur_point.i;
                int j = cur_point.j;     

                if(path->valid(i,j) && path->get(i,j) != 0) 
                    continue;

                iter++;

                path->set(i,j,start_idx+1);

                array->set(i*2+1,j*2+1,maze->empty_cell);
                array->set(i*2+1-cur_point.x_dir,j*2+1-cur_point.y_dir,maze->empty_cell);

                if(iter == path->elements)
                {
                    maze->goal.i = i*2+1;
                    maze->goal.j = j*2+1;
                    array->set(i*2+1,j*2+1,maze->goal_cell);
                }

                int free_idx[4] = { 0, 0, 0,0};
                unsigned free_count = 0;

                for(unsigned k=0;k<4;k++)
                {   
                    int x_tmp = i + maze->dir.x[k];
                    int y_tmp = j + maze->dir.y[k];        

                    if(path->valid(x_tmp , y_tmp))
                    {   
                        if( path->get(x_tmp , y_tmp) == 0)
                        {
                            free_idx[free_count] = k;
                            free_count++;
                        }
                        else if(contact_list.at(path->get(x_tmp , y_tmp)-1) == 0 )
                        {   
                            conctacts++;
                            contact_list.at(path->get(x_tmp , y_tmp)-1)++;
                            array->set(i*2+1+maze->dir.x[k],j*2+1+ maze->dir.y[k],maze->empty_cell);
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
                        sub_path.push_back(PointDir(x_tmp,y_tmp,maze->dir.x[dir_idx],maze->dir.y[dir_idx]));                        
                        
                    }      

                }
            }
        }     

            if(all_empty)
                break;       
       

    }
    std::cout << "Contacts : " << conctacts << '\n';
}

#endif //MAZE_BUILDER_MULTI_H