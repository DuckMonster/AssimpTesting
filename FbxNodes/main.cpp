#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/types.h>
#include <assimp/scene.h>

#include <map>
#include <regex>
using namespace std;

class AnimNode {
public:
	static AnimNode* Create( aiNode* base, AnimNode* parent );

public:
	AnimNode( string name, AnimNode* parent ) : m_Name( name ), m_Parent( parent ) {}

	vector<size_t>				m_Meshes;
	std::string					m_Name;
	AnimNode*					m_Parent;
	vector<AnimNode*>			m_Children;
	map<string, aiMatrix4x4>	m_Attributes;
};

string GetNodeName( aiNode* node ) {
	string str( node->mName.C_Str( ) );
	smatch match;
	regex expr( "^(\\w+)_\\$" );

	if (regex_search( str, match, expr )) {
		return match[1];
	}
	else return str;
}

string GetNodeData( aiNode* node ) {
	string str( node->mName.C_Str( ) );
	smatch match;
	regex expr( "_(\\w+)$" );

	if (regex_search( str, match, expr )) {
		return match[1];
	}
	else return "";
}

void ProcessNode( aiNode* node, int indents ) {
	for (size_t i = 0; i < indents; i++) {
		cout << " ";
	}
	cout << node->mName.C_Str( ) << "\n";

	for (size_t i = 0; i < node->mNumChildren; i++) {
		ProcessNode( node->mChildren[i], indents + 1 );
	}
}

void ProcessNode( AnimNode* node, int indents ) {
	for (size_t i = 0; i < indents; i++) {
		cout << "  ";
	}

	cout << node->m_Name << " | [ ";

	for (auto m : node->m_Meshes) {
		cout << m << ", ";
	}

	cout << "\b\b ] ";
	cout << "[ " << node->m_Attributes.size( ) << " ]\n";

	for (auto c : node->m_Children) {
		ProcessNode( c, indents + 1 );
	}
}

void ProcessAnimation( aiAnimation* animation ) {
	cout << "ANIMATION " << animation->mName.C_Str( ) << "\n";
	for (size_t i = 0; i < animation->mNumChannels; i++) {
		aiNodeAnim* anim = animation->mChannels[i];
		cout << anim->mNodeName.C_Str( ) << " [ ";
		cout << anim->mNumPositionKeys << ", " << anim->mNumRotationKeys << ", " << anim->mNumScalingKeys << " ]\n";
	}


}

int main( ) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( "anim.fbx", aiProcess_Triangulate );

	ProcessNode( scene->mRootNode, 0 );
	AnimNode* base = AnimNode::Create( scene->mRootNode, NULL );

	ProcessNode( base, 0 );

	for (size_t i = 0; i < scene->mNumAnimations; i++) {
		ProcessAnimation( scene->mAnimations[i] );
		cout << "\n\n";
	}

	cin.get( );
}

AnimNode* AnimNode::Create( aiNode* base, AnimNode* parent ) {
	AnimNode* node = new AnimNode( GetNodeName( base ), parent );
	cout << "Node " << node->m_Name << " created!\n";

	// Read all attributes
	while (true) {
		string attrib = GetNodeData( base );
		if (attrib == "")
			break;

		cout << node->m_Name << "." << attrib << "\n";

		node->m_Attributes[attrib] = base->mTransformation;
		base = base->mChildren[0];
	}

	for (size_t i = 0; i < base->mNumMeshes; i++) {
		node->m_Meshes.push_back( base->mMeshes[i] );
	}

	for (size_t i = 0; i < base->mNumChildren; i++) {
		node->m_Children.push_back( Create( base->mChildren[i], node ) );
	}

	return node;
}
