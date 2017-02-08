#include "stdafx.h"
#include "meshtree.h"

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

	m_BaseTransform( 1.f ),
	m_LocalAxes( 1.f ),
	m_Pivot( 1.f ), m_PivotInverse( 1.f ),

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
	matrix = matrix * (m_BaseTransform * m_LocalAxes * m_Pivot * m_Transform.getMatrix( ) * m_PivotInverse);

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

void CMeshTree::Node::SetPivot( const glm::vec3& world ) {
	m_Pivot = translate( glm::mat4( 1.f ), -world );
	m_PivotInverse = inverse( m_Pivot );
}

void CMeshTree::Node::SetPivot( const glm::mat4& mat ) {
	m_Pivot = mat;
	m_PivotInverse = inverse( m_Pivot );
}

const glm::vec3 CMeshTree::Node::GetPivotPoint( ) {
	return vec3( GetPivotMatrix( ) * vec4( 0.f, 0.f, 0.f, 1.f ) );
}

const glm::mat4 CMeshTree::Node::GetPivotMatrix( ) {
	return m_Pivot;
}

const glm::vec3 CMeshTree::Node::GetTransformedPivot( ) {
	return glm::vec3( GetTransform( ) * glm::vec4( GetPivotPoint( ), 1.f ) );
}

const glm::mat4 CMeshTree::Node::GetTransform( ) {
	glm::mat4 transform = m_BaseTransform * m_Pivot * m_Transform.getMatrix( ) *  m_PivotInverse;

	if (m_Parent)
		transform = m_Parent->GetTransform( ) * transform;

	return transform;
}
