#pragma once

#include <windows.h>
#include <thread>
#include <chrono>

#include "AnabasisTypes.hpp"
#include "WinDebuggerTypes.hpp"

using namespace std::chrono;

namespace Anabasis {

	class CWinDebuggerExecutionMonitor {
	public:
		CWinDebuggerExecutionMonitor();
		~CWinDebuggerExecutionMonitor();
		void set_kill_on_idle(bool kill_on_idle);
		void set_max_execution_time(uint64_t max_execution_time);

		void run(ProcessHandle debuggee);
		void worker_main();
		bool check_debuggee_is_active();
		bool check_debuggee_execution_time_is_over(uint64_t start_execution_time);
		void terminate_debuggee_and_children();

	private:
		bool execution_monitor_is_enabled_	= false;

		ProcessHandle debuggee_				= 0;
		bool kill_on_idle_					= false;
		uint64_t max_execution_time_		= 0;

		std::thread worker_;
		bool worker_is_started_				= false;

		FILETIME last_debuggee_kernel_time_ = {0};
		FILETIME last_debuggee_user_time_	= {0};
	};

}