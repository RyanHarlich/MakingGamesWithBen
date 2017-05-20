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

		void linkShaders();

		GLuint getUniformLocation(const std::string& uniformName);

		void use();

		void unuse();


	private:
		void compileShader(const std::string filePath, GLint shaderID);


		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		int _numAttrib;

	};

}
