#include "stdafx.h"
#include "mesh.h"

CMesh::CMesh( ) :
	m_DrawMode( GL_TRIANGLES ),
	m_VAO( ), m_VertexBuffer( 3 ), m_NormalBuffer( 3 ), m_UVBuffer( 2 ), m_ElemBuffer( ),
	m_ElemSize( 0 ), m_VertSize( 0 ) {

	m_VAO.bind( );

	m_VAO.bindBufferToAttr( &m_VertexBuffer, 0 );
	m_VAO.bindBufferToAttr( &m_NormalBuffer, 1 );
	m_VAO.bindBufferToAttr( &m_UVBuffer, 2 );
	m_ElemBuffer.bind( );

	glt::VAO::release( );
}

void CMesh::SetVertices( const std::vector<glm::vec3>& vertices ) {
	m_VertexBuffer.setData( vertices );
	m_VertSize = vertices.size( );
}

void CMesh::SetVertices( void* ptr, size_t size ) {
	m_VertexBuffer.setData( reinterpret_cast<vec3*>(ptr), size );
	m_VertSize = size / sizeof( vec3 );
}

void CMesh::SetNormals( const std::vector<glm::vec3>& normals ) {
	m_NormalBuffer.setData( normals );
}

void CMesh::SetNormals( void * ptr, size_t size ) {
	m_NormalBuffer.setData( reinterpret_cast<vec3*>(ptr), size );
}

void CMesh::SetUVs( const std::vector<glm::vec2>& uvs ) {
	m_UVBuffer.setData( uvs );
}

void CMesh::SetUVs( void * ptr, size_t size ) {
	m_UVBuffer.setData( reinterpret_cast<vec2*>(ptr), size );
}

void CMesh::SetElements( const std::vector<size_t>& elements ) {
	m_ElemBuffer.setData( elements );
	m_ElemSize = elements.size( );
}

void CMesh::SetElements( void * ptr, size_t size ) {
	m_ElemBuffer.setData( reinterpret_cast<size_t*>(ptr), size );
	m_ElemSize = size / sizeof( size_t );
}

void CMesh::Draw( ) {
	FillBuffers( );

	m_VAO.bind( );

	if (m_ElemSize > 0)
		glDrawElements( m_DrawMode, m_ElemSize, GL_UNSIGNED_INT, NULL );
	else
		glDrawArrays( m_DrawMode, 0, m_VertSize );

	glt::VAO::release( );
}

void CMesh::FillBuffers( ) {
	m_NormalBuffer.setVertexCount( m_VertSize, vec3( 1.f ) );
	m_UVBuffer.setVertexCount( m_VertSize, vec2( 0.f ) );
}
