#include <fstream>
#include <vector>

#include "GLSLProgram.h"
#include "BengineErrors.h"



namespace Bengine {

	GLSLProgram::GLSLProgram() : m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0), m_numAttrib(0)
	{
	}




	GLSLProgram::~GLSLProgram()
	{
	}




	void GLSLProgram::addAttribute(const std::string &attributeName) {
		glBindAttribLocation(m_programID, m_numAttrib++, attributeName.c_str());
	}




	void GLSLProgram::compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath) {
		m_programID = glCreateProgram();
		if (m_programID == GL_FALSE) {
			fatalError("Program ID was not created!");

		}

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == GL_FALSE) {
			fatalError("Vertex Shader ID was not created!");
		}

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == GL_FALSE) {
			fatalError("Fragment Shader ID was not created!");
		}

		compileShader(vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}





	void GLSLProgram::linkShaders() {

		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		glLinkProgram(m_programID);



		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);

			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);
			glDeleteProgram(m_programID);

			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);
			printf("%s\n", &errorLog[0]);
			fatalError("Shaders were not linked to program!");
		}



		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}





	GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {

		GLuint location = glGetUniformLocation(m_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}





	void GLSLProgram::use() {

		glUseProgram(m_programID);

		for (int i = 0; i < m_numAttrib; ++i) {
			glEnableVertexAttribArray(i);
		}
	}




	void GLSLProgram::unuse() {

		glUseProgram(0);

		for (int i = 0; i < m_numAttrib; ++i) {
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