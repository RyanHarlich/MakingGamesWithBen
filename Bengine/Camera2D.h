#pragma once

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 



namespace Bengine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);


		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		// setters
		void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		// getters
		glm::vec2 getPosition() { return m_position; }
		float getScale() { return m_scale; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }



	private:
		int m_screenWidth, m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
	};

}

