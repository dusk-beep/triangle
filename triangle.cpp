#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>

//TODO
//modularize
//shader files

//function defn sdl
//


//datatyoe for SDL_Window
struct sdl_t {
  SDL_Window* window;
  SDL_GLContext glContext;
};

//enum to represent running state 
enum state {QUIT,RUNNING,PAUSED};

static int compileShader(const std::string source,unsigned int type) {

  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id,1,&src,nullptr);
  glCompileShader(id);
  
  int result;
  glGetShaderiv(id,GL_COMPILE_STATUS,&result);

  if(result == GL_FALSE) {
    int length;
    glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
    char* messsage = (char*) alloca(length* sizeof(char));
    glGetShaderInfoLog(id,length,&length,messsage);
    std::cout << "failed to compike shader" <<std::endl;
    std::cout << messsage <<std::endl;
    glDeleteShader(id);
    return 0;
 }
  return id;
}
//compile vertexShader and fragemebtShader and return an index for the compiked shadee
static unsigned int createShader(const std::string& vertexShader,const std::string& fragmentShader) {

  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(vertexShader,GL_VERTEX_SHADER);
  unsigned int fs = compileShader(fragmentShader,GL_FRAGMENT_SHADER);

  glAttachShader(program,vs);
  glAttachShader(program,fs);
  glLinkProgram(program); 
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}
//main game loop 
void handleInput(state& st) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT :
        st = QUIT;
        return;
    }
  }
}
//initialize sdl window and cr£te opengl context
bool sdlInit(sdl_t* sdl) {

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr<<"sdl initialization failed"<<SDL_GetError()<<std::endl;
    return false;
  }

  // Set OpenGL attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  sdl->window= SDL_CreateWindow("Triangle",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED , 680 , 480 , SDL_WINDOW_OPENGL| SDL_WINDOW_SHOWN);

  if(!sdl->window) {
    std::cerr<<"window initialization failed"<<SDL_GetError()<<std::endl;
    return false;
  }

  //create opengl context
  sdl->glContext = SDL_GL_CreateContext(sdl->window);
  if (!sdl->glContext) {
    std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
    return false; 
  }

  return true;
}

//after creating openglcontexg initialjze glew 
bool glewInitialization() {
  if(glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return false;
  }

  return true;
}


int main() {
  //create a sdl windkw type etc
  sdl_t sdl = {0};
  state st = RUNNING;
  
  //create sdl window and opengl context
  if(!sdlInit(&sdl)) return -1;

  glewExperimental = GL_TRUE;
  if(!glewInitialization()) return -1;
  
  float vertices[] = {
    0.0f,0.5f,//top
    -0.5f,-0.5f,//left
    0.5f,-0.5f//right
  };
  
  //buffer is a key which points to the data
  unsigned int vao,vbo;
  glGenVertexArrays(1,&vao);
  glGenBuffers(1,&vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);

  glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),vertices,GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

  std::string vertexShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0)in vec4 vertex;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vertex;\n"
    "}\n";
  

  std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0)out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1.0,0.0,0.0,1.0);\n"
    "}\n";
  
  unsigned int shader = createShader(vertexShader,fragmentShader);
  glUseProgram(shader);

  //window loop 
  //closw when user inputs zero
  while(st == RUNNING) {
    handleInput(st);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    //swap buffer
    SDL_GL_SwapWindow(sdl.window);
  }

  //clean up 
  //TODO make a function
  SDL_GL_DeleteContext(sdl.glContext);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();


  return 0;
}

