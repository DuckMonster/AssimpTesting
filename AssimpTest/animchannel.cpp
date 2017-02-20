#include "stdafx.h"
#include "animchannel.h"

CAnimationChannel::Key CAnimationChannel::Key::Lerp( const Key& a, const Key& b, float x ) {
	x = clamp( x, 0.f, 1.f );

	return{
		lerp( a.m_Position, b.m_Position, x ),
		slerp( a.m_Rotation, b.m_Rotation, x ),
		lerp( a.m_Scale, b.m_Scale, x )
	};
}

CAnimationChannel::CAnimationChannel( const std::string & nodeName, const size_t length ) :
	m_NodeName( nodeName ),
	m_Length( length ),
	m_Keys( NULL ) {

	if (length != 0)
		m_Keys = new Key[length];
}

CAnimationChannel::~CAnimationChannel( ) {
	if (m_Keys != NULL)
		delete[] m_Keys;

	m_Keys = NULL;
}

const CAnimationChannel::Key CAnimationChannel::GetValue( float frame ) const {
	Key k;

	if (frame < 0 || m_Length == 0)
		return k;

	size_t iFrame = (size_t)frame % m_Length;
	size_t iNext = (iFrame + 1) % m_Length;

	float interp = float( frame - iFrame );
	k = Key::Lerp( m_Keys[iFrame], m_Keys[iNext], interp );

	return k;
}