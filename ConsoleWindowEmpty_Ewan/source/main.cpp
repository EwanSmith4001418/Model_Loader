#include "Renderer.h"

int main(int argc, char* argv[])
{
	Renderer* MyRenderer = new Renderer();
	MyRenderer->Run("Renderer", 1920, 1080, false);
	delete MyRenderer;
	return 0;
}