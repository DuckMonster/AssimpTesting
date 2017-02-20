#include "stdafx.h"
#include "animation.h"

CAnimation::CAnimation( ) : CAnimation( 0.f, 0.f ) {
}

CAnimation::CAnimation( float length, float framerate ) :
	m_Time( 0.f ), m_Length( length ), m_Framerate( framerate ),
	m_Channels( ) {
}

CAnimation::~CAnimation( ) {
}

CAnimationChannel* CAnimation::GetChannel( const std::string nodeName ) {
	// Search for nodename
	for (CAnimationChannel* chnl : m_Channels) {
		if (chnl->GetNodeName( ) == nodeName)
			return chnl;
	}

	// Add new channel if there is none existing
	m_Channels.push_back( new CAnimationChannel( nodeName, m_Length + 1 ) );
	return m_Channels.back( );
}

void CAnimation::Update( float delta ) {
	m_Time = mod( m_Time + delta * m_Framerate, m_Length );
}