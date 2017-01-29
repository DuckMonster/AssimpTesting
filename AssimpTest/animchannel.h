#pragma once
#include <glm/gtx/compatibility.hpp>

class CAnimationChannel {
public:
	struct Key {
		vec3	m_Position;
		static Key Lerp( const Key& a, const Key& b, float x );
	};

public:
	CAnimationChannel( const std::string& nodeName );

	void AddKey( const Key& value );

	const std::string& GetNodeName( ) const { return m_NodeName; }
	const Key GetValue( float frame ) const;

private:
	std::string			m_NodeName;
	std::vector<Key>	m_Keys;
};