#include "Skybox.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Texture.h"
#include "ShaderUtil.h"

//Constructor and Destructor
Skybox::Skybox() : m_skyboxVBO(0), m_skyboxVAO(0), m_skyboxShader(0)
{
    //Call to the CubeMap constructor to construct a new CubeMap as Skybox is constructed
    m_skyboxTexture = new CubeMap();
}
Skybox::~Skybox()
{
    delete m_skyboxTexture;
}

void Skybox::SetupSkybox()
{
    ShaderUtil* shaderUtilIns = ShaderUtil::GetInstance();

    //Create the Skybox Shader Program
    unsigned int vertexShader = ShaderUtil::LoadShader("resource/shaders/skybox_vertex.glsl", GL_VERTEX_SHADER);
    unsigned int fragmentShader = ShaderUtil::LoadShader("resource/shaders/skybox_fragment.glsl", GL_FRAGMENT_SHADER);
    m_skyboxShader = ShaderUtil::CreateProgram(vertexShader, fragmentShader);
    //Delete shaders once program is created
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenBuffers(1, &m_skyboxVBO);
    glGenVertexArrays(1, &m_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBindVertexArray(m_skyboxVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::RenderSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glDepthMask(GL_FALSE);
    glUseProgram(m_skyboxShader);

    int skyboxLocation = glGetUniformLocation(m_skyboxShader, "CubeMap");
    glUniform1i(skyboxLocation, 0);

    glm::mat4 viewMat = glm::mat4(glm::mat3(viewMatrix)); //Remove translation from view matrix

    //Pass view and projection matrix to the skybox shader
    int projectionLocation = glGetUniformLocation(m_skyboxShader, "ProjectionMatrix");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int viewLocation = glGetUniformLocation(m_skyboxShader, "View");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));

    glBindVertexArray(m_skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture->GetCubeMapTexture());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    //Return the depth function to default and stop using Skybox shaders
    glDepthMask(GL_TRUE); 
    glUseProgram(0);
}