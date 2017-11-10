/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason_for_call, LPVOID reserved) {
	switch (reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
