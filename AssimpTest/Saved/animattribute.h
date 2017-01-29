#pragma once
#include <glm/gtx/compatibility.hpp>

template<typename T>
class CAnimationAttribute {
private:
	struct Key {
		T	value;
	};

public:
	CAnimationAttribute( const T& defValue );

	void AddKey( T value );
	T GetValue( float frame );

private:
	T					m_DefValue;
	std::vector<Key>	m_Keys;
};

template<typename T>
inline CAnimationAttribute<T>::CAnimationAttribute( const T& defValue ) :
	m_DefValue( defValue ), m_Keys( ) {
}

template<typename T>
inline void CAnimationAttribute<T>::AddKey( T value ) {
	m_Keys.push_back( { value } );
}

template<typename T>
inline T CAnimationAttribute<T>::GetValue( float frame ) {
	T value( m_DefValue );

	if (frame < 0 || m_Keys.size( ) == 0)
		return value;

	size_t iFrame = (size_t)frame % m_Keys.size( );
	size_t iNext = (iFrame + 1) % m_Keys.size( );

	float interp = float( frame - iFrame );
	value = lerp( m_Keys[iFrame].value, m_Keys[iNext].value, interp );

	return value;
}
