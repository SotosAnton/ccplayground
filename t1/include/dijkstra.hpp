

#include <graph.hpp>
#include <queue>


using dist_t =  std::pair<unsigned,unsigned>   ;
// Declaring a custom comparator
struct pair_comp {
    // Operator() overloading
    bool operator()(const dist_t& p1,
                    const dist_t& p2)
    {        
        return p1.first < p2.first;
    }
};

void solveDijkstra(Graph& graph, unsigned start,unsigned end)
{


    std::priority_queue<dist_t, 
                        std::vector<dist_t>,
                        pair_comp> 
                        qt;

    std::vector<bool> visited(graph.nodes.size(),false);
    std::vector<int> distances(graph.nodes.size(),std::numeric_limits<int>::max());


    unsigned start_node_id,end_node_id;
    bool find_start=false;
    bool find_end=false;

    for(unsigned i=0;i<graph.nodes.size();i++)
    {   
         std::cout << "i : " << i << '\n';
        if(graph.nodes.at(i).id == start)
        {
            start_node_id = i;
            find_start = true;
            std::cout << "start_node_id : " << start_node_id << '\n';
        }
        if(graph.nodes.at(i).id == end)
        {
            end_node_id = i;
            find_end = true;
            std::cout << "find_end : " << start_node_id << '\n';
        }
        if(find_end&&find_start) break;
    }
    if(!find_start){
        std::cerr << "Failed to find start node.\n";
        return;
    }

    qt.push(dist_t(0,start_node_id));
    distances.at(start_node_id) = 0;


    while(!qt.empty())
    {   
        auto tmp = qt.top();
        qt.pop();


        // unsigned current_distance = tmp.first;
        unsigned current_node_id = tmp.second;
        std::cout << "current_node_index : " << current_node_id << " id : " << graph.nodes.at(current_node_id).id << '\n';


        if(visited.at(current_node_id)) continue;
        visited.at(current_node_id) = true;
        
        for(auto &edje: graph.nodes.at(current_node_id).edges)
        {   
            // std::cout << "distances.at(edje.destination) : " << distances.at(edje.destination) << '\n';
            if(visited.at(edje.destination)) continue;

            if( distances.at(edje.destination) > distances.at(current_node_id) + edje.weight )
            {
                distances.at(edje.destination) = distances.at(current_node_id) + edje.weight;

                qt.push(dist_t(distances.at(edje.destination),edje.destination));
            }
        }
    }
    if(find_end)
        std::cout << "distances.at(end_node_id) : " << distances.at(end_node_id) << '\n';
    else
        std::cout << "end_node_id not found \n";
}