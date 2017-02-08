#include "stdafx.h"
#include "program.h"
#include "FBXLoader.h"

CProgram::CProgram( CFramework * framework ) :
	m_Framework( framework ), m_Camera( framework->WindowSize( ) ), m_Animator( m_MeshTree ) {
}

void CProgram::Load( ) {
	m_Shader.compileFile( "shdr_default" );

	{
		int gridSize = 50;
		std::vector<glm::vec3> gridVerts;
		std::vector<glm::vec3> normals;

		for (int x = -gridSize; x <= gridSize; x++) {
			gridVerts.push_back( vec3( x, 0, -gridSize ) );
			gridVerts.push_back( vec3( x, 0, gridSize ) );
			gridVerts.push_back( vec3( -gridSize, 0, x ) );
			gridVerts.push_back( vec3( gridSize, 0, x ) );

			normals.push_back( vec3( 1.f ) );
			normals.push_back( vec3( 1.f ) );
			normals.push_back( vec3( 1.f ) );
			normals.push_back( vec3( 1.f ) );
		}

		m_Grid.m_DrawMode = GL_LINES;
		m_Grid.SetVertices( gridVerts );
	}

	CreateTempMeshTree( );
	FBXLoader::Load( "anim.fbx", m_MeshTree );
	FBXLoader::Load( "anim.fbx", m_Animation );

	m_Animator.SetAnimation( &m_Animation );
	m_Nodes[0] = m_MeshTree.GetRoot( )->GetChild( 0 );
	//m_Nodes[1] = m_Nodes[0]->GetChild( 0 );

	m_Shader.use( );
}


void CProgram::Update( float delta ) {
	static float t = 0.f;
	static float rSpeed = 0.1f;
	t += delta;

	m_Camera.position = glm::vec3( sin( t * rSpeed ), 0.4f, cos( t * rSpeed ) ) * 6.f;
	m_Camera.direction = -m_Camera.position;

	m_Animator.Update( delta );

	/*CMeshTree::Node* node = m_Nodes[0];

	while (node != NULL) {
		node->m_Transform.rotation.y = t * 40.f;
		node->m_Transform.rotation.z = t * 10.f;
		node = node->GetChildCount( ) > 0 ? node->GetChild( 0 ) : NULL;
	}*/
}

void CProgram::Render( float delta ) {
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m_Shader.set( "u_camera", m_Camera.getMatrix( ) );

	float ratio = m_Camera.getAspectRatio( );
	mat4 o = ortho( -8.f * ratio, 8.f * ratio, -8.f, 8.f, -40.f, 40.f );

	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num1 )) {
		m_Shader.set( "u_camera", o * lookAt( vec3( 0.f, 0.f, 8.f ), vec3( 0.f ), vec3( 0.f, 1.f, 0.f ) ) );
	}
	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num2 )) {
		m_Shader.set( "u_camera", o * lookAt( vec3( 8.f, 0.f, 0.f ), vec3( 0.f ), vec3( 0.f, 1.f, 0.f ) ) );
	}
	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num3 )) {
		m_Shader.set( "u_camera", o * lookAt( vec3( 0.f, 8.f, 0.f ), vec3( 0.f ), vec3( 0.f, 0.f, -1.f ) ) );
	}

	m_Shader.set( "u_model", mat4( 1.f ) );
	m_Grid.Draw( );
	//m_Shader.set( "u_model", mat4( 1.f ) );
	m_MeshTree.Draw( m_Shader );

	glDisable( GL_DEPTH_TEST );
	CTreeRenderer::GetShader( ).set( "u_camera", m_Camera.getMatrix( ) );
	m_TreeRenderer.RenderTree( m_MeshTree.GetRoot( ) );
}

void CProgram::KeyEvent( sf::Keyboard::Key key, int action ) {
	if (key == sf::Keyboard::Escape)
		m_Framework->Exit( );
}

void CProgram::CreateTempMeshTree( ) {
	CMeshTree::Node* node = new CMeshTree::Node( "ROOT" );
	m_MeshTree.SetRoot( node );

	for (int i = 0; i < 3; i++) {
		vec3 verts[]{
			vec3( -0.5f, -0.5f, float( i ) ),
			vec3( 0.5f, -0.5f, float( i ) ),
			vec3( 0.5f, 0.5f, float( i ) ),

			vec3( -0.5f, -0.5f, float( i ) ),
			vec3( 0.5f, 0.5f, float( i ) ),
			vec3( -0.5f, 0.5f, float( i ) ),
		};

		vec3 norm[]{
			vec3( 0.f, 0.f, 1.f ),
			vec3( 0.f, 0.f, 1.f ),
			vec3( 0.f, 0.f, 1.f ),

			vec3( 0.f, 0.f, 1.f ),
			vec3( 0.f, 0.f, 1.f ),
			vec3( 0.f, 0.f, 1.f ),
		};

		CMesh* mesh = new CMesh;
		if (i == 0)
			FBXLoader::Load( "test.fbx", *mesh );
		else {
			mesh->SetVertices( verts, sizeof( verts ) );
			mesh->SetNormals( norm, sizeof( norm ) );
		}

		CMeshTree::Node* mNode = new CMeshTree::Node( "ALLAH" );
		mNode->SetPivot( glm::vec3( 0.f, 0.f, float( i ) ) );

		node->AddChild( mNode );
		node = mNode;

		m_Nodes[i] = mNode;
	}
}
