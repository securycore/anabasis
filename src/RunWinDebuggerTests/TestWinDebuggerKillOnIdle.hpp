/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include "TestWinDebugger.hpp"

class CTestWinDebuggerKillOnIdle : public CTestWinDebugger {
public:
	CTestWinDebuggerKillOnIdle();
	~CTestWinDebuggerKillOnIdle();

	void run();
};
