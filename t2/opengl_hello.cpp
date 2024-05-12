/*
    Minimal SDL2 + OpenGL3 example.

    Author: https://github.com/koute

    This file is in the public domain; you can do whatever you want with it.
    In case the concept of public domain doesn't exist in your jurisdiction
    you can also use this code under the terms of Creative Commons CC0 license,
    either version 1.0 or (at your option) any later version; for details see:
        http://creativecommons.org/publicdomain/zero/1.0/

    This software is distributed without any warranty whatsoever.

    Compile and run with: gcc opengl3_hello.c `sdl2-config --libs --cflags` -lGL -Wall && ./a.out
*/

#define GL_GLEXT_PROTOTYPES
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#include <SDL.h>
#include <SDL_opengl.h>
#include <compile_shaders.hpp>
#include <data.hpp>
#include <object_loader.hpp>
#include "vboindexer.hpp"
// #include <SDL2/SDL_image.h>
#include <SDL_image.h>

#include <memory>

#include <stdio.h>
#include <cstring>

typedef float t_mat4x4[16];


int main( int argc, char * argv[] )
{
    SDL_Init( SDL_INIT_VIDEO );
    // SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    // SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    // SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    std::vector<unsigned short> indeces_extern;
    std::vector < glm::vec3 > vertices_extern;
    std::vector < glm::vec2 > uvs_extern;
    std::vector < glm::vec3 > normals_extern;
    std::vector < glm::vec3 > color_extern;

    std::vector<std::shared_ptr<sa_::Mesh>> objects;

    objects.push_back(std::make_shared<sa_::Mesh>(&indeces_extern, &vertices_extern,&uvs_extern,&normals_extern,&color_extern));
    objects.push_back(std::make_shared<sa_::Mesh>(&indeces_extern, &vertices_extern,&uvs_extern,&normals_extern,&color_extern));
    

    // object.load("/home/sotiris/spp/t2/dice.obj");
    objects.at(0)->load("/home/sotiris/spp/t2/duck/duck.obj");
    // objects.at(0)->load("/home/sotiris/spp/t2/dice.obj",0.05);
    objects.at(1)->load("/home/sotiris/spp/t2/floor/floor.obj");

    objects.at(0)->loadTexture("/home/sotiris/spp/t2/duck/duck.jpg");
    objects.at(1)->loadTexture("/home/sotiris/spp/t2/floor/floor.jpg");


    std::cout << " indeces_extern : " << indeces_extern.size() << '\n';
    std::cout << " vertices_extern : " << vertices_extern.size() << '\n';
    std::cout << " uvs_extern : " << uvs_extern.size() << '\n';
    std::cout << " normals_extern : " << normals_extern.size() << '\n';
    
    static const int width = 1024;
    static const int height = 1024;

    SDL_Window * window = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    SDL_GLContext context = SDL_GL_CreateContext( window );

    GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    std::string vertex_file_path = "/home/sotiris/spp/t2/shaders/vertex_uv";
    std::string fragment_file_path = "/home/sotiris/spp/t2/shaders/fragment_uv";
    GLuint programID = LoadShaders( vertex_file_path, fragment_file_path );

   
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glViewport( 0, 0, width, height );

    glUseProgram( programID );    

    GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices_extern.size() * sizeof(decltype(vertices_extern)::value_type), vertices_extern.data(), GL_STATIC_DRAW);

   
	GLuint uv_buffer;
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs_extern.size() * sizeof(decltype(uvs_extern)::value_type), uvs_extern.data(), GL_STATIC_DRAW);
    

    // Create one OpenGL texture
    // objects.at(0)->textureID;
    glGenTextures(1, &objects.at(0)->textureID);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, objects.at(0)->textureID);   

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, objects.at(0)->surface->w, objects.at(0)->surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, objects.at(0)->getTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Create one OpenGL texture
    // objects.at(1)->textureID;
    glGenTextures(1, &objects.at(1)->textureID);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, objects.at(1)->textureID);   

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, objects.at(1)->surface->w, objects.at(1)->surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, objects.at(1)->getTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);


    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces_extern.size() * sizeof(decltype(indeces_extern)::value_type), &indeces_extern[0], GL_STATIC_DRAW);

   // Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 1.1f, 100.0f);

    // Or, for an ortho camera:
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(1,1,1), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 Model_b = glm::mat4(1.0f);
    // Our ModelViewProjection: multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    glUniformMatrix4fv( glGetUniformLocation( programID, "MVP" ), 1, GL_FALSE, &mvp[0][0] );
    

    auto rot_step = glm::rotate(glm::mat4(1.0f), 0.000f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto rot_step_B = glm::rotate(glm::mat4(1.0f), -0.000f, glm::vec3(0.0f, 0.0f, 1.0f));
    rot_step_B = glm::translate(rot_step_B,glm::vec3(0.0f, 0.0f, -0.0000f));
    rot_step = glm::translate(rot_step,glm::vec3(0.0f, 0.1f, 0.0000f));

    objects.at(0)->setAffine(rot_step *Model);
    objects.at(0)->verify();

    for( ;; )
    {   
        // std::cout << " setAffine : " <<  2 << "\n";
        
        // objects.at(1)->setAffine(Model_b);

        objects.at(0)->simulate(0.001f);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        void* mappedData = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        std::memcpy(mappedData, vertices_extern.data(), vertices_extern.size() * sizeof(decltype(vertices_extern)::value_type));

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUnmapBuffer(GL_ARRAY_BUFFER);


        // glClear( GL_COLOR_BUFFER_BIT );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Model = Model * rot_step;
        Model_b = Model_b * rot_step_B;
        mvp = Projection * View ;
        glUniformMatrix4fv( glGetUniformLocation( programID, "MVP" ), 1, GL_FALSE, &mvp[0][0] );


        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_KEYUP:
                    if( event.key.keysym.sym == SDLK_ESCAPE )
                        return 0;
                    break;
            }
        }

        // 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,uv_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_TRUE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        // glDrawArrays( GL_TRIANGLES, 0, g_vertex_data_s.size()/3 );

        for( auto object : objects)
        {   
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->idx.size() * sizeof(decltype(indeces_extern)::value_type), &object->idx[0], GL_STATIC_DRAW);

            // Draw the triangles !

            glBindTexture(GL_TEXTURE_2D, object->textureID);   


            glDrawElements(
                GL_TRIANGLES,      // mode
                object->idx.size(),    // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0    // element array buffer offset
            );
            // glDisableVertexAttribArray(0);
            // glDisableVertexAttribArray(1);
        }

        SDL_GL_SwapWindow( window );
        SDL_Delay( 10 );
        // std::cout << " t1\n";
    }

    SDL_GL_DeleteContext( context );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
