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







int main (int argc, const char * argv[]) {
    // insert code here...

    if(argc != 3 ) 
    {
        std::cout << "Wrong arguments\n";
        return 0;
    }

    unsigned M = std::stoi(argv[1]);
    unsigned N = std::stoi(argv[2]);


    auto maze = std::make_shared<Maze>(M,N);
    // auto maze =  new Array2d<unsigned>(M,N);


    auto st = std::chrono::high_resolution_clock::now();
    // mazeBuildReccursive(maze.get());
    // mazeBuildVector(maze.get());
    mazeBuildMulti(maze.get(),2);
    auto elapsed_time = std::chrono::high_resolution_clock::now()-st;
    std::cout << " Maze generation time : " << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count() << "ms\n";

    st = std::chrono::high_resolution_clock::now();
    Graph graph;
    unsigned goal;
    mazeToGraphSmart(&graph,maze.get(),&goal);
    elapsed_time = std::chrono::high_resolution_clock::now()-st;
    std::cout << " Graph generation time : " << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed_time).count() << "ms\n";

    // maze->build();
    // maze->path->print();
    maze->print();
    

    solveDijkstra(graph,graph.nodes.at(0).id,goal);

    return 0;
}