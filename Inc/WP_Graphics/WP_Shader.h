#include <glm.hpp>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

struct WP_Shader
{
	WP_Shader() :
		m_shaderID(-1)
	{}

	WP_Shader(GLuint _shaderID) :
		m_shaderID(_shaderID)
	{}

	GLuint m_shaderID;
};

class WP_ShaderManager
{
private:
	std::unordered_map<std::string,WP_Shader> m_shaders;

	static std::unique_ptr<WP_ShaderManager> s_shaderManager;

private:
	//return the file in filepath as a string
	std::string LoadShaderSource(const std::string& _filePath)
	{
		std::ifstream file(_filePath);
		if (!file.is_open())
		{
			std::cerr << "cannot find file" << _filePath << std::endl;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}
	GLuint CompileShader(GLenum _type, const std::string& _source)
	{
		GLuint shader = glCreateShader(_type);

		const char* shaderSource = _source.c_str();

		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);


		/*!
			Check for compile error
		*/
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
		}

		return shader;
	}
public:
	//default constructor
	WP_ShaderManager(){}
	//destructor to release all shader resources
	~WP_ShaderManager()
	{
		for (auto& i : m_shaders)
		{
			glDeleteShader(i.second.m_shaderID);
		}
	}

	static WP_ShaderManager& GetInstance()
	{
		if (!s_shaderManager)
		{
			CreateInstance();
		}
		return *s_shaderManager;
	}

	static void CreateInstance()
	{
		//do not create when theres something that exists
		//ideally its called automatically but when its called manually beware
		assert(!s_shaderManager);

		s_shaderManager = std::make_unique<WP_ShaderManager>();
	}

	/*!
		This function assumes [fileName] is the same for [fileName].vert and [fileName].frag 
	
	*/
	GLuint AddShader(const std::string& _shaderName)
	{
		if (m_shaders.find(_shaderName) != m_shaders.end())
			return m_shaders[_shaderName].m_shaderID;

		std::string fragmentSourcePath = _shaderName;
		fragmentSourcePath += ".frag";
		
		std::string fragmentSource = LoadShaderSource(fragmentSourcePath);

		std::string vertexSourcePath = _shaderName;
		vertexSourcePath += ".vert";

		std::string vertexSource = LoadShaderSource(vertexSourcePath);

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		const GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);


		// Check for linking errors
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
		}

		// Cleanup shaders as they’re linked into the program now and no longer necessary
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//add into the shader map
		m_shaders[_shaderName] = WP_Shader(program);

		return program;
	}
};