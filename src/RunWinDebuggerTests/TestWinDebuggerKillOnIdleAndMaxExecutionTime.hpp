/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include "TestWinDebugger.hpp"

class CTestWinDebuggerKillOnIdleAndMaxExecutionTime : public CTestWinDebugger {
public:
	CTestWinDebuggerKillOnIdleAndMaxExecutionTime();
	~CTestWinDebuggerKillOnIdleAndMaxExecutionTime();

	void run();
};
