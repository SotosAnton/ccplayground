#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <maze.hpp>
#include <reccursive_builder.hpp>
#include <stack_builder.hpp>
#include <multi_path_builder.hpp>
#include <graph.hpp>
#include <dijkstra.hpp>
#include <numeric>
#include <random>




std::mt19937 rng;


int main (int argc, const char * argv[]) {
    // insert code here...

    if(argc != 3 ) 
    {
        std::cout << "Wrong arguments\n";
        return 0;
    }

    unsigned M = std::stoi(argv[1]);
    unsigned N = std::stoi(argv[2]);

    unsigned iter = 30;

    std::vector<int> time_maze_gen;
    std::vector<int> time_graph_gen;
    std::vector<int> time_solve;

    rng.seed(time(NULL));


    for(unsigned i=0;i<iter;i++)
    {
        auto maze = std::make_shared<Maze>(M,N);
        // auto maze =  new Array2d<unsigned>(M,N);


        auto st = std::chrono::high_resolution_clock::now();
        // mazeBuildReccursive(maze.get());
        // mazeBuildVector(maze.get());
        mazeBuildMulti(maze.get(),4);
        auto elapsed_time = std::chrono::high_resolution_clock::now()-st;
        // std::cout << " Maze generation time : " << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count() << "ms\n";
        time_maze_gen.push_back(std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count());
        
        st = std::chrono::high_resolution_clock::now();
        Graph graph;
        unsigned goal;

        // mazeBuildMulti(maze.get(),8);
        srand(time(NULL));

        mazeToGraphSmart(&graph,maze.get(),&goal);
        elapsed_time = std::chrono::high_resolution_clock::now()-st;
        // std::cout << " Graph generation time : " << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count() << "ms\n";
        time_graph_gen.push_back(std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count());

        // maze->build();
        // maze->path->print();
        
        st = std::chrono::high_resolution_clock::now();
        auto path = solveDijkstra(graph,graph.nodes.at(0).id,goal);


        elapsed_time = std::chrono::high_resolution_clock::now()-st;
        // std::cout << " Dijkstra time : " << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count() << "ms\n";
        time_solve.push_back(std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count());

        // drawPath(graph,path,maze->array);
        // maze->printColor();
    }

    auto const count = static_cast<float>(time_solve.size());
    auto gen_time = std::reduce(time_maze_gen.begin(), time_maze_gen.end()) / count;
    std::cout << "Average gen time : " <<  gen_time  << '\n';
    auto graph_time = std::reduce(time_graph_gen.begin(), time_graph_gen.end()) / count;
    std::cout << "Average graph time : " <<  graph_time  << '\n';
    auto solve_time = std::reduce(time_solve.begin(), time_solve.end()) / count;
    std::cout << "Average solve time : " <<  solve_time  << '\n';

    return 0;
}