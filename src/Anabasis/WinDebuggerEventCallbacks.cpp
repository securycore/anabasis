/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerEventCallbacks.hpp"

namespace Anabasis {

	CWinDebuggerEventCallbacks::CWinDebuggerEventCallbacks(
		CWinDebuggerExecutionMonitor& execution_monitor,
		std::vector<ProcessHandle>& debuggee_processes, 
		bool& exception_is_continuable)
		: 
		execution_monitor_(execution_monitor),
		debuggee_processes_(debuggee_processes),
		exception_is_continuable_(exception_is_continuable) {
	}

	CWinDebuggerEventCallbacks::~CWinDebuggerEventCallbacks() {
	}

	STDMETHODIMP_(ULONG) CWinDebuggerEventCallbacks::AddRef(THIS) {
		return 1;
	}

	STDMETHODIMP_(ULONG) CWinDebuggerEventCallbacks::Release(THIS) {
		return 0;
	}

	STDMETHODIMP CWinDebuggerEventCallbacks::GetInterestMask(THIS_ OUT PULONG mask) {
		*mask = 
			DEBUG_EVENT_CREATE_PROCESS |
			DEBUG_EVENT_EXCEPTION;
		return S_OK;
	}

	STDMETHODIMP CWinDebuggerEventCallbacks::CreateProcess(
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
			IN ULONG64 start_offset) {

		UNREFERENCED_PARAMETER(image_file_handle);
		UNREFERENCED_PARAMETER(handle);
		UNREFERENCED_PARAMETER(module_size);
		UNREFERENCED_PARAMETER(module_name);
		UNREFERENCED_PARAMETER(checksum);
		UNREFERENCED_PARAMETER(time_date_stamp);
		UNREFERENCED_PARAMETER(initial_thread_handle);
		UNREFERENCED_PARAMETER(thread_data_offset);
		UNREFERENCED_PARAMETER(start_offset);

		debuggee_processes_.push_back(handle);

		// If the first process is created, launch a monitoring thread
		if (debuggee_processes_.size() == 1) {
			execution_monitor_.run(debuggee_processes_.at(0));
		}

		return DEBUG_STATUS_GO;
	}

	STDMETHODIMP CWinDebuggerEventCallbacks::Exception(THIS_ IN PEXCEPTION_RECORD64 exception, IN ULONG first_chance) {
		exception_is_continuable_ = true;

		if (first_chance) {
			if (exception->ExceptionCode == STATUS_BREAKPOINT ||
				exception->ExceptionCode == STATUS_SINGLE_STEP) {

				return DEBUG_STATUS_GO_HANDLED;
			}

			if (exception->ExceptionCode == STATUS_ACCESS_VIOLATION ||
				exception->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION ||
				exception->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {

				exception_is_continuable_ = false;
				return DEBUG_STATUS_BREAK;
			}
		} else {
			exception_is_continuable_ = false;
			return DEBUG_STATUS_BREAK;
		}

		return DEBUG_STATUS_GO_NOT_HANDLED;
	}

}