#pragma once
#include <glm/gtx/compatibility.hpp>

class CAnimationChannel {
public:
	struct Key {
		vec3	m_Position;
		quat	m_Rotation;
		vec3	m_Scale;

		Key( ) : m_Position( 0.f ), m_Rotation( ), m_Scale( 1.f ) {}
		Key( vec3 pos, quat rot, vec3 scale ) :
			m_Position( pos ),
			m_Rotation( rot ),
			m_Scale( scale ) {
		}

		static Key Lerp( const Key& a, const Key& b, float x );
	};

public:
	CAnimationChannel( const std::string& nodeName, const size_t length );
	~CAnimationChannel( );

	Key& GetKey( const size_t index ) { if (index >= m_Length) throw std::exception( ); return m_Keys[index]; }

	const std::string& GetNodeName( ) const { return m_NodeName; }
	const Key GetValue( float frame ) const;

private:
	std::string			m_NodeName;

	size_t				m_Length;
	Key*				m_Keys;
};