#include <fstream>
#include <vector>

#include "GLSLProgram.h"
#include "Errors.h"



namespace Bengine {

	GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttrib(0)
	{
	}




	GLSLProgram::~GLSLProgram()
	{
	}




	void GLSLProgram::addAttribute(const std::string &attributeName) {
		glBindAttribLocation(_programID, _numAttrib++, attributeName.c_str());
	}




	void GLSLProgram::compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath) {
		_programID = glCreateProgram();
		if (_programID == GL_FALSE) {
			fatalError("Program ID was not created!");

		}

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == GL_FALSE) {
			fatalError("Vertex Shader ID was not created!");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == GL_FALSE) {
			fatalError("Fragment Shader ID was not created!");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}





	void GLSLProgram::linkShaders() {

		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		glLinkProgram(_programID);



		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);

			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			glDeleteProgram(_programID);

			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
			printf("%s\n", &errorLog[0]);
			fatalError("Shaders were not linked to program!");
		}



		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}





	GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {

		GLuint location = glGetUniformLocation(_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}





	void GLSLProgram::use() {

		glUseProgram(_programID);

		for (int i = 0; i < _numAttrib; ++i) {
			glEnableVertexAttribArray(i);
		}
	}




	void GLSLProgram::unuse() {

		glUseProgram(0);

		for (int i = 0; i < _numAttrib; ++i) {
			glEnableVertexAttribArray(i);
		}
	}





	void GLSLProgram::compileShader(const std::string filePath, GLint shaderID) {

		std::ifstream shaderFile(filePath);
		if (shaderFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}
		std::string fileContents = "";
		std::string line;

		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}


		shaderFile.close();


		const char* fileContent = fileContents.c_str();
		glShaderSource(shaderID, 1, &fileContent, nullptr);
		glCompileShader(shaderID);



		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(shaderID);

			printf("%s\n", &errorLog[0]);
			fatalError("Failed to compile " + filePath);
		}
	}

}