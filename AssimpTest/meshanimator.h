#pragma once
#include "meshtree.h"
#include "animation.h"

class CMeshAnimator {
public:
	CMeshAnimator( CMeshTree& tree );
	void SetAnimation( CAnimation* animation ) { m_Animation = animation; }

	void Update( float delta );

private:
	void ApplyAnimation( );

	CAnimation*	m_Animation;
	CMeshTree&	m_Tree;
};