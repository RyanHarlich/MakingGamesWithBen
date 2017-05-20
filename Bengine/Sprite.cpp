#include <cstddef> /* offsetof() */

#include "Sprite.h"
#include "Vertex.h"
#include "Errors.h"
#include "ResourceManager.h"




namespace Bengine {

	Sprite::Sprite() : _vboID(0)
	{
	}



	Sprite::~Sprite()
	{

		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}





	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_texture = ResourceManager::getTexture(texturePath);

		if (_vboID == 0) {
			glGenBuffers(1, &_vboID);
		}

		if (_vboID == 0) {
			fatalError("Vertex Buffer ID was not created!");
		}


		Vertex vertexData[6]; // 6 vertices to make a square

		// Triangle 1
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		// Triangle 2
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);


		for (int i = 0; i < 6; ++i) {
			vertexData[i].setColor(255, 0, 255, 255);
		}
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);


		//bind to VBO CPU
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//upload to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//unbind from VBO CPU
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}






	void Sprite::draw() {

		glBindTexture(GL_TEXTURE_2D, _texture.id);
		//bind to VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);


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


		glDrawArrays(GL_TRIANGLES, 0, 6);


		// remove position
		glDisableVertexAttribArray(0);
		// remove color
		glDisableVertexAttribArray(1);
		// remove uv coords
		glDisableVertexAttribArray(2);


		//unbind from VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}