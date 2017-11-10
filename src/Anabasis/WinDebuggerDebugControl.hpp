/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>

namespace Anabasis {

	class CWinDebuggerDebugControl {
	public:
		CWinDebuggerDebugControl();
		~CWinDebuggerDebugControl();

		IDebugControl* control_ = nullptr;
	};

}