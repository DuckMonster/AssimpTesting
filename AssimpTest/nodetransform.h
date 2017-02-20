#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CNodeTransform {
public:
	glm::vec3	m_Position;
	glm::quat	m_Rotation;
	glm::vec3	m_Scale;

	CNodeTransform( ) :
		m_Position( 0.f ), m_Rotation( ), m_Scale( 1.f ) 
	{}

	CNodeTransform( glm::vec3 position, glm::quat rotation, glm::vec3 scale ) :
		m_Position( position ), m_Rotation( rotation ), m_Scale( scale ) 
	{}

	glm::mat4 GetTransform( ) {
		glm::mat4 scale = glm::scale( glm::mat4( 1.f ), m_Scale ),
			rotation = (glm::mat4)m_Rotation;

		glm::mat4 mat = rotation * scale;
		mat[3][0] = m_Position.x;
		mat[3][1] = m_Position.y;
		mat[3][2] = m_Position.z;

		return mat;
	}

	glm::mat4 GetTransform( const glm::mat4& rotationAxes ) {
		glm::mat4 rotation = rotationAxes * (glm::mat4)m_Rotation;

		glm::mat4 mat = rotation;
		mat[3][0] = m_Position.x;
		mat[3][1] = m_Position.y;
		mat[3][2] = m_Position.z;

		return mat;
	}
};