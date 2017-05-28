#include <fstream>
#include <vector>

#include "GLSLProgram.h"
#include "BengineErrors.h"

/* NEW */
#include "IOManager.h"



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


		/* NEW */
		std::string vertSource;
		std::string fragSource;
		
		/* NEW */
		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);


		/* NEW: to replace the code below */
		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());


		/* New: Moved to compileShadersFromSource to avoid duplicate code
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


		/* NEW: pass in source code rather than load file to read, also, passing in file path name for error checking
		//compileShader(vertexShaderFilePath, m_vertexShaderID);
		//compileShader(fragmentShaderFilePath, m_fragmentShaderID);
		compileShader(vertSource.c_str(), vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragSource.c_str(), fragmentShaderFilePath, m_fragmentShaderID);
		*/
	}




	/* NEW */
	void GLSLProgram::compileShadersFromSource(const char * vertexSource, const char * fragmentSource) {
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

		compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
		compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);

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






	/* NEW */
	void GLSLProgram::dispose() {
		if (m_programID) glDeleteProgram(m_programID);
	}






	/* NEW: changed parameter, and add a name paramter for error checking */
	//void GLSLProgram::compileShader(const std::string filePath, GLint shaderID) {
	void GLSLProgram::compileShader(const char* source, const std::string& name, GLint shaderID) {

		/* NEW: removed since now passing in the file source rather than the file path to read
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
		*/


		/* NEW: now using source (that is passed in) instead of file path's file content */
		//glShaderSource(shaderID, 1, &fileContent, nullptr);
		glShaderSource(shaderID, 1, &source, nullptr);



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


			/* NEW: changed to name instead of filePath (ex. Vertex Shader, Fragment Shader) */
			//fatalError("Failed to compile " + filePath);
			fatalError("Failed to compile " + name);
		}
	}

}