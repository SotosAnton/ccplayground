#ifndef GRAPH_H
#define GRAPH_H

#include <iostream> 
#include <vector> 
#include <maze.hpp> 
#include <stack_builder.hpp> 
#include <unordered_map>
#include <thread>


struct Edge
{   
    Edge(int d,int w):
    destination(d),
    weight(w)
    {};

    int destination;
    int weight;
};

struct Node
{   
    Node(int id):
    id(id)
    {};
    std::vector<Edge> edges;
    int id;
};

struct  Graph
{
    void insertEdge(int node_index,const Edge &edge)
    {
        nodes.at(node_index).edges.push_back(edge);
    }

    void insertNode(const int id)
    {
        nodes.push_back(Node(id));
    }

    std::vector<Node> nodes;
};


PointDir traversePath(const Maze *maze ,PointDir start,int * path_length, Array2d<char> * array_cp = nullptr);


void mazeToGraph(Graph* graph,Maze* maze)
{

    int i = 1;
    int j = 1;
    unsigned map_cords;

    std::stack<PointDir> st;
    std::unordered_map<unsigned, unsigned> visited_nodes;

    Array2d<char> *array = maze->array;
    Array2d<char> array_cp = *array;
    // Array2d<char> *array_h = &array_cp;

    
    st.push(PointDir(i,j,0,0));
    map_cords = i * array->cols + j;
    graph->insertNode(map_cords);
    visited_nodes[map_cords] = 0;
    

    while(!st.empty())
    {
        
        PointDir tmp = st.top();
        st.pop();

        i = tmp.i;
        j = tmp.j;

        unsigned map_cords = i * array->cols + j;

        size_t current_node = visited_nodes[map_cords];

        for(unsigned k=0;k<4;k++)
        {   
            

            int x_tmp = i + 2*maze->dir.x[k];
            int y_tmp = j + 2*maze->dir.y[k];
            
            map_cords = x_tmp * array->cols + y_tmp;

            if(array->valid(x_tmp , y_tmp))
            {   
                map_cords = x_tmp * array->cols + y_tmp ;
                
                int x_step = i + maze->dir.x[k];
                int y_step = j + maze->dir.y[k];

                if( array->get(x_step , y_step) == maze->empty_cell)
                {   
                    

                    if(!visited_nodes.contains(map_cords))
                    {   
                        graph->nodes.push_back(Node(map_cords));
                        visited_nodes[map_cords] =  graph->nodes.size() - 1;
                        st.push(PointDir(x_tmp,y_tmp,0,0));

                        // array_cp.set(x_tmp,y_tmp,'.' );
                        
                    }

                    // array_cp.set(x_step,y_step,'.' );
                    graph->nodes.at(current_node).edges.push_back(Edge(visited_nodes[map_cords],1));                    
                    
                    
                }
            
            }      
        
        }
    }
    // std::cout << "array_cp\n";


    // for(auto node: graph->nodes)
    // {   
    //     int sx = node.id / array_cp.cols;
    //     int sy = node.id % array_cp.cols;
    //     array_cp.set(sx,sy,'*' );

    //     for(auto edge: node.edges)
    //     {
    //         int tx = graph->nodes.at(edge.destination).id / array_cp.cols;
    //         int ty = graph->nodes.at(edge.destination).id % array_cp.cols;

    //         array_cp.set( (sx+tx)/2,(sy+ty)/2,'-' );
    //     array_h->print();
    //             std::this_thread::sleep_for(std::chrono::milliseconds(40));
    //     }
    // }
    // array_cp.print();
}


void mazeToGraphSmart(Graph* graph,const Maze* maze, unsigned *goal_id = nullptr)
{

    int i = 1;
    int j = 1;
    unsigned map_cords;

    std::stack<PointDir> st;
    std::unordered_map<unsigned, unsigned> visited_nodes;

    Array2d<char> *array = maze->array;
    Array2d<char> array_cp = *array;
    // Array2d<char> *array_h = &array_cp;

    
    st.push(PointDir(i,j,0,0));
    map_cords = i * array->cols + j;
    graph->insertNode(map_cords);
    visited_nodes[map_cords] = 0;
    

    while(!st.empty())
    {
        
        PointDir tmp = st.top();
        st.pop();

        i = tmp.i;
        j = tmp.j;

        unsigned map_cords = i * array->cols + j;

        size_t current_node = visited_nodes[map_cords];

        for(unsigned k=0;k<4;k++)
        {   
            

            int x_tmp = i + 2*maze->dir.x[k];
            int y_tmp = j + 2*maze->dir.y[k];
            
            map_cords = x_tmp * array->cols + y_tmp;

            if(array->valid(x_tmp , y_tmp))
            {   
                map_cords = x_tmp * array->cols + y_tmp ;
                
                int x_step = i + maze->dir.x[k];
                int y_step = j + maze->dir.y[k];

                if( array->get(x_step , y_step) == maze->empty_cell)
                {   
                    
                    int path_length = 1;
                    if(!visited_nodes.contains(map_cords))
                    {   
                        
                        PointDir end = traversePath(maze,PointDir(x_tmp,y_tmp,i,j),&path_length);

                        unsigned end_cords = end.i * array->cols + end.j;
                        map_cords = end_cords;
                        if( !visited_nodes.contains(end_cords) )
                        {
                            graph->nodes.push_back(Node(end_cords));
                            visited_nodes[end_cords] =  graph->nodes.size() - 1;
                            st.push(PointDir(end.i,end.j,0,0));
                        }
                        // array_cp.set(x_tmp,y_tmp,'.' );
                        if( array->get(x_tmp , y_tmp) == maze->goal_cell )
                        {   
                            std::cout << "Graph found goal : " << map_cords << ".\n";
                            if(goal_id) *goal_id = map_cords;
                        }
                    }

                    // array_cp.set(x_step,y_step,'.' );
                    graph->nodes.at(current_node).edges.push_back(Edge(visited_nodes[map_cords],path_length));
                    
                    
                }
            
            }      
        
        }
    }
    // std::cout << "array_cp\n";


    // for(auto node: graph->nodes)
    // {   
    //     int sx = node.id / array_cp.cols;
    //     int sy = node.id % array_cp.cols;
    //     array_cp.set(sx,sy,'*' );

    //     for(int i =0;i< node.edges.size();i++)
    //     {   
    //         Edge &edge= node.edges.at(i);
    //         int tx = graph->nodes.at(edge.destination).id / array_cp.cols;
    //         int ty = graph->nodes.at(edge.destination).id % array_cp.cols;    

    //         array_cp.set( tx,ty,'0'+edge.weight );
    //         array_cp.print();
    //         std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    //         array_cp.set( tx,ty,' ');   
        
    //     }
    // }
   
    // array_cp.print();
}


PointDir traversePath( const Maze *maze ,PointDir start, int * path_length , Array2d<char> * array_cp )
{

    int i = start.i;
    int j = start.j;

    int i_prev = start.x_dir;
    int j_prev = start.y_dir;


    while(true)
    {        
        unsigned free_count = 0;    
        int free_idx[4] = { 0, 0, 0,0};

        for(unsigned k=0;k<4;k++)
        {
            int x_tmp = i + 2*maze->dir.x[k];
            int y_tmp = j + 2*maze->dir.y[k];

            if( (x_tmp != i_prev || y_tmp != j_prev) && 
                    maze->array->valid(x_tmp , y_tmp) )
            {
                int x_step = i + maze->dir.x[k];
                int y_step = j + maze->dir.y[k];

                if( maze->array->get(x_step , y_step) == maze->empty_cell)
                {
                    free_idx[free_count] = k;
                    free_count++;
                }
            }
        }

        if(free_count == 0 )
        {   
            if(array_cp) array_cp->set( i,j,'d' );
            // i = i + 2*maze->dir.x[free_idx[0]];
            // j = j + 2*maze->dir.y[free_idx[0]];
            return PointDir(i,j,0,0);
        }
        else if(free_count == 1)
        {   
            // if(array_cp) 
            // {
            //     // array_cp->set( i,j,'-' );
            //     // array_cp->print();
            //     // std::this_thread::sleep_for(std::chrono::milliseconds(40));
            // }

            i_prev = i;
            j_prev = j;
            i = i + 2*maze->dir.x[free_idx[0]];
            j = j + 2*maze->dir.y[free_idx[0]];
            (*path_length) ++ ;


        }
        else
        {
            return PointDir(i,j,0,0);
        }

    }


}

#endif // GRAPH_H