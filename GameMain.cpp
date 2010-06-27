#include "TGameManager.h"
#include "TGameMenuMain.h"
#include "TGamePlay.h"
using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char **argv)
#endif
{
	new TGameManager();
	
	if(!TGameManager::getSingletonPtr()->start(TGameMenuMain::getInst()))
		return 0;

	return 0; 
}