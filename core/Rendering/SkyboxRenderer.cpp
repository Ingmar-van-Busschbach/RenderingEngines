//
// Created by Gebruiker on 11/7/2026.
//

#include "SkyboxRenderer.h"

namespace core
{
    namespace rendering
    {
        SkyboxRenderer::SkyboxRenderer(GLuint shaderProgram) : matrixShaderData(shaderProgram)
        {
            this->shaderProgram = shaderProgram;
            glUseProgram(shaderProgram);
            this->textureUniform = glGetUniformLocation(shaderProgram, "skybox");
            glUniform1i(textureUniform, 0);
            this->matrixShaderData.GetUniforms();
        }

        SkyboxRenderer::~SkyboxRenderer()
        {

        }

        void SkyboxRenderer::Render(Mesh mesh, Texture texture)
        {
            glDepthFunc(GL_LEQUAL);
            glUseProgram(shaderProgram);
            mesh.renderSkyBox(texture);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

        void SkyboxRenderer::ApplyMatrices(glm::mat4 view, glm::mat4 projection)
        {
            view = glm::mat4(glm::mat3(view));
            glm::mat4 model = glm::mat4(1);
            this->matrixShaderData.ApplyUniforms(model, view, projection);
        }
    } // rendering
} // core