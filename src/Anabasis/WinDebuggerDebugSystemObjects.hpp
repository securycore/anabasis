/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>

namespace Anabasis {

	class CWinDebuggerDebugSystemObjects {
	public:
		CWinDebuggerDebugSystemObjects();
		~CWinDebuggerDebugSystemObjects();

		IDebugSystemObjects* system_objects_ = nullptr;
	};

}