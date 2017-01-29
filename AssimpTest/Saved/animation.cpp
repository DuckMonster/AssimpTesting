#include "stdafx.h"
#include "animation.h"

CAnimation::CAnimation( ) : CAnimation( 0.f, 0.f ) {
}

CAnimation::CAnimation( float length, float framerate ) :
	m_Time( 0.f ), m_Length( length ), m_Framerate( framerate ),
	m_PositionAttribute( vec3( 0.f ) ),
	m_ScaleAttribute( vec3( 1.f ) ),
	m_RotationAttribute( quat( ) ) {
}

vec3 CAnimation::GetPositionValue( ) {
	return m_PositionAttribute.GetValue( m_Time );
}

vec3 CAnimation::GetScaleValue( ) {
	return m_ScaleAttribute.GetValue( m_Time );
}

quat CAnimation::GetRotationValue( ) {
	return m_RotationAttribute.GetValue( m_Time );
}

mat4 CAnimation::GetTransform( ) {
	/*return translate( mat4( 1.f ), GetPositionValue( ) ) *
		scale( mat4( 1.f ), GetScaleValue( ) ) *
		mat4( GetRotationValue( ) );*/

	mat4 t = translate( mat4( 1.f ), GetPositionValue( ) );
	mat4 s = scale( mat4( 1.f ), GetScaleValue( ) );
	mat4 r = mat4( GetRotationValue( ) );

	return t * r * s;
}

void CAnimation::Update( float delta ) {
	m_Time = mod( m_Time + delta * m_Framerate, m_Length );
}