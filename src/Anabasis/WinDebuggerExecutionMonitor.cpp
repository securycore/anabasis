/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerExecutionMonitor.hpp"

namespace Anabasis {

	CWinDebuggerExecutionMonitor::CWinDebuggerExecutionMonitor() {
		debuggee_ = 0;
		kill_on_idle_ = false;
		max_execution_time_ = 0;

		execution_monitor_is_enabled_ = false;
		worker_is_started_ = false;

		last_debuggee_kernel_time_ = {0};
		last_debuggee_user_time_ = {0};
	}

	CWinDebuggerExecutionMonitor::~CWinDebuggerExecutionMonitor() {
		if (execution_monitor_is_enabled_ && worker_is_started_) {
			worker_.join();
		}
	}

	void CWinDebuggerExecutionMonitor::set_kill_on_idle(bool kill_on_idle) {
		kill_on_idle_ = kill_on_idle;
		if (kill_on_idle_) {
			execution_monitor_is_enabled_ = true;
		}
	}

	void CWinDebuggerExecutionMonitor::set_max_execution_time(uint64_t max_execution_time) {
		max_execution_time_ = max_execution_time;
		if (max_execution_time_ != 0) {
			execution_monitor_is_enabled_ = true;
		}
	}

	void CWinDebuggerExecutionMonitor::run(ProcessHandle debuggee_process) {
		if (execution_monitor_is_enabled_) {
			debuggee_ = debuggee_process;
			worker_is_started_ = true;

			worker_ = std::thread(&CWinDebuggerExecutionMonitor::worker_main, this);
		}
	}

	void CWinDebuggerExecutionMonitor::worker_main() {
		int poll_time = 500;

		uint64_t start_execution_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		do {
			Sleep(poll_time);

			if (kill_on_idle_) {
				if (!check_debuggee_is_active()) {
					terminate_debuggee_and_children();
					break;
				}
			}

			if (max_execution_time_ != 0) {
				if (check_debuggee_execution_time_is_over(start_execution_time)) {
					terminate_debuggee_and_children();
					break;
				}
			}
		} while (1);
	}

	bool CWinDebuggerExecutionMonitor::check_debuggee_is_active() {
		FILETIME creation_time = {0};
		FILETIME exit_time = {0};
		FILETIME kernel_time = {0};
		FILETIME user_time = {0};
		
		if (!GetProcessTimes((HANDLE)debuggee_, &creation_time, &exit_time, &kernel_time, &user_time)) {
			return false;
		}

		if (kernel_time.dwLowDateTime == last_debuggee_kernel_time_.dwLowDateTime &&
			kernel_time.dwHighDateTime == last_debuggee_kernel_time_.dwHighDateTime &&
			user_time.dwLowDateTime == last_debuggee_user_time_.dwLowDateTime &&
			user_time.dwHighDateTime == last_debuggee_user_time_.dwHighDateTime) {

			return false;
		}

		last_debuggee_kernel_time_ = kernel_time;
		last_debuggee_user_time_ = user_time;

		return true;
	}

	bool CWinDebuggerExecutionMonitor::check_debuggee_execution_time_is_over(uint64_t start_execution_time) {
		uint64_t execution_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		if (execution_time >= start_execution_time + max_execution_time_) {
			return true;
		}

		return false;
	}

	void CWinDebuggerExecutionMonitor::terminate_debuggee_and_children() {
		BOOL result = TerminateProcess((HANDLE)debuggee_, 0);
	}
}