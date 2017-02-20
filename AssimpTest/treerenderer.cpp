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
	m_VAO.bindBufferToAttr( &m_VBO_Color, 1 );
	glt::VAO::release( );
}

void CTreeRenderer::RenderTree( CMeshTree::Node* root ) {
	GetShader( ).use( );

	std::vector<glm::vec3> points, colors;
	AddNode( root, points, colors );

	m_VAO.bind( );

	m_VBO.setData( points );
	m_VBO_Color.setData( colors );

	glLineWidth( 4.f );
	glDrawArrays( GL_LINES, 0, points.size( ) );

	glt::VAO::release( );
}

void CTreeRenderer::AddNode( CMeshTree::Node* node, std::vector<glm::vec3>& pointList, std::vector<glm::vec3>& colorList, glm::mat4 matrix, glm::mat4 axes ) {
	mat4 localSpace = matrix * node->GetLocalSpace( );
	axes = axes * node->GetRotationAxes( );
	vec3 localPoint = vec3( localSpace * vec4( 0.f, 0.f, 0.f, 1.f ) );

	if (node->GetParent( ) != NULL) {
		pointList.push_back( vec3( matrix * vec4( 0.f, 0.f, 0.f, 1.f ) ) );
		pointList.push_back( vec3( localSpace * vec4( 0.f, 0.f, 0.f, 1.f ) ) );
		colorList.push_back( vec3( 1.f ) );
		colorList.push_back( vec3( 1.f ) );

		// Draw local rotation axes
		auto draw_axes = [&pointList, &colorList, &node, &localPoint, &axes]( glm::vec3 axis ) {
			pointList.push_back( localPoint );
			pointList.push_back( localPoint + vec3( axes * vec4( axis, 1.f ) ) * 0.5f );

			colorList.push_back( axis );
			colorList.push_back( axis );
		};
		//draw_axes( glm::vec3( 1.f, 0.f, 0.f ) );
		//draw_axes( glm::vec3( 0.f, 1.f, 0.f ) );
		//draw_axes( glm::vec3( 0.f, 0.f, 1.f ) );
	}

	for (CMeshTree::Node* child : node->GetChildren( )) {
		AddNode( child, pointList, colorList, localSpace, axes );
	}
}
