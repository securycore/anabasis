/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "TestWinDebugger.hpp"

bool CTestWinDebugger::is_logs_exist() {
	error_code ec;

	if (log_dir_ == "" || !exists(log_dir_, ec) || !is_directory(log_dir_, ec)) {
		return false;
	}

	return true;
}

void CTestWinDebugger::remove_logs() {
	if (is_logs_exist()) {
		remove_all(log_dir_);
	}
}