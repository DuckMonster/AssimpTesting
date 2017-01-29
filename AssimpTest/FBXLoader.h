#pragma once
#include "mesh.h"
#include "meshtree.h"
#include "animation.h"
#include <assimp/scene.h>

class FBXLoader {
public:
	static void Load( const std::string file, CMesh& mesh );
	static void Load( const std::string file, CMeshTree& tree );
	static void Load( const std::string file, CAnimation& animation );

private:
	static void ParseMesh( const aiScene* scene, const size_t index, const size_t count, CMesh& mesh );
	static void ParseNode( const aiNode* node, CMeshTree::Node* treeNode );
};