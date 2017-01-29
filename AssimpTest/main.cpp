#include "stdafx.h"
#include "framework.h"
#include "program.h"

int main( ) {
	CFramework framework( 1024, 768, "Assimp Testing" );
	CProgram program( &framework );

	framework.Run( program );

	return 0;
}