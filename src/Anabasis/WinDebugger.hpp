/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <windows.h>
#include <dbgeng.h>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "AnabasisTypes.hpp"
#include "WinDebuggerTypes.hpp"
#include "WinDebuggerDebugClient.hpp"
#include "WinDebuggerDebugControl.hpp"
#include "WinDebuggerDebugSymbols.hpp"
#include "WinDebuggerDebugSystemObjects.hpp"
#include "WinDebuggerOutputCallbacks.hpp"
#include "WinDebuggerEventCallbacks.hpp"
#include "WinDebuggerExecutionMonitor.hpp"
#include "WinDebuggerCrashManager.hpp"
#include "WinDebuggerCOM.hpp"
#include "WinDebuggerGlobalFlags.hpp"

namespace Anabasis {

	// A debugger based on Windows Debugger Engine (DbgEng.dll)
	class CWinDebugger {
	public:

		// Debug new process
		// @param command_line	Path to an executable with arguments. "@@" characters will be replaced
		//						with fuzzed_file. Example: "notepad.exe @@"
		// @param fuzzed_file	Path to a fuzzed file that should be opened
		//						You can specify path to any file and it will then be copied into 
		//						a log directory when crash occurred
		//						Pass "" to ignore
		ANABASIS_API CWinDebugger(
			const char* command_line,
			const char* fuzzed_file
		);

		// Debug existing process
		// @param process_id	PID to attach the debugger
		// @param fuzzed_file	Path to a fuzzed file
		//						You can specify path to any file and it will then be copied into 
		//						a log directory when crash occurred
		//						Pass "" to ignore
		ANABASIS_API CWinDebugger(
			uint32_t process_id,
			const char* fuzzed_file
		);

		ANABASIS_API ~CWinDebugger();

		// Set path to a log directory where crash dumps and logs will be stored
		// An empty string will be converted to "logs"
		// Default value: "logs"
		ANABASIS_API void set_log_dir(const char* log_dir);

		// Set path to a DDK or Debugging Tools for Windows directory where gflags.exe is located
		// Use this method together with set_enable_page_heap()
		// Default value: ""
		ANABASIS_API void set_debuggers_dir(const char* debuggers_dir);

		// Set path to a symbols
		// Default value: ""
		ANABASIS_API void set_symbols_path(const char* set_symbols_path);

		// Specify whether a debuggee's children should be debugged
		// Default value: false
		ANABASIS_API void set_debug_children(bool debug_children);

		// Kill a debuggee and it's children when a debuggee's CPU usage is zero
		// Default value: false
		ANABASIS_API void set_kill_on_idle(bool kill_on_idle);

		// Kill a debuggee and it's children when a debuggee's execution time is greater than specified, 
		// in milliseconds.
		// Default value: 0 (disabled)
		ANABASIS_API void set_max_execution_time(uint64_t max_execution_time);

		// Use PageHeap feature
		// You must also call set_debuggers_dir() if you want to enable PageHeap
		// Default value: false
		ANABASIS_API void set_enable_page_heap(bool enable_page_heap);

		// Redirect a debuggee's output to a console
		// Default value: false
		ANABASIS_API void set_verbose_output(bool verbose_output);

		// Initialize Windows Debugger Engine
		// The method should be called when set_* methods are invoked
		ANABASIS_API AnabasisResult initialize();

		// Start debugging
		// The method should be called after initialize()
		ANABASIS_API AnabasisResult debug();

	private:
		AnabasisResult initialize_engine();
		AnabasisResult create_debuggee_process();
		AnabasisResult attach_to_existing_process();
		AnabasisResult enter_debug_loop();
		AnabasisResult is_debuggee_process_active(bool& is_active);
		AnabasisResult terminate_debuggee_and_children_processes();
		AnabasisResult deinitialize_engine();

		inline void substitute_command_line_with_fuzzed_file();

	private:
		std::string command_line_		= "";
		std::string fuzzed_file_		= "";
		std::string log_dir_			= "logs";
		std::string debuggers_dir_		= "";
		std::string symbols_path_		= "";
		bool debug_children_			= false;
		bool kill_on_idle_				= false;
		uint64_t max_execution_time_	= 0;
		bool enable_page_heap_			= false;
		bool verbose_output_			= false;

		uint32_t process_id_to_attach_	= 0;

		CWinDebuggerDebugClient client_;
		CWinDebuggerDebugControl control_;
		CWinDebuggerDebugSymbols symbols_;
		CWinDebuggerDebugSystemObjects system_objects_;
		CWinDebuggerOutputCallbacks output_callbacks_;
		CWinDebuggerEventCallbacks event_callbacks_;

		// Used by CWinDebuggerEventCallbacks to launch a monitoring thread when a debuggee process is created
		CWinDebuggerExecutionMonitor execution_monitor_;

		CWinDebuggerCrashManager crash_manager_;
		CWinDebuggerCOM com_library_;
		CWinDebuggerGlobalFlags global_flags_;

		// Used by CWinDebuggerOutputCallbacks to store the debugger output
		std::string debugger_output_		= "";

		// Used by CWinDebuggerEventCallbacks to signal whether an exception is continuable
		bool exception_is_continuable_		= false;

		// Used by CWinDebuggerEventCallbacks to store handles of a debuggee process and it's subprocesses.
		// The first vector entry is a debuggee process handle and the following entries are subprocesses handles
		std::vector<ProcessHandle> debuggee_processes_;

		FILETIME last_debuggee_kernel_time_ = {0};
		FILETIME last_debuggee_user_time_	= {0};
	};

}