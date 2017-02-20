#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <glm/glm.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace rapidxml;

void printNode( xml_node<>* node ) {
	cout << node->name( ) << "\n";
	for (xml_attribute<>* attr = node->first_attribute( ); attr; attr = attr->next_attribute( )) {
		cout << "\t";
		cout << attr->name( ) << " = " << attr->value( ) << "\n";
	}

	cout << "\n\n";
	cout << "\t";
	cout << node->value( );
	cout << "\n";
}

struct Mesh {
	std::vector<glm::vec3>	m_Vertices;
	std::vector<glm::vec3>	m_Normals;
	std::vector<glm::vec2>	m_Uvs;
	std::vector<size_t>		m_Indicies;
};

std::map<std::string, Mesh> m_Meshes;

void LoadMesh( xml_node<>* node ) {
	std::string id( node->first_attribute( "id" )->value( ) );


}

void LoadGeometries( xml_node<>* node ) {

}

int main( ) {
	ifstream stream( "file.dae" );
	vector<char> buffer( (istreambuf_iterator<char>( stream )), istreambuf_iterator<char>( ) );
	buffer.push_back( '\0' );

	xml_document<char> doc;
	doc.parse<0>( &buffer[0] );

	xml_node<>* node = doc.first_node( );
	printNode( node );
	printNode( node->first_node( "library_geometries" )->first_node( )->first_node( )->first_node( )->first_node( ) );

	system( "pause" );
	//rapidxml::print( cout, doc );
}