#pragma once

#include "OpenGLRenderer.h"

class Node
{
public:
	Node();
	Node(const std::string_view name);
	virtual ~Node() {}

	virtual void Update(float frametime);
	virtual void Render(IRenderer& renderer, GLuint program);

	void AddNode(std::shared_ptr<Node> node);
	inline Node* GetParent() { return m_pParent; }
	inline auto& GetNodes() { return m_arrNodes; }
	inline const auto& GetNodes() const { return m_arrNodes; }
	inline const std::string& GetName() const { return m_strName; }
	inline void SetName(const std::string_view name) { m_strName = name; }
	
	Node* FindNode(const std::string_view name);

	inline void SetPos(const glm::vec3& pos) {
		m_mModel[3][0] = pos.x;
		m_mModel[3][1] = pos.y;
		m_mModel[3][2] = pos.z;
	}
	inline glm::vec3 GetPos() const { return m_mModel[3]; }

	inline auto& Matrix() { return m_mModel; }
	inline void SetMatrix(const glm::mat4& m) { m_mModel = m; }
	inline glm::mat4 GetWorldMatrix() const { return (m_pParent) ? m_pParent->GetWorldMatrix() * m_mModel : m_mModel; }

	inline auto& GetVelocity() { return m_vVelocity; }
	inline void SetVelocity(const glm::vec3& v) { m_vVelocity = v; }
	inline auto& GetAcceleration() { return m_vAcceleration; }
	inline void SetAcceleration(const glm::vec3& a) { m_vAcceleration = a; }
	inline auto& GetRadius() { return m_fRadius; }
	inline void SetRadius(float r) { m_fRadius = r; }

protected:
	glm::mat4 m_mModel;

	Node* m_pParent;
	std::vector<std::shared_ptr<Node>> m_arrNodes;

	glm::vec3 m_vVelocity;
	glm::vec3 m_vAcceleration;

	float m_fRadius;

private:
	std::string m_strName;

};