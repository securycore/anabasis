/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include "TestWinDebugger.hpp"

class CTestWinDebuggerAttachProcess : public CTestWinDebugger {
public:
	CTestWinDebuggerAttachProcess();
	~CTestWinDebuggerAttachProcess();

	void run();
};
