/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include <windows.h>

void main() {
	void* mem = VirtualAlloc(NULL, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READONLY | PAGE_GUARD);
	if (!mem) {
		return;
	}

	*(int*)mem = 0;
}