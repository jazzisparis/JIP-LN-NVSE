#include "nvse/GameTasks.h"

// IOManager** g_ioManager = (IOManager**)0x00B33A10;
UInt32 * kBSTaskCounter = (UInt32*) 0x0011C3B38;

ModelLoader *ModelLoader::GetSingleton()
{
	return *(ModelLoader**)0x011C3B3C;
}

UInt32 *BSTask::GetCounterSingleton()
{
	return kBSTaskCounter;
}

