#pragma once

#include <vector>
#include "OpenGLRenderer.h"

class Geometry {
public:
	struct VERTEX
	{
		VERTEX() : x(0.0f), y(0.0f), z(0.0f) {}

		VERTEX(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		VERTEX(const glm::vec3& pos) : x(pos.x), y(pos.y), z(pos.z) {}

		static constexpr int32_t GetStride() { return 12; }

		float x, y, z;
	};

	Geometry();
	~Geometry();

	void Clear();

	void GenSphere(const glm::vec3& radius, const glm::vec3& offset = glm::vec3(0.0f), uint32_t rings = 24, uint32_t segments = 24);

	void SetAttribs(GLuint program);
	void DisableAttribs(GLuint program);

	void Draw(IRenderer& renderer);

private:
	std::vector<VERTEX> m_arrVertices;
	GLenum m_eDrawmode;
};