#include "../include/Node.h"

Node::Node() :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_vVelocity(0.0f, 0.0f, 0.0f),
	m_vAcceleration(0.0f, 0.0f, 0.0f),
	m_fRadius(1.0f)
{
}

Node::Node(const std::string_view name) :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_strName(name),
	m_vVelocity(0.0f, 0.0f, 0.0f),
	m_vAcceleration(0.0f, 0.0f, 0.0f),
	m_fRadius(1.0f)
{
}

void Node::Update(float frametime)
{
	auto pos = GetPos();

	m_vVelocity += m_vAcceleration * frametime;

	pos += m_vVelocity * frametime;
	SetPos(pos);

	// Upadte child nodes
	for (auto& node : m_arrNodes) 
	{
		node->Update(frametime);
	}
}

void Node::Render(IRenderer& renderer, GLuint program)
{
	// Render child nodes
	for (auto& node : m_arrNodes)
	{
		node->Render(renderer, program);
	}
}

void Node::AddNode(std::shared_ptr<Node> node)
{
	node->m_pParent = this;
	m_arrNodes.push_back(node);
}

Node* Node::FindNode(const std::string_view name)
{
	if (m_strName == name) return this;

	for (auto& node : m_arrNodes) 
	{
		auto n = node->FindNode(name);
		if (n) return n;
	}

	return nullptr;
}
