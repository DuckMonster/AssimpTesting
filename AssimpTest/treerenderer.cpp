#include "stdafx.h"
#include "treerenderer.h"

#include "meshtree.h"

glt::Shader& CTreeRenderer::GetShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_line" );
		COMPILED = true;
	}

	return SHADER;
}

CTreeRenderer::CTreeRenderer( ) {
	m_VAO.bindBufferToAttr( &m_VBO, 0 );
	glt::VAO::release( );
}

void CTreeRenderer::RenderTree( CMeshTree::Node* root ) {
	GetShader( ).use( );

	std::vector<glm::vec3> points;
	AddNode( root, points );

	m_VAO.bind( );

	m_VBO.setData( points );
	glDrawArrays( GL_LINES, 0, points.size( ) );

	glt::VAO::release( );
}

void CTreeRenderer::AddNode( CMeshTree::Node* node, std::vector<glm::vec3>& pointList, glm::mat4 matrix ) {
	for (CMeshTree::Node* child : node->GetChildren( )) {
		pointList.push_back( node->GetTransformedPivot( ) );
		pointList.push_back( child->GetTransformedPivot( ) );

		AddNode( child, pointList, matrix );
	}
}
