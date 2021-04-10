
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#define SDL_MAIN_HANDLED
#include <SDL2\SDL.h>
#include <gl\glew.h>
#include <SDL2\SDL_opengl.h>
#include <gl\glu.h>

#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
"out vec4 vertexColor;\n" // specify a color output to the fragment shader
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // set the output variable to a dark-red color
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n" // the input variable from the vertex shader (same name and same type) 
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;

int main(int argc, char* args[])
{

    // Initialize SDL
    SDL_Init( SDL_INIT_VIDEO );

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Create window
    gWindow = SDL_CreateWindow( "NANDGIZMOS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );


    //Create context
    gContext = SDL_GL_CreateContext( gWindow );

    //Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    
    //Use Vsync
    SDL_GL_SetSwapInterval( 1 );


    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // only triangle
        -0.5f, -0.5f, 0.0f,  // left
         0.5f, -0.5f, 0.0f,  // right
         0.0f,  0.5f, 0.0f   // top
        // first triangle
        // -0.9f, -0.5f, 0.0f,  // left 
        // -0.0f, -0.5f, 0.0f,  // right
        // -0.45f, 0.5f, 0.0f,  // top 
        // second triangle
         // 0.0f, -0.5f, 0.0f,  // left
         // 0.9f, -0.5f, 0.0f,  // right
         // 0.45f, 0.5f, 0.0f   // top  
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;


    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT )
            {
                int x = 0, y = 0;
                SDL_GetMouseState( &x, &y );

                //Toggle quad
                if( e.text.text[0] == 'q' )
                {
                    // gRenderQuad = !gRenderQuad;
                }
            }
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0); // no need to unbind it every time 
        
        //Update screen
        SDL_GL_SwapWindow( gWindow );
    }

    //Disable text input
    SDL_StopTextInput();

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Destroy window    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

