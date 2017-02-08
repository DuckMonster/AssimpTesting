#include "stdafx.h"
#include "meshanimator.h"

CMeshAnimator::CMeshAnimator( CMeshTree& tree ) :
	m_Tree( tree ), m_Animation( NULL ) {
}

void CMeshAnimator::Update( float delta ) {
	if (!m_Animation)
		return;

	m_Animation->Update( delta );
	ApplyAnimation( );
}

void CMeshAnimator::ApplyAnimation( ) {
	for (const CAnimationChannel& channel : *m_Animation) {
		CAnimationChannel::Key key = channel.GetValue( m_Animation->GetTime( ) );
		std::string nodeName = channel.GetNodeName( );

		CMeshTree::Node* node = m_Tree.FindNode( nodeName );
		Transform transform( key.m_Position, vec3( 0.f ), vec3( 1.f ) );

		if (node) {
			node->SetBaseTransform( transform.getMatrix( ) );
			//node->m_Transform = transform;
		}
	}
}