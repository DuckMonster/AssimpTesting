#pragma once
#include "mesh.h"
#include "nodetransform.h"

class CMeshTree {
public:
	class Node {
	public:
		Node( const std::string name );
		~Node( );

		const std::string& GetName( ) { return m_Name; }

		void Draw( glt::Shader& shader, CMeshTree* tree );
		void Draw( glt::Shader& shader, glm::mat4 matrix, CMeshTree* tree );

		Node* AddChild( Node* node );

		Node* GetParent( ) const { return m_Parent; }

		Node* GetChild( size_t index ) { return m_Children[index]; }
		size_t GetChildCount( ) { return m_Children.size( ); }
		std::vector<Node*>& GetChildren( ) { return m_Children; }

		Node* FindNode( const std::string& name );

		void SetTransform( const glm::mat4& mat );
		void SetTransform( const CNodeTransform& transform ) { m_Transform = transform; }
		void SetRotationAxes( const glm::mat4& mat ) { m_RotationAxes = mat; }
		const glm::mat4& GetRotationAxes( ) { return m_RotationAxes; }

		const CNodeTransform& GetTransform( ) { return m_Transform; }
		const glm::mat4 GetLocalSpace( ) { return m_Transform.GetTransform( m_RotationAxes ); }

		void SetMesh( size_t mesh ) { m_MeshIndex = mesh; }

	private:
		const std::string	m_Name;

		CNodeTransform		m_Transform;
		glm::mat4			m_RotationAxes;

		int					m_MeshIndex;
		Node*				m_Parent;
		std::vector<Node*>	m_Children;
	};

public:
	CMeshTree( );
	~CMeshTree( );

	Node* GetRoot( ) { return m_Root; }
	void SetRoot( Node* node );

	Node* FindNode( const std::string name ) { return GetRoot( )->FindNode( name ); }

	CMesh* GetMesh( size_t index ) { return m_MeshList[index]; }
	void AddMesh( CMesh* mesh ) { m_MeshList.push_back( mesh ); }

	void Draw( glt::Shader& shader );

private:

	Node*				m_Root;
	std::vector<CMesh*>	m_MeshList;
};