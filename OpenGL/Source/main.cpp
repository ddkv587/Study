#include <GL/glew.h>
#include <GL/glfw.h>

int main(int argc, const char *argv[])
{
	glfwInit();
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(1024, 768, 0, 1.0f, 0, 0, 32, 0, GLFW_WINDOW);

	glewInit();

	//glClearColor(0.0f, 0.0f, 0.3f ,0.0f);

	while(1)
	{
		glfwSwapBuffers();
	}

	return 0;
}
