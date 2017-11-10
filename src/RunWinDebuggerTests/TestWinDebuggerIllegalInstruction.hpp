/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include "TestWinDebugger.hpp"

class CTestWinDebuggerIllegalInstruction : public CTestWinDebugger {
public:
	CTestWinDebuggerIllegalInstruction();
	~CTestWinDebuggerIllegalInstruction();

	void run();
};
