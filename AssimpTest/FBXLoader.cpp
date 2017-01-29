#include "stdafx.h"
#include "FBXLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <regex>

using namespace std;

namespace {
	string GetNodeName( const std::string node ) {
		string str( node );
		smatch match;
		regex expr( "^(\\w+)_\\$" );

		if (regex_search( str, match, expr )) {
			return match[1];
		}
		else return str;
	}
	string GetNodeName( const aiNode* node ) { return GetNodeName( node->mName.C_Str( ) ); }
	string GetNodeName( const aiNodeAnim* node ) { return GetNodeName( node->mNodeName.C_Str( ) ); }

	string GetNodeData( const std::string node ) {
		string str( node );
		smatch match;
		regex expr( "_(\\w+)$" );

		if (regex_search( str, match, expr )) {
			return match[1];
		}
		else return "";
	}
	string GetNodeData( const aiNode* node ) { return GetNodeData( node->mName.C_Str( ) ); }
	string GetNodeData( const aiNodeAnim* node ) { return GetNodeData( node->mNodeName.C_Str( ) ); }

	void ProcessNode( aiNode* node, int indents ) {
		for (size_t i = 0; i < indents; i++) {
			cout << " ";
		}
		cout << node->mName.C_Str( ) << "\n";

		for (size_t i = 0; i < node->mNumChildren; i++) {
			ProcessNode( node->mChildren[i], indents + 1 );
		}
	}

	template<typename OutT, typename InT>
	OutT convert( const InT& value ) {
		OutT result;

		for (size_t i = 0; i < result.length( ); i++)
			result[i] = value[i];

		return result;
	}

	mat4 convert( const aiMatrix4x4& mat ) {
		mat4 result;

		for (size_t i = 0; i < result.length( ); i++) {
			for (size_t j = 0; j < result[i].length( ); j++) {
				result[i][j] = mat[i][j];
			}
		}

		return result;
	}
}

void FBXLoader::Load( const string file, CMesh& mesh ) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate );
	if (!scene) {
		cout << "Failed to load " << file << ": " << importer.GetErrorString( ) << "\n";
		return;
	}

	ParseMesh( scene, 0, scene->mNumMeshes, mesh );
	return;
}

void FBXLoader::Load( const string file, CMeshTree & tree ) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate );
	if (!scene) {
		cout << "Failed to load " << file << ": " << importer.GetErrorString( ) << "\n";
		return;
	}

	ProcessNode( scene->mRootNode, 0 );

	// Parse meshes
	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		CMesh* mesh = new CMesh( );
		ParseMesh( scene, i, 1, *mesh );

		tree.AddMesh( mesh );
	}

	// Parse nodes
	CMeshTree::Node* node = new CMeshTree::Node( GetNodeName( scene->mRootNode ) );
	ParseNode( scene->mRootNode, node );

	tree.SetRoot( node );

	return;
}

void FBXLoader::Load( const std::string file, CAnimation& animation ) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate );
	if (!scene) {
		cout << "Failed to load " << file << ": " << importer.GetErrorString( ) << "\n";
		return;
	}

	for (size_t a = 0; a < scene->mNumAnimations; a++) {
		aiAnimation* fbxAnimation = scene->mAnimations[a];
		animation = CAnimation( fbxAnimation->mDuration, fbxAnimation->mTicksPerSecond );

		for (size_t c = 0; c < fbxAnimation->mNumChannels; c++) {
			aiNodeAnim* fbxChannel = fbxAnimation->mChannels[c];

			std::string nodeName = GetNodeName( fbxChannel );
			std::string dataName = GetNodeData( fbxChannel );

			if (dataName == "" || dataName == "Translation") {
				CAnimationChannel chnl( nodeName );

				for (size_t p = 0; p < fbxChannel->mNumPositionKeys; p++) {
					aiVector3D value = fbxChannel->mPositionKeys[p].mValue;
					chnl.AddKey( { convert<vec3>( value ) } );
				}

				animation.AddChannel( chnl );
			}
		}
	}

	return;
}

void FBXLoader::ParseMesh( const aiScene* scene, const size_t index, const size_t count, CMesh& mesh ) {
	vector<aiVector3D> vertices;
	vector<aiVector3D> normals;
	vector<aiVector2D> uvs;
	vector<size_t> indicies;

	size_t elem_offset = 0;

	for (size_t m = 0; m < count; m++) {
		aiMesh* fbxMesh = scene->mMeshes[index + m];

		for (size_t i = 0; i < fbxMesh->mNumVertices; i++) {
			vertices.push_back( fbxMesh->mVertices[i] );

			if (fbxMesh->HasNormals( ))
				normals.push_back( fbxMesh->mNormals[i] );
			if (fbxMesh->HasTextureCoords( 0 )) {
				aiVector3D v = fbxMesh->mTextureCoords[0][i];
				uvs.push_back( aiVector2D( v.x, v.y ) );
			}
		}

		for (size_t f = 0; f < fbxMesh->mNumFaces; f++) {
			aiFace face = fbxMesh->mFaces[f];

			for (size_t i = 0; i < face.mNumIndices; i++) {
				indicies.push_back( face.mIndices[i] + elem_offset );
			}
		}

		elem_offset += fbxMesh->mNumVertices;
	}

	mesh.SetVertices( &vertices[0].x, sizeof( vertices[0] ) * vertices.size( ) );
	mesh.SetNormals( &normals[0].x, sizeof( normals[0] ) * normals.size( ) );
	mesh.SetUVs( &uvs[0].x, sizeof( uvs[0] ) * vertices.size( ) );
	mesh.SetElements( &indicies[0], sizeof( indicies[0] ) * indicies.size( ) );
}

void FBXLoader::ParseNode( const aiNode* node, CMeshTree::Node* targetNode ) {
	std::string data( GetNodeData( node ) );

	// Loop through nodes to fetch all data in FBX parser
	while (data != "") {
		if (data == "RotationPivot")
			targetNode->SetPivot( transpose( convert( node->mTransformation ) ) );
		if (data == "PreRotation")
			targetNode->SetBaseTransform( transpose( convert( node->mTransformation ) ) );

		node = node->mChildren[0];
		data = GetNodeData( node );
	}

	// Set meshes
	for (size_t i = 0; i < node->mNumMeshes; i++)
		targetNode->SetMesh( node->mMeshes[i] );

	// Base transform
	if (!node->mTransformation.IsIdentity( ))
		targetNode->SetBaseTransform( transpose( convert( node->mTransformation ) ) );

	// We made it! To the actual node
	for (size_t i = 0; i < node->mNumChildren; i++) {
		aiNode* child = node->mChildren[i];

		CMeshTree::Node* newNode = new CMeshTree::Node( GetNodeName( child ) );
		ParseNode( child, newNode );

		targetNode->AddChild( newNode );
	}
}