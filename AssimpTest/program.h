#pragma once
#include "framework.h"
#include "mesh.h"
#include "meshtree.h"
#include "treerenderer.h"
#include "animation.h"
#include "meshanimator.h"

class CProgram : public IProgram {
public:
	CProgram( CFramework* framework );

	void Load( ) override;
	void Update( float delta ) override;
	void Render( float delta ) override;
	void KeyEvent( sf::Keyboard::Key key, int action ) override;

private:
	void CreateTempMeshTree( );
	CFramework*		m_Framework;

	glt::Shader		m_Shader;
	glt::Camera		m_Camera;

	CMesh			m_Grid;
	CMeshTree		m_MeshTree;
	CTreeRenderer	m_TreeRenderer;
	CMeshTree::Node*	m_Nodes[3];
	CAnimation		m_Animation;
	CMeshAnimator	m_Animator;
};