
#include <string>
#include <GL/glew.h>
#include <glm.hpp>

struct WP_Image
{
	WP_Image();
	~WP_Image();

private:
	GLuint m_imageID;
	std::string m_imageName;
	int m_width;
	int m_height;
	int m_nrChannels;

public:
	void LoadImageIntoOpenGL(const std::string& _filePath,const std::string& _imageName);
	GLuint GetImage();
	int GetWidth();
	int GetHeight();
	const std::string& GetImageName() const;

private:
	void FreeImage();
};