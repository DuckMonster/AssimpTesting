#pragma once
#include "animattribute.h"

class CAnimation {
public:
	CAnimation( );
	CAnimation( float length, float framerate );

	void AddPositionKey( const vec3& key ) { m_PositionAttribute.AddKey( key ); }
	vec3 GetPositionValue( );

	void AddScaleKey( const vec3& key ) { m_ScaleAttribute.AddKey( key ); }
	vec3 GetScaleValue( );

	void AddRotationKey( const quat& key ) { m_RotationAttribute.AddKey( key ); }
	quat GetRotationValue( );

	mat4 GetTransform( );

	void Update( float delta );

private:
	float m_Time;
	float m_Length;
	float m_Framerate;

	CAnimationAttribute<vec3> m_PositionAttribute;
	CAnimationAttribute<vec3> m_ScaleAttribute;
	CAnimationAttribute<quat> m_RotationAttribute;
};