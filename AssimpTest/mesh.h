#pragma once
class CMesh {
public:
	CMesh( );
	void SetVertices( const std::vector<glm::vec3>& vertices );
	void SetVertices( void* ptr, size_t size );
	void SetNormals( const std::vector<glm::vec3>& normals );
	void SetNormals( void* ptr, size_t size );
	void SetUVs( const std::vector<glm::vec2>& uvs );
	void SetUVs( void* ptr, size_t size );
	void SetElements( const std::vector<size_t>& elements );
	void SetElements( void* ptr, size_t size );

	void Draw( );

	GLuint					m_DrawMode;

private:
	void FillBuffers( );

	glt::VAO				m_VAO;
	glt::VBO<glm::vec3>		m_VertexBuffer;
	glt::VBO<glm::vec3>		m_NormalBuffer;
	glt::VBO<glm::vec2>		m_UVBuffer;
	glt::EBO				m_ElemBuffer;

	size_t					m_ElemSize;
	size_t					m_VertSize;
};