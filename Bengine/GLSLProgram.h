#pragma once

#include <string>

#include <GL/glew.h>


namespace Bengine {

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void addAttribute(const std::string &attributeName);

		void compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);

		/* NEW */
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

		void linkShaders();

		GLuint getUniformLocation(const std::string& uniformName);

		void use();

		void unuse();


		/* NEW */
		void dispose();


	private:

		/* NEW: changed parameter, and add a name paramter for error checking */
		//void compileShader(const std::string filePath, GLint shaderID);
		void compileShader(const char* source, const std::string& name, GLint shaderID);


		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
		int m_numAttrib;

	};

}

