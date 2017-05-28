#include "DebugRenderer.h"

const float PI = 3.14159265359f;

namespace {
	const char* VERT_SRC = R"(#version 130

in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main(){
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
})";

	const char* FRAG_SRC = R"(#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

void main(){
	color = fragmentColor;
})";
}

namespace Bengine {

	DebugRenderer::DebugRenderer() {
		// Empty
	}



	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}



	void DebugRenderer::init() {

		// Shader init
		m_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
		m_program.addAttribute("vertexPosition");
		m_program.addAttribute("vertexColor");
		m_program.linkShaders();


		// Set up buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		
		// Postion
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, position));
		// Color
		glEnableVertexAttribArray(1);
		// normalized is true because want to normalize it between a number between 0-1
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, color));


		glBindVertexArray(0);
	}






	void DebugRenderer::end() {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// Orphon the buffer
		glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		// Orphon the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



		m_numElements = m_indices.size();
		m_indices.clear();
		m_verts.clear();
	}







	glm::vec2 rotatePoint(glm::vec2 pos, float angle) {
		glm::vec2 newv; // new vector
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}


	void DebugRenderer::drawBox(const glm::vec4 destRect, const ColorRGBA8 & color, float angle) {

		int i = m_verts.size();
		m_verts.resize(m_verts.size() + 4);

		// Get points centered at origin
		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);
		glm::vec2 tl(-halfDims.x, halfDims.y); // tl = topleft
		glm::vec2 bl(-halfDims.x, -halfDims.y); // bl = bottomleft
		glm::vec2 br(halfDims.x, -halfDims.y); // br = bottomright
		glm::vec2 tr(halfDims.x, halfDims.y); // tr = topright


		glm::vec2 positionOffSet(destRect.x, destRect.y);


		// Rotate the points
		m_verts[i].position = rotatePoint(tl, angle) + halfDims + positionOffSet;
		m_verts[i + 1].position = rotatePoint(bl, angle) + halfDims + positionOffSet;
		m_verts[i + 2].position = rotatePoint(br, angle) + halfDims + positionOffSet;
		m_verts[i + 3].position = rotatePoint(tr, angle) + halfDims + positionOffSet;


		for (int j = i; j < i + 4; ++j) {
			m_verts[j].color = color;
		}


		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i + 1);

		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);

		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);

		m_indices.push_back(i + 3);
		m_indices.push_back(i);

	}





	void DebugRenderer::drawCircle(const glm::vec2 & center, const ColorRGBA8 & color, float radius) {

		static const int NUM_VERTS = 100;

		// Set up vertices
		int start = m_verts.size();
		m_verts.resize(m_verts.size() + NUM_VERTS);
		// start at 0 degrees (0) and go all the way around to 360 degrees (1) and multiple by 2pi.
		for (int i = 0; i < NUM_VERTS; ++i) {
			// the angle in radians of each point
			float angle = ((float)i / NUM_VERTS) * PI * 2.0f;

			m_verts[start + i].position.x = cos(angle) * radius + center.x;
			m_verts[start + i].position.y = sin(angle) * radius + center.y;
			m_verts[start + i].color = color;
		}

		// Set up indices for indexed drawing
		m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
		for (int i = 0; i < NUM_VERTS - 1; ++i) {
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}
		m_indices.push_back(start + NUM_VERTS - 1);
		m_indices.push_back(start);
	}



	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth) {
		m_program.use();

		// Camera matrix
		GLint pUniform = m_program.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);
		// the pointer is 0 since it is a ibo (index buffer object)
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		m_program.unuse();
	}



	void DebugRenderer::dispose() {
		if (m_vao) {
			glDeleteVertexArrays(1, &m_vao);
		}
		if (m_vbo) {
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_ibo) {
			glDeleteBuffers(1, &m_ibo);
		}
		m_program.dispose();
	}

}