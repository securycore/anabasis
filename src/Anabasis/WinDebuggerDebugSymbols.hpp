/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>

namespace Anabasis {

	class CWinDebuggerDebugSymbols {
	public:
		CWinDebuggerDebugSymbols();
		~CWinDebuggerDebugSymbols();

		IDebugSymbols* symbols_ = nullptr;
	};

}