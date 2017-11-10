/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include <chrono>

#include "TestWinDebuggerKillOnIdleAndMaxExecutionTime.hpp"

using namespace std::chrono;

CTestWinDebuggerKillOnIdleAndMaxExecutionTime::CTestWinDebuggerKillOnIdleAndMaxExecutionTime() {
	command_line_		= "TestWinDebuggerKillOnIdleAndMaxExecutionTime.exe";
	fuzzed_file_		= "";
	log_dir_			= "TestWinDebuggerKillOnIdleAndMaxExecutionTimeLogs";
	debuggers_dir_		= "";
	symbols_path_		= "";
	debug_children_		= false;
	kill_on_idle_		= true;
	max_execution_time_ = 500;
	enable_page_heap_	= false;
	verbose_output_		= false;
}

CTestWinDebuggerKillOnIdleAndMaxExecutionTime::~CTestWinDebuggerKillOnIdleAndMaxExecutionTime() {
}

void CTestWinDebuggerKillOnIdleAndMaxExecutionTime::run() {
	AnabasisResult result;
	CAssert assert;

	CWinDebugger dbg(command_line_, fuzzed_file_);
	dbg.set_log_dir(log_dir_);
	dbg.set_debuggers_dir(debuggers_dir_);
	dbg.set_symbols_path(symbols_path_);
	dbg.set_debug_children(debug_children_);
	dbg.set_kill_on_idle(kill_on_idle_);
	dbg.set_max_execution_time(max_execution_time_);
	dbg.set_enable_page_heap(enable_page_heap_);
	dbg.set_verbose_output(verbose_output_);

	if ((result = dbg.initialize()) != AnabasisResult::OK) {
		throw assert;
	}

	uint64_t start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	if ((result = dbg.debug()) != AnabasisResult::OK) {
		remove_logs();
		throw assert;
	}
	uint64_t end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	if (end - start >= 10000) {
		throw assert;
	}

	if (is_logs_exist()) {
		throw assert;
	}
}