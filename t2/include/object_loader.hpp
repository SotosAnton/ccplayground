#pragma once

#include<vector>
#include<string>
#include <GL/gl.h>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <vboindexer.hpp>
#include <SDL_image.h>
#include <unordered_map>

namespace sa_
{

class Mesh
{   
    public:

    Mesh(
        std::vector<unsigned short> *indeces_extern,
        std::vector<glm::vec3>*vertices_extern,
        std::vector<glm::vec2>*uvs_extern,
        std::vector<glm::vec3>*normals_extern,
        std::vector<glm::vec3>*color_extern
        ):
    indeces_extern(indeces_extern),
    vertices_extern(vertices_extern),
    uvs_extern(uvs_extern),
    normals_extern(normals_extern),
    color_extern(color_extern)
    {
        Model = glm::mat4(1.0f);
    };

    struct Edge
    {   
        uint16_t i;
        uint16_t j;     
        float d0;
        

        Edge(uint32_t ij )
        {
            i = static_cast<uint16_t>(ij >> 16);
            // Extract the lower 16 bits (num2) using bitwise AND with 0xFFFF
            j = static_cast<uint16_t>(ij & 0xFFFF);
            d0  = 0;
        }
      
        Edge( uint16_t x, uint16_t y)
        {
            i = x;
            j = y;
            d0 = 0;
        }

        uint32_t combine()
        {
            uint32_t result = static_cast<uint32_t>(i) << 16;
            // OR the second number with the result to place it in the lower 16 bits
            result |= static_cast<uint32_t>(j);
            return result;
        }       
        
        uint32_t combineInv()
        {
            uint32_t result = static_cast<uint32_t>(j) << 16;
            // OR the second number with the result to place it in the lower 16 bits
            result |= static_cast<uint32_t>(i);
            return result;
        }       

    };

    ~Mesh(){};

    bool load(const std::string &path,float scale = 1.0f)
    {
        std::vector< unsigned short > vertexIndices, uvIndices, normalIndices;
        // std::vector< glm::vec3 > sim_vertices;
        sim_vertices.clear();
        std::vector< glm::vec2 > temp_uvs;
        std::vector< glm::vec3 > temp_normals;

        std::vector< glm::vec3 > out_vertices;
        std::vector< glm::vec2 > out_uvs;
        std::vector< glm::vec3 > out_normals;

        FILE * file = fopen(path.c_str(), "r");
        if( file == NULL )
        {
            printf("Impossible to open the file !\n");
            return false;
        }

        unsigned triangle[3][2] = {{0,1},{1,2},{2,0}};

        while( 1 ){
            
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.
            if ( strcmp( lineHeader, "v" ) == 0 ){
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                sim_vertices.push_back(vertex * scale );
            }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                glm::vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y );
                uv.x = uv.x;
                uv.y = 1-uv.y;
                temp_uvs.push_back(uv);
            }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                                    &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
                                    &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                    &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                    }
                    vertexIndices.push_back(vertexIndex[0]-1);
                    vertexIndices.push_back(vertexIndex[1]-1);
                    vertexIndices.push_back(vertexIndex[2]-1);
                    uvIndices    .push_back(uvIndex[0]-1);
                    uvIndices    .push_back(uvIndex[1]-1);
                    uvIndices    .push_back(uvIndex[2]-1);
                    normalIndices.push_back(normalIndex[0]-1);
                    normalIndices.push_back(normalIndex[1]-1);
                    normalIndices.push_back(normalIndex[2]-1);

                for(unsigned j=0;j<3;j ++)
                {
                    unsigned front = vertexIndex[triangle[j][0]]-1;
                    unsigned back = vertexIndex[triangle[j][1]]-1;

                    Edge test(front,back);                

                    if ( auto search1 = edges_map.find(test.combine()); search1 != edges_map.end())
                    {
                        edges_map[test.combine()]++;
                    }
                    else if ( auto search2 = edges_map.find(test.combineInv()) ; search2 != edges_map.end())
                    {
                        edges_map[test.combineInv()]++;
                    }
                    else
                    {   
                        
                        edges_map[test.combine()] = 1;
                        edges.push_back(Edge(front,back));
                        edges.back().d0 =  glm::length( sim_vertices.at(front) - sim_vertices.at(back) ) ;
                        std::cout << " edge : " << front << '-' << back << '\n';
                    }
                }

            }

        }

        // std::cout << "vertexIndices : " << vertexIndices.size() << '\n';
        // std::cout << "uvIndices : " << uvIndices.size() << '\n';
        // std::cout << "normalIndices : " << normalIndices.size() << '\n';

        start_idx = vertices_extern->size();
        // For each vertex of each triangle
        for( unsigned int i=0; i<vertexIndices.size(); i++ )
        {   
            
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];

            out_vertices.push_back(sim_vertices[vertexIndex]);
            out_uvs.push_back(temp_uvs[uvIndex]);
            out_normals.push_back(temp_normals[normalIndex]);

        }

        idx.clear();
        out_vertices_extern_origin.clear();

        indexVBO(&vertexIndices,
                  out_vertices,
                  out_uvs,
                  out_normals,
                 &idx,vertices_extern,
                 &out_vertices_extern_origin,
                  uvs_extern,normals_extern
                  );
        
        vel.assign(sim_vertices.size(),glm::vec3(0.0f,0.0f,0.0f));

        for(auto item:idx)
            indeces_extern->push_back(item);

        // for(auto &item:*vertices_extern)
        //     std::cout << item.x << ' ' << item.y << " " << item.z << '\n';       


        std::cout << "edges num : " << edges.size() << '\n';

        end_idx = vertices_extern->size() ;

        p_prev.clear();
        for(unsigned i=0;i<sim_vertices.size();i++)
        {   
            p_prev.push_back(sim_vertices.at(i)); 
        }

        std::cout << "Loaded mesh from : " << start_idx << '/' << end_idx << '\n'; 
        return true;
    }

    bool loadTexture(const std::string &path)
    {
        // SDL_Surface* surface = IMG_Load("/home/sotiris/spp/t2/dice_unwrap.png");
        surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
            return false;
        }   

        std::cout << "Loaded texture : " <<  surface->w << "X" << surface->h << '\n';

        

    }

    float constaint(float k,float d0,const glm::vec3 &x1,const glm::vec3 &x2)
    {
        return  k * (glm::length(x1-x2) - d0);
    }

    glm::vec3 constaintGrad(const glm::vec3 &x1, const glm::vec3 &x2)
    {   
        if( glm::length(x1-x2) < 1e-6)
        return glm::vec3(0.0f);
        return  glm::normalize(x1-x2);
    }

    void verify()
    {
        float correction_vector_sum = 0;
        for(unsigned i=0;i<edges.size();i++)
        { 
            unsigned id_1 = edges.at(i).i;
            unsigned id_2 = edges.at(i).j;

            auto &p1 = sim_vertices.at(id_1);
            auto &p2 = sim_vertices.at(id_2);
            float c_force = constaint(1,edges.at(i).d0,p1,p2);
            correction_vector_sum += c_force;           

        }

        std::cout << " init state : " << correction_vector_sum  / edges.size() << '\n';
    }

    void simulate(float dt)
    {
        float wi = 1.1f;
        float stiffness = 0.9f;
        glm::vec3 g = glm::vec3(0.0f,-3.81f,0.0f);
        // g.y = 0;

        for(unsigned i=0;i<sim_vertices.size();i++)
        {   
            vel.at(i) += dt * g ;
            sim_vertices.at(i) += dt * vel.at(i);
            // if( sim_vertices.at(i).y < 0.0 ) std::cout << "GROUND\n";
            sim_vertices.at(i).y =  sim_vertices.at(i).y < 0.0 ? 0.0 :sim_vertices.at(i).y;
        }

        unsigned sim_iter = 10;
        glm::vec3 correction_vector;
        double correction_vector_sum;

        for(unsigned i=0;i<sim_iter;i++)
        {   
            correction_vector_sum = 0;
            for(unsigned i=0;i<edges.size();i++)
            { 
                unsigned id_1 = edges.at(i).i;
                unsigned id_2 = edges.at(i).j;

                auto &p1 = sim_vertices.at(id_1);
                auto &p2 = sim_vertices.at(id_2);
                float c_force = constaint(stiffness,edges.at(i).d0,p1,p2);
                glm::vec3 c_grad = constaintGrad(p1,p2);

                correction_vector = -c_force * c_grad/ wi;   // eq. 1

                correction_vector_sum += glm::length(correction_vector);

                p1 += correction_vector / wi ;
                p2 -= correction_vector / wi ;

                p1.y =   p1.y < 0.0 ? 0.0 : p1.y;
                p2.y =   p2.y < 0.0 ? 0.0 : p2.y;

            }



            std::cout << " i :" << i << " err : " << correction_vector_sum  / edges.size() << '\n';
           
        }

        for(unsigned i=0;i<sim_vertices.size();i++)
        {   
            vel.at(i) = (sim_vertices.at(i) - p_prev.at(i))/dt;
            p_prev.at(i) = sim_vertices.at(i);
        }

        for(unsigned i=start_idx;i<end_idx;i++)
        {   
            unsigned j = i - start_idx;
            vertices_extern->at(i) = sim_vertices.at( out_vertices_extern_origin.at(j));
                        // std::cout << " out_vertices_extern_origin " << j << '/'<< out_vertices_extern_origin.size() << " = " << out_vertices_extern_origin.at(j)  << '\n';

        }
    }

    void* getTexture() { return surface->pixels;};

    void setAffine( glm::mat4 affine )
    {
        Model = affine;

        for(unsigned i = start_idx ; i < end_idx;i++)
        {
            vertices_extern->at(i) = glm::vec3 (affine * glm::vec4(vertices_extern->at(i), 1.0f) );
        }
        for(unsigned i = 0 ; i < sim_vertices.size();i++)
        {
            sim_vertices.at(i) = glm::vec3 (affine * glm::vec4(sim_vertices.at(i), 1.0f) );
            p_prev.at(i) = glm::vec3 (affine * glm::vec4(p_prev.at(i), 1.0f) );
        }
    }
    
    SDL_Surface* surface;
    std::vector<short unsigned int> idx;
    GLuint textureID;

    private:

    glm::mat4 Model;

    unsigned start_idx;
    unsigned end_idx;

    std::vector<unsigned short> out_vertices_extern_origin;
    std::vector< glm::vec3 > sim_vertices;
    std::vector < unsigned short > *indeces_extern;
    std::vector < glm::vec3 > *vertices_extern;
    std::vector < glm::vec2 > *uvs_extern;
    std::vector < glm::vec3 > *normals_extern;
    std::vector < glm::vec3 > *color_extern;

    std::vector<glm::vec3> vel ;
    std::vector<glm::vec3> p_prev ;

    std::unordered_map< uint32_t , uint32_t > edges_map;
    std::vector< Edge > edges;



};

}