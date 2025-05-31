#include <WP_Scene/WP_DefaultParticleScene.h>
#include <WP_Graphics/WP_Shader.h>
#include <gtc/matrix_transform.hpp>
#include <WP_Graphics/WP_Primitive.h>
#include <imgui.h>
#include <gtc/type_ptr.hpp>


void WP_DefaultParticleScene::PreSceneInitialisation()
{
    m_defaultShaderID =
        WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultParticleShader");

    m_instancedShaderID = 
        WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultParticleInstanced");

    m_mvpLocation = glGetUniformLocation(m_defaultShaderID, "MVP");
    m_colorLocation = glGetUniformLocation(m_defaultShaderID, "color");

    glGenBuffers(1, &m_optimisedInstancedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_optimisedInstancedVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles
        * sizeof(instancedData), NULL, GL_DYNAMIC_DRAW);
}
void WP_DefaultParticleScene::StartScene()
{
    m_particleSystem.GenerateParticles(m_numberOfParticles);
}
void WP_DefaultParticleScene::RenderImGui()
{
    ImGui::Begin("ParticleSettings");
    if (ImGui::SliderInt("Number Particles", &m_numberOfParticles, 0, m_maxParticles))
    {
        m_particleSystem.DestroyParticles();
        m_particleSystem.GenerateParticles(m_numberOfParticles);
    }

    if (ImGui::SliderFloat("Particle System Speed", &m_particleSystemSpeed, 0, 1))
    {}

    int currentSelection = static_cast<int>(m_renderType);

    const char* renderType[] = { "Default particle rendering", "Instanced particle rendering",
        "Optimised Instanced particle rendering"};
    if (ImGui::Combo("render type", &currentSelection, renderType, IM_ARRAYSIZE(renderType)))
    {
        m_renderType = (currentSelection);
    }

    ImGui::End();
}

void WP_DefaultParticleScene::RenderTriangle(const glm::mat4& _mvp, const glm::vec3& _color)
{
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, (const GLfloat*)&_mvp);
    glUniform3fv(m_colorLocation, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, Primitives::Primitive::triangle.indicesSize, GL_UNSIGNED_INT, 0);
}

void WP_DefaultParticleScene::RenderDefaultNormally()
{

    glUseProgram(m_defaultShaderID);


    glBindVertexArray(Primitives::Primitive::triangle.vertexArray);
    for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
    {
        glm::mat4 mvp;
        mvp = glm::mat4(1.f);
        mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
            m_particleSystem.m_particles[i].pos.y, 0.0f));
        mvp = glm::scale(mvp, glm::vec3(0.1f));
        RenderTriangle(mvp, m_particleSystem.m_particles[i].col);
    }
}

void WP_DefaultParticleScene::RenderDefaultInstanced()
{
    glUseProgram(m_instancedShaderID);

    glBindVertexArray(Primitives::Primitive::triangle.vertexArray);

    std::vector<instancedData> modelMatrices;
    modelMatrices.reserve(m_particleSystem.m_particles.size());

    for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
    {
        glm::mat4 mvp;
        mvp = glm::mat4(1.f);
        mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
            m_particleSystem.m_particles[i].pos.y, 0.0f));
        mvp = glm::scale(mvp, glm::vec3(0.1f));

        modelMatrices.push_back({ mvp, m_particleSystem.m_particles[i].col });
    }

    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() 
        * sizeof(instancedData), modelMatrices.data(), GL_STATIC_DRAW);

    unsigned int vertexBindingLocation = 3;

    for (GLuint i = 0; i < 4; i++) {
        glEnableVertexAttribArray(vertexBindingLocation + i);
        glVertexAttribPointer(vertexBindingLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(vertexBindingLocation + i, 1);
    }

    unsigned int fragmentBindingLocation = 7;
    glEnableVertexAttribArray(fragmentBindingLocation);
    glVertexAttribPointer(fragmentBindingLocation, 3, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::mat4)));
    glVertexAttribDivisor(fragmentBindingLocation, 1);

    glDrawElementsInstanced(GL_TRIANGLES, Primitives::Primitive::triangle.indicesSize, GL_UNSIGNED_INT, 0, m_particleSystem.m_particles.size());

    glDeleteBuffers(1,&instanceVBO);
}


void WP_DefaultParticleScene::RenderDefaultInstancedOptimised()
{
    glUseProgram(m_instancedShaderID);

    glBindVertexArray(Primitives::Primitive::triangle.vertexArray);

    std::vector<instancedData> modelMatrices;
    modelMatrices.reserve(m_particleSystem.m_particles.size());

    for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
    {
        glm::mat4 mvp;
        mvp = glm::mat4(1.f);
        mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
            m_particleSystem.m_particles[i].pos.y, 0.0f));
        mvp = glm::scale(mvp, glm::vec3(0.1f));

        modelMatrices.push_back( {mvp, m_particleSystem.m_particles[i].col});
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_optimisedInstancedVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size()
        * sizeof(instancedData), modelMatrices.data());

    unsigned int vertexBindingLocation = 3;

    for (GLuint i = 0; i < 4; i++) {
        glEnableVertexAttribArray(vertexBindingLocation + i);
        glVertexAttribPointer(vertexBindingLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(vertexBindingLocation + i, 1);
    }

    unsigned int fragmentBindingLocation = 7;
    glEnableVertexAttribArray(fragmentBindingLocation);
    glVertexAttribPointer(fragmentBindingLocation, 3, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::mat4)));
    glVertexAttribDivisor(fragmentBindingLocation, 1);

    glDrawElementsInstanced(GL_TRIANGLES, Primitives::Primitive::triangle.indicesSize, GL_UNSIGNED_INT, 0, m_particleSystem.m_particles.size());
}

void WP_DefaultParticleScene::UpdateScene()
{
    m_particleSystem.RunParticleSystem(m_particleSystemSpeed);

    switch( m_renderType)
    {
        case 0:
        {
            RenderDefaultNormally();
            break;
        }
        case 1:
        {
            RenderDefaultInstanced();
            break;
        }
        case 2:
        {
            RenderDefaultInstancedOptimised();
            break;
        }
        default:
        {

        }
    }
}
void WP_DefaultParticleScene::DeallocateScene()
{

    glDeleteBuffers(1, &m_optimisedInstancedVBO);
}