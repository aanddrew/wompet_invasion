#ifndef S3WINDOW_H
#define S3WINDOW_Hglm::vec3 translation

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>

namespace s3
{

class Window
{
public:
	Window(const std::string& name, int width, int height);
	~Window();

	SDL_Window* getSDLWindow();

	void setBG(glm::vec3 color);
	void setBG(float, float, float);
	void setBG(float, float, float, float);

	void clear();
	void render();
private:
	SDL_Window* window;
};

}

#endif