/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <windows.h>

#include "AnabasisTypes.hpp"

namespace Anabasis {

	// COM library should be initialized before ShellExecute usage
	class CWinDebuggerCOM {
	public:
		CWinDebuggerCOM();
		~CWinDebuggerCOM();

		AnabasisResult initialize_com_library();
		AnabasisResult deinitialize_com_library();

	private:
		bool is_initialized_ = false;
	};

}