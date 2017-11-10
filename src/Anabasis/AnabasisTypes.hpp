/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

// WARNING: only built-in types like char*, wchar_t*, int and so on must be used as arguments of exported methods
// since passing objects like std::string might cause memory corruption
#ifdef ANABASIS_EXPORT
#	define ANABASIS_API __declspec(dllexport)
#else
#	define ANABASIS_API __declspec(dllimport)
#endif

namespace Anabasis {

	typedef enum {
		OK = 0,
		WINDEBUGGER_INITIALIZE_ENGINE_FAILED,
		WINDEBUGGER_INITIALIZE_COM_LIBRARY_FAILED,
		WINDEBUGGER_GLOBAL_FLAGS_REQUIRE_ELEVATION,
		WINDEBUGGER_ENABLE_PAGE_HEAP_FAILED,
		WINDEBUGGER_DISABLE_PAGE_HEAP_FAILED,
		WINDEBUGGER_CREATE_PROCESS_AND_ATTACH_FAILED,
		WINDEBUGGER_ATTACH_PROCESS_FAILED,
		WINDEBUGGER_IS_DEBUGGEE_PROCESS_ACTIVE_CHECK_FAILED,
		WINDEBUGGER_TERMINATE_DEBUGGEE_AND_CHILDREN_PROCESSES_FAILED,
		WINDEBUGGER_ENGINE_ERROR,
		WINDEBUGGER_CRASH_ANALYSIS_FAILED,
	} AnabasisResult;

}
