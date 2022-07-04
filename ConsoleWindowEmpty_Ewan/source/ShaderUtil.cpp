#include "ShaderUtil.h"
#include "Utilities.h"
#include <glad/glad.h>
#include <iostream>

//Static Instance of ShaderUtil
ShaderUtil* ShaderUtil::mInstance = nullptr;
//Singleton Creation, Fetch and Destroy functionality
ShaderUtil* ShaderUtil::GetInstance()
{
	if (mInstance == nullptr)
	{
		return ShaderUtil::CreateInstance();
	}
	return mInstance;
}

ShaderUtil* ShaderUtil::CreateInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ShaderUtil();
	}
	else
	{
		//Print to console that attempt to create multiple instance of ShaderUtil
		std::cout << "Attempt to create multiple instances of ShaderUtil" << std::endl;
	}
	return mInstance;
}

void ShaderUtil::DestroyInstance()
{
	if (mInstance != nullptr)
	{
		delete mInstance;
		mInstance = nullptr;
	}
	else
	{
		//Print to console that attempt to destroy null instance of ShaderUtil
		std::cout << "Attempt to destroy null instance of ShaderUtil" << std::endl;
	}
}

ShaderUtil::ShaderUtil()
{
}

ShaderUtil::~ShaderUtil()
{
	//Delete any shaders that have not been unloaded
	for (auto iter = mShaders.begin(); iter != mShaders.end(); ++iter)
	{
		glDeleteShader(*iter);
	}
	//Destroy and programs that are still dangling about
	for (auto iter = mPrograms.begin(); iter != mPrograms.end(); ++iter)
	{
		glDeleteProgram(*iter);
	}
}

unsigned int ShaderUtil::LoadShader(const char* a_filename, unsigned int a_type)
{
	ShaderUtil* instance = ShaderUtil::GetInstance();
	return instance->LoadShaderInternal(a_filename, a_type);
}

unsigned int ShaderUtil::LoadShaderInternal(const char* a_filename, unsigned int a_type)
{
	//Integer to test for shader creation success
	int success = GL_FALSE;
	//Grab the shader source from the file
	char* source = Utility::fileToBuffer(a_filename);
	unsigned int shader = glCreateShader(a_type);
	//Set the source buffer for the shader
	glShaderSource(shader, 1, (const char**)&source, 0);
	glCompileShader(shader);
	//As the buffer from fileToBuffer was allocated this needs to be destroyed now
	delete[] source;

	//Test shader compilation for any errors and display them to console
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (GL_FALSE == success) //Shader compilation failed, get logs and display them to console
	{
		int infoLogLength = 0; //Variable to store the length of the error log
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength]; //Allocate buffer to hold data
		glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);
		std::cout << "Unable to compile: " << a_filename << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		return 0;
	}
	//Success - Add shader to mShaders vector
	mShaders.push_back(shader);
	return shader;
}

void ShaderUtil::DeleteShader(unsigned int a_shader)
{
	ShaderUtil* instance = ShaderUtil::GetInstance();
	instance->DeleteShaderInternal(a_shader);
}

void ShaderUtil::DeleteShaderInternal(unsigned int a_shader)
{
	//Loop through the shaders vector
	for (auto iter = mShaders.begin(); iter != mShaders.end(); ++iter)
	{
		if (*iter == a_shader) //If we find the shader we are looking for
		{
			glDeleteShader(*iter);	//Delete the shader
			mShaders.erase(iter);	//Remove this item from the shaders vector
			break;					//Break out of the for loop
		}
	}
}

unsigned int ShaderUtil::CreateProgram(const int& a_vertexShader, const int& a_fragmentShader)
{
	ShaderUtil* instance = ShaderUtil::GetInstance();
	return instance->CreateProgramInternal(a_vertexShader, a_fragmentShader);
}

unsigned int ShaderUtil::CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader)
{
	//Boolean value to test for shader program linkage success
	int success = GL_FALSE;

	//Create a shader program and attach the shaders to it
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_fragmentShader);
	//Link the shaders together into one shader program
	glLinkProgram(handle);
	//Test to see if the program was successfully linked
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (GL_FALSE == success) //if something has gone wrong then execute this
	{
		int infoLogLength = 0; //Integer value to tell us the length of the error log
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		//Allocate enough space in a buffer for the error message
		char* infoLog = new char[infoLogLength];
		//Fill the buffer with data
		glGetProgramInfoLog(handle, infoLogLength, 0, infoLog);
		//Print Log message to console
		std::cout << "Shader linker error" << std::endl;
		std::cout << infoLog << std::endl;

		//Delete the char buffer now we have displayed it
		delete[] infoLog;
		return 0; //Return 0, programID 0 is a null program
	}
	//add the program to the shader program vector
	mPrograms.push_back(handle);
	return handle; //Return the program ID
}

void ShaderUtil::DeleteProgram(unsigned int a_program)
{
	ShaderUtil* instance = ShaderUtil::GetInstance();
	instance->DeleteProgramInternal(a_program);
}

void ShaderUtil::DeleteProgramInternal(unsigned int a_program)
{
	//Loop through the programs vector
	for (auto iter = mPrograms.begin(); iter != mPrograms.end(); ++iter)
	{
		if (*iter == a_program) //If we find the program we are looking for
		{
			glDeleteProgram(*iter);	//Delete the program
			mPrograms.erase(iter);	//Remove this item from the programs vector
			break;					//Break out of the for loop
		}
	}
}