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
	for (const CAnimationChannel* channel : *m_Animation) {
		CAnimationChannel::Key key = channel->GetValue( m_Animation->GetTime( ) );
		std::string nodeName = channel->GetNodeName( );

		CMeshTree::Node* node = m_Tree.FindNode( nodeName );
		//Transform transform( key.m_Position, eulerAngles( key.m_Rotation ), key.m_Scale );

		vec3 rotEuler = eulerAngles( key.m_Rotation );
		//std::cout << "{ " << key.m_Position.x << ", " << key.m_Position.y << ", " << key.m_Position.z << " }\n";

		if (node) {
			CNodeTransform transform;

			transform.m_Position	= key.m_Position;
			transform.m_Rotation	= key.m_Rotation;
			transform.m_Scale		= key.m_Scale;

			node->SetTransform( transform );
			//node->m_Transform = transform;
		}
	}
}