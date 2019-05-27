#include "Window.h"

#include <GL/glew.h>

namespace s3
{
	Window::Window(const std::string& name, int width, int height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
	  {
	    printf("Failed to init SDL\n");
	  }

    //opengl shit===========
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //we need to enable depth buffer?
    //aparently set to 16 bits by defualt

	  //create a window, the last flag says its gonna be an opengl context
	  window = SDL_CreateWindow(
	    name.c_str(),
	    SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED,
	    width,
	    height,
	    SDL_WINDOW_OPENGL
	  );

	  //create context
	  SDL_GLContext mainContext = SDL_GL_CreateContext(window);

	  //disable v-sync
	  SDL_GL_SetSwapInterval(0);

	  glEnable(GL_DEPTH_TEST);

	  //this is critical, seg fault happens without it
	  glewExperimental = GL_TRUE; 
	  glewInit();

	  this->setBG(0.1f, 0.15f, 0.25f);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	SDL_Window* Window::getSDLWindow()
	{
		return window;
	}

	//all three methods for setting the background color
	void Window::setBG(glm::vec3 color){
		setBG(color[0], color[1], color[2], 1.0);
	}
	void Window::setBG(float r , float g, float b){
		setBG(r, g, b, 1.0);
	}
	void Window::setBG(float r , float g, float b, float a){
		glClearColor(r, g, b, a);
	}

	void Window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Window::render()
	{
		SDL_GL_SwapWindow(window);
	}
}