#pragma once
#include "meshtree.h"

class CTreeRenderer {
public:
	static glt::Shader& GetShader( );

public:
	CTreeRenderer( );

	void RenderTree( CMeshTree::Node* root );

private:
	void AddNode( CMeshTree::Node* node, std::vector<glm::vec3>& pointList, std::vector<glm::vec3>& colorList, glm::mat4 matrix = glm::mat4( 1.f ), glm::mat4 axes = glm::mat4( 1.f ) );

	glt::VAO				m_VAO;
	glt::VBO<glm::vec3>		m_VBO;
	glt::VBO<glm::vec3>		m_VBO_Color;
};