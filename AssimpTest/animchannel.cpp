#include "stdafx.h"
#include "animchannel.h"

CAnimationChannel::Key CAnimationChannel::Key::Lerp( const Key& a, const Key& b, float x ) {
	x = clamp( x, 0.f, 1.f );
	return{ lerp( a.m_Position, b.m_Position, x ) };
}

CAnimationChannel::CAnimationChannel( const std::string & nodeName ) :
	m_NodeName( nodeName ) {
}

void CAnimationChannel::AddKey( const Key& value ) {
	m_Keys.push_back( value );
}

const CAnimationChannel::Key CAnimationChannel::GetValue( float frame ) const {
	Key k;

	if (frame < 0 || m_Keys.size( ) == 0)
		return k;

	size_t iFrame = (size_t)frame % m_Keys.size( );
	size_t iNext = (iFrame + 1) % m_Keys.size( );

	float interp = float( frame - iFrame );
	k = Key::Lerp( m_Keys[iFrame], m_Keys[iNext], interp );

	return k;
}