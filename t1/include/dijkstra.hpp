

#include <graph.hpp>
#include <queue>
#include <array_2d.hpp>


using dist_t =  std::pair<unsigned,unsigned>   ;
// Declaring a custom comparator
struct pair_comp {
    // Operator() overloading
    bool operator()(const dist_t& p1,
                    const dist_t& p2)
    {        
        return p1.first > p2.first;
    }
};

std::vector<unsigned> solveDijkstra(Graph& graph, unsigned start,unsigned end)
{


    std::priority_queue<dist_t, 
                        std::vector<dist_t>,
                        pair_comp> 
                        qt;

    std::vector<bool> visited(graph.nodes.size(),false);
    std::vector<int> distances(graph.nodes.size(),std::numeric_limits<int>::max());

    std::vector<unsigned> parent_node(graph.nodes.size(),0);
    std::vector<unsigned> path;


    unsigned start_node_index,end_node_index;
    bool find_start=false;
    bool find_end=false;

    for(unsigned i=0;i<graph.nodes.size();i++)
    {   
        if(graph.nodes.at(i).id == start)
        {
            start_node_index = i;
            find_start = true;
            // std::cout << "start node index : " << start_node_index << '\n';
        }
        if(graph.nodes.at(i).id == end)
        {
            end_node_index = i;
            find_end = true;
            // std::cout << "end node index : " << end_node_index << '\n';
        }
        if(find_end&&find_start) break;
    }
    if(!find_start){
        std::cerr << "Failed to find start node.\n";
        return path;
    }

    qt.push(dist_t(0,start_node_index));
    distances.at(start_node_index) = 0;
    parent_node.at(start_node_index) = start_node_index;


    while(!qt.empty())
    {   
        auto tmp = qt.top();
        qt.pop();


        // unsigned current_distance = tmp.first;
        unsigned current_node_index = tmp.second;

        if(visited.at(current_node_index)) continue;
        visited.at(current_node_index) = true;

        if(current_node_index == end_node_index)
            break;
        
        for(auto &edje: graph.nodes.at(current_node_index).edges)
        {   
            // std::cout << "distances.at(edje.destination) : " << distances.at(edje.destination) << '\n';
            if(visited.at(edje.destination)) continue;

            if( distances.at(current_node_index) + edje.weight < distances.at(edje.destination) )
            {
                distances.at(edje.destination) = distances.at(current_node_index) + edje.weight;
                parent_node.at(edje.destination) = current_node_index;

                qt.push(dist_t(distances.at(edje.destination),edje.destination));
            }
        }


    }
    if(find_end)
    {   
        if(visited.at(end_node_index))
        {
        unsigned path_node = end_node_index;
        while(path_node!=start_node_index)
        {
            path.push_back(path_node);
            // std::cout << " , " << path_node ;
            path_node = parent_node.at(path_node);
        }
        path.push_back(path_node);
        // std::cout << " , " << path_node << '\n';
        
        // std::cout << "distances.at(" <<  end_node_index << ") : " << distances.at(end_node_index) << '\n';
        }
        else
        {
            std::cout << "End node not visited.\n";

        }
    }
    else
        std::cout << "end_node_index not found \n";

    return path;
}


void drawPath(const Graph &graph,const std::vector<unsigned>& path, Array2d<char>* array)
{
    unsigned sx = graph.nodes.at(path.at(0)).id / array->cols;
    unsigned sy = graph.nodes.at(path.at(0)).id % array->cols;
    auto xy_pair = array->unmap(graph.nodes.at(path.at(0)).id);
    sx = xy_pair.first;
    sy = xy_pair.second;

    array->set(sx,sy,'G');
    sx = graph.nodes.at(path.back()).id / array->cols;
    sy = graph.nodes.at(path.back()).id % array->cols;
    array->set(sx,sy,'S');

    for(unsigned i=1;i<path.size()-1;i++)
    {   
        auto &node_index = path.at(i);
        unsigned node_id = graph.nodes.at(node_index).id;
        unsigned sx = node_id / array->cols;
        unsigned sy = node_id % array->cols;
        array->set(sx,sy,'#');
    }


}