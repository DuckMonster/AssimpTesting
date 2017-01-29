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

void CAnimation::AddChannel( const CAnimationChannel& channel ) {
	m_Channels.push_back( channel );
}

void CAnimation::Update( float delta ) {
	m_Time = mod( m_Time + delta * m_Framerate, m_Length );
}