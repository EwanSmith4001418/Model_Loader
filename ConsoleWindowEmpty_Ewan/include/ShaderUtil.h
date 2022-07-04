#pragma once
#include <vector>

class ShaderUtil
{
public:
	static ShaderUtil* CreateInstance();
	static ShaderUtil* GetInstance();
	static void DestroyInstance();
	static unsigned int LoadShader(const char* a_filename, unsigned int a_type);
	static void DeleteShader(unsigned int a_shaderID);
	static unsigned int CreateProgram(const int& a_vertexShader, const int& a_fragmentShader);
	static void DeleteProgram(unsigned int a_program);

private:
	//private Constructor and Destructor
	//ShaderUtil implements a Singleton Design Pattern
	ShaderUtil();
	~ShaderUtil();

	std::vector<unsigned int>mShaders;
	std::vector<unsigned int>mPrograms;

	unsigned int LoadShaderInternal(const char* a_filename, unsigned int a_type);
	void DeleteShaderInternal(unsigned int a_shaderID);
	unsigned int CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader);
	void DeleteProgramInternal(unsigned int a_program);
	static ShaderUtil* mInstance;
};