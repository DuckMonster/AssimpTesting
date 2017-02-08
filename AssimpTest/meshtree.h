#pragma once
#include "mesh.h"

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

		void SetPivot( const glm::vec3& world );
		void SetPivot( const glm::mat4& mat );
		void SetBaseTransform( const glm::mat4& mat ) { m_BaseTransform = mat; }
		void SetLocalAxes( const glm::mat4& mat ) { m_LocalAxes = mat; }

		const glm::vec3 GetPivotPoint( );
		const glm::mat4 GetPivotMatrix( );
		const glm::vec3 GetTransformedPivot( );
		const glm::mat4 GetTransform( );
		const glm::mat4 GetLocalAxes( ) { return m_LocalAxes; }

		void SetMesh( size_t mesh ) { m_MeshIndex = mesh; }

		glt::Transform		m_Transform;

	private:
		const std::string	m_Name;

		glm::mat4			m_BaseTransform;
		glm::mat4			m_LocalAxes;
		glm::mat4			m_Pivot;
		glm::mat4			m_PivotInverse;

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