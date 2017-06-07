#include <algorithm> /* sort() */

#include "SpriteBatch.h"




namespace Bengine {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
		texture(Texture),
		depth(Depth) {
		// z represents width and w represents height
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);
		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}





	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
		texture(Texture),
		depth(Depth) {


		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f); 
		glm::vec2 tl(-halfDims.x, halfDims.y); // tl = topleft
		glm::vec2 bl(-halfDims.x, -halfDims.y); // bl = bottomleft
		glm::vec2 br(halfDims.x, -halfDims.y); // br = bottomright
		glm::vec2 tr(halfDims.x, halfDims.y); // tr = topright

		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;


		// z represents width and w represents height
		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);
		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}





	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle) {

		glm::vec2 newv; // new vector
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;

	}






	SpriteBatch::SpriteBatch() : m_vboID(0), m_vaoID(0)
	{
	}




	SpriteBatch::~SpriteBatch()
	{
	}




	void SpriteBatch::init() {
		createVertexArray();
	}





	void SpriteBatch::dispose() {
		if (m_vaoID != 0) {
			glDeleteVertexArrays(1, &m_vaoID);
			m_vaoID = 0;
		}
		if (m_vboID != 0) {
			glDeleteBuffers(1, &m_vboID);
			m_vboID = 0;
		}
	}




	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortTypeT30::TEXTURE */) {
		m_sortType = sortType;
		m_renderBatches.clear();
		m_glyphs.clear();
	}





	void SpriteBatch::end() {
		m_glyphPointers.resize(m_glyphs.size());
		for (size_t i = 0; i < m_glyphs.size(); ++i) {
			m_glyphPointers[i] = &m_glyphs[i];
		}
		sortGlyphs();
		createRenderBatchs();
	}





	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}




	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}





	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {

		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir)); // will always be a positive angle
		if (dir.y < 0.0f) angle = -angle;

		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}







	void SpriteBatch::renderBatch() {
		glBindVertexArray(m_vaoID);
		for (size_t i = 0; i < m_renderBatches.size(); ++i) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}





	void SpriteBatch::createRenderBatchs() {

		std::vector <Vertex> vertices;

		vertices.resize(m_glyphPointers.size() * 6);

		if (m_glyphPointers.empty()) {
			return;
		}

		int offset = 0;
		int cv = 0; // current vertex

		m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);

		vertices[cv++] = m_glyphPointers[0]->topLeft;
		vertices[cv++] = m_glyphPointers[0]->bottomLeft;
		vertices[cv++] = m_glyphPointers[0]->bottomRight;
		vertices[cv++] = m_glyphPointers[0]->bottomRight;
		vertices[cv++] = m_glyphPointers[0]->topRight;
		vertices[cv++] = m_glyphPointers[0]->topLeft;

		offset += 6;

		for (size_t cg = 1; cg < m_glyphs.size(); ++cg) { //current Glyph
			if (m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture) {
				m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
			}
			else {
				m_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = m_glyphPointers[cg]->topLeft;
			vertices[cv++] = m_glyphPointers[cg]->bottomLeft;
			vertices[cv++] = m_glyphPointers[cg]->bottomRight;
			vertices[cv++] = m_glyphPointers[cg]->bottomRight;
			vertices[cv++] = m_glyphPointers[cg]->topRight;
			vertices[cv++] = m_glyphPointers[cg]->topLeft;

			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
		// unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}





	void SpriteBatch::createVertexArray() {

		if (m_vaoID == 0) {
			glGenVertexArrays(1, &m_vaoID);
		}
		glBindVertexArray(m_vaoID);


		if (m_vboID == 0) {
			glGenBuffers(1, &m_vboID);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);



		// add position
		glEnableVertexAttribArray(0);
		// add color
		glEnableVertexAttribArray(1);
		// add uv coordinates
		glEnableVertexAttribArray(2);

		// add position pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// add color pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// add uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// unbind vbo
		glBindVertexArray(0);
	}






	void SpriteBatch::sortGlyphs() {

		switch (m_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
			break;
		}
	}







	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}






	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}







	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}


}
