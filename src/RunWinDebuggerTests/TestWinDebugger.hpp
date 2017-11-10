/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <boost/filesystem.hpp>

#include "Test.hpp"
#include "Anabasis.hpp"

using namespace boost::filesystem;

class CTestWinDebugger : public CTest {
public:
	virtual void run() = 0;

	bool is_logs_exist();
	void remove_logs();

public:
	char* command_line_;
	char* fuzzed_file_;
	char* log_dir_;
	char* debuggers_dir_;
	char* symbols_path_;
	bool debug_children_;
	bool kill_on_idle_;
	uint64_t max_execution_time_;
	bool enable_page_heap_;
	bool verbose_output_;
};
