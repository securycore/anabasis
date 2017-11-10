/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>
#include <iostream>
#include <vector>

#include "WinDebuggerTypes.hpp"
#include "WinDebuggerExecutionMonitor.hpp"

namespace Anabasis {

	class CWinDebuggerEventCallbacks : public DebugBaseEventCallbacks {
	public:
		CWinDebuggerEventCallbacks::CWinDebuggerEventCallbacks(
			CWinDebuggerExecutionMonitor& execution_monitor,
			std::vector<ProcessHandle>& debuggee_processes,
			bool& exception_is_continuable);
		CWinDebuggerEventCallbacks::~CWinDebuggerEventCallbacks();

		// IUnknown.
		STDMETHOD_(ULONG, AddRef)(THIS);
		STDMETHOD_(ULONG, Release)(THIS);

		// IDebugEventCallbacks.
		STDMETHOD(GetInterestMask)(THIS_ OUT PULONG Mask);
		STDMETHOD(CreateProcess)(
			THIS_
			IN ULONG64 image_file_handle,
			IN ULONG64 handle,
			IN ULONG64 base_offset,
			IN ULONG module_size,
			IN PCSTR module_name,
			IN PCSTR image_name,
			IN ULONG checksum,
			IN ULONG time_date_stamp,
			IN ULONG64 initial_thread_handle,
			IN ULONG64 thread_data_offset,
			IN ULONG64 start_offset
			);
		STDMETHOD(Exception)(THIS_ IN PEXCEPTION_RECORD64 exception, IN ULONG first_chance);

	private:
		CWinDebuggerExecutionMonitor& execution_monitor_;
		std::vector<ProcessHandle>& debuggee_processes_;
		bool& exception_is_continuable_;
	};

}