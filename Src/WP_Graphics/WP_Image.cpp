#include <WP_Graphics/WP_Image.h>
#include <iostream>

#include <stb_image.h>

WP_Image::WP_Image()
    :
    m_imageID{ (GLuint) - 1},
    m_width{0},
    m_height{0},
    m_nrChannels{0}
{
}
WP_Image::~WP_Image()
{
    FreeImage();
}

void WP_Image::LoadImageIntoOpenGL(const std::string& _filePath,const std::string& _fileName)
{
    if (m_imageID != (GLuint) - 1)
        FreeImage();

    m_imageName = _fileName;

    glGenTextures(1, &m_imageID);
    glBindTexture(GL_TEXTURE_2D, m_imageID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(_filePath.c_str(), &m_width, &m_height, &m_nrChannels, 0);

    if (data)
    {
        if (m_nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if(m_nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "cannot load image " << _filePath.c_str();
    }

    stbi_image_free(data);
}
GLuint WP_Image::GetImage()
{
    return m_imageID;
}
int WP_Image::GetWidth()
{
    return m_width;
}
int WP_Image::GetHeight()
{
    return m_height;
}
int WP_Image::GetChannels()
{
    return m_nrChannels;
}
const std::string& WP_Image::GetImageName() const
{
    return m_imageName;
}
void WP_Image::FreeImage()
{
    glDeleteTextures(1, &m_imageID);
}