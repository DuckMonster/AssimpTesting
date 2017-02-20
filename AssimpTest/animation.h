#pragma once
#include "animchannel.h"

class CAnimation {
public:
	typedef std::vector<CAnimationChannel*>::iterator iterator;
	typedef std::vector<CAnimationChannel*>::const_iterator const_iterator;

public:
	CAnimation( );
	CAnimation( float length, float framerate );
	~CAnimation( );

	CAnimationChannel* GetChannel( const std::string nodeName );
	void Update( float delta );

	float GetTime( ) { return glm::mod( m_Time, m_Length ); }

	// ITERATION
	iterator begin( ) { return m_Channels.begin( ); }
	iterator end( ) { return m_Channels.end( ); }
	const_iterator cbegin( ) { return m_Channels.cbegin( ); }
	const_iterator cend( ) { return m_Channels.cend( ); }

private:
	float m_Time;
	float m_Length;
	float m_Framerate;

	std::vector<CAnimationChannel*> m_Channels;
};