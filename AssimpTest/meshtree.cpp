#include "stdafx.h"
#include "meshtree.h"

#include <glm/gtx/matrix_decompose.hpp>

CMeshTree::CMeshTree( ) :
	m_Root( NULL ) {
}

CMeshTree::~CMeshTree( ) {
	delete m_Root;
	m_Root = NULL;

	for (CMesh* mesh : m_MeshList)
		delete mesh;

	m_MeshList.clear( );
}

void CMeshTree::SetRoot( Node* node ) {
	if (m_Root) {
		delete m_Root;
		m_Root = NULL;
	}

	m_Root = node;
}

void CMeshTree::Draw( glt::Shader& shader ) {
	if (m_Root)
		m_Root->Draw( shader, this );
}

CMeshTree::Node::Node( std::string name ) :
	m_Name( name ),

	m_Transform( ),
	m_RotationAxes( 1.f ),

	m_MeshIndex( -1 ),
	m_Parent( NULL ), m_Children( ) {
}

CMeshTree::Node::~Node( ) {
	for (Node* child : m_Children)
		delete child;

	m_Children.clear( );
}

void CMeshTree::Node::Draw( glt::Shader& shader, CMeshTree* tree ) { Draw( shader, mat4( 1.f ), tree ); }
void CMeshTree::Node::Draw( glt::Shader& shader, glm::mat4 matrix, CMeshTree* tree ) {
	matrix = matrix * GetLocalSpace( );

	if (tree && m_MeshIndex != -1) {
		shader.set( "u_model", matrix );
		tree->GetMesh( m_MeshIndex )->Draw( );
	}

	for (Node* n : m_Children)
		n->Draw( shader, matrix, tree );
}

CMeshTree::Node* CMeshTree::Node::AddChild( Node* node ) {
	node->m_Parent = this;
	m_Children.push_back( node );

	return node;
}

CMeshTree::Node * CMeshTree::Node::FindNode( const std::string & name ) {
	if (m_Name == name)
		return this;

	// End of the line
	if (GetChildCount( ) == 0)
		return NULL;

	else {
		Node* result = NULL;

		// Keep searching
		for (Node* child : m_Children) {
			result = child->FindNode( name );

			// Found in child
			if (result)
				break;
		}

		return result;
	}
}

void CMeshTree::Node::SetTransform( const glm::mat4& mat ) {
	CNodeTransform transform;
	glm::vec3 ununsed3;
	glm::vec4 ununsed4;

	glm::decompose( mat, transform.m_Scale, transform.m_Rotation, transform.m_Position, ununsed3, ununsed4 );

	SetTransform( transform );
}