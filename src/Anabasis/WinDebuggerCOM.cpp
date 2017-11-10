/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerCOM.hpp"

namespace Anabasis {

	CWinDebuggerCOM::CWinDebuggerCOM() {
		is_initialized_ = false;
	}

	CWinDebuggerCOM::~CWinDebuggerCOM() {
	}

	AnabasisResult CWinDebuggerCOM::initialize_com_library() {
		if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) != S_OK) {
			return AnabasisResult::WINDEBUGGER_INITIALIZE_COM_LIBRARY_FAILED;
		}

		is_initialized_ = true;
		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebuggerCOM::deinitialize_com_library() {
		if (is_initialized_) {
			CoUninitialize();
			is_initialized_ = false;
		}

		return AnabasisResult::OK;
	}

}