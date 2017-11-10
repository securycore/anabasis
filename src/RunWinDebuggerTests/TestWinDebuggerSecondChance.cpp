/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "TestWinDebuggerSecondChance.hpp"

CTestWinDebuggerSecondChance::CTestWinDebuggerSecondChance() {
	command_line_		= "TestWinDebuggerSecondChance.exe";
	fuzzed_file_		= "";
	log_dir_			= "TestWinDebuggerSecondChanceLogs";
	debuggers_dir_		= "";
	symbols_path_		= "";
	debug_children_		= false;
	kill_on_idle_		= false;
	max_execution_time_ = 0;
	enable_page_heap_	= false;
	verbose_output_		= false;
}

CTestWinDebuggerSecondChance::~CTestWinDebuggerSecondChance() {
}

void CTestWinDebuggerSecondChance::run() {
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

	if ((result = dbg.debug()) != AnabasisResult::OK) {
		remove_logs();
		throw assert;
	}

	if (!is_logs_exist()) {
		throw assert;
	}

	remove_logs();
}