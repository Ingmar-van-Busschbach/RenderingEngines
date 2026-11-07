#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace core {
    Texture::Texture(const std::string &path) {
        glGenTextures(1, &id);
        int width, height, nrComponents;
        if (unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0)) {
            GLenum format = 0;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }

            printf("Loaded Texture with resolution of %d x %d [Components: %d]!\r\n", width, height, nrComponents);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0,
                         static_cast<GLint>(format)
                    , width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        } else {
            printf("Texture failed to load at path: %s\n", path.c_str());
            stbi_image_free(data);
        }
    }

    Texture::Texture(const std::string path[6])
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        for (unsigned int i = 0; i < 6; i++)
        {
            int width, height, nrComponents;
            unsigned char *data = stbi_load(path[i].c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                stbi_set_flip_vertically_on_load(false);
                glTexImage2D
                (
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                    );
                stbi_image_free(data);
                printf("Loaded Texture with resolution of %d x %d [Components: %d]!\r\n", width, height, nrComponents);
            }
            else
            {
                printf("Texture failed to load at path: %s\n", path[i].c_str());
                stbi_image_free(data);
            }
        }
    }

    GLuint Texture::getId() {
        return id;
    }
}