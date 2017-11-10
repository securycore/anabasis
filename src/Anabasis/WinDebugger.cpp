/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebugger.hpp"

namespace Anabasis {

	CWinDebugger::CWinDebugger(const char* command_line, const char* fuzzed_file)
		:
		output_callbacks_(debugger_output_),
		event_callbacks_(execution_monitor_, debuggee_processes_, exception_is_continuable_) {

		command_line_ = command_line;
		fuzzed_file_ = fuzzed_file;
		substitute_command_line_with_fuzzed_file();
	}

	CWinDebugger::CWinDebugger(uint32_t process_id, const char* fuzzed_file)
		:
		output_callbacks_(debugger_output_),
		event_callbacks_(execution_monitor_, debuggee_processes_, exception_is_continuable_) {

		process_id_to_attach_ = process_id;
		fuzzed_file_ = fuzzed_file;
	}

	CWinDebugger::~CWinDebugger() {
		if (enable_page_heap_) {
			global_flags_.disable_page_heap();
		}

		com_library_.deinitialize_com_library();

		deinitialize_engine();
	}

	void CWinDebugger::set_log_dir(const char* log_dir) {
		log_dir_ = log_dir;
		if (log_dir_ == "") {
			log_dir_ = "logs";
		}
	}

	void CWinDebugger::set_debuggers_dir(const char* debuggers_dir) {
		debuggers_dir_ = debuggers_dir;
		global_flags_.set_debuggers_dir(debuggers_dir_);
	}

	void CWinDebugger::set_symbols_path(const char* symbols_path) {
		symbols_path_ = symbols_path;
	}

	void CWinDebugger::set_debug_children(bool debug_children) {
		debug_children_ = debug_children;
	}

	void CWinDebugger::set_kill_on_idle(bool kill_on_idle) {
		kill_on_idle_ = kill_on_idle;
		execution_monitor_.set_kill_on_idle(kill_on_idle_);
	}

	void CWinDebugger::set_max_execution_time(uint64_t max_execution_time) {
		max_execution_time_ = max_execution_time;
		execution_monitor_.set_max_execution_time(max_execution_time_);
	}

	void CWinDebugger::set_enable_page_heap(bool enable_page_heap) {
		enable_page_heap_ = enable_page_heap;
	}

	void CWinDebugger::set_verbose_output(bool verbose_output) {
		verbose_output_ = verbose_output;
		output_callbacks_.set_verbose_output(verbose_output_);
	}

	AnabasisResult CWinDebugger::initialize() {
		AnabasisResult result;

		if ((result = initialize_engine()) != AnabasisResult::OK) {
			return result;
		}

		if ((result = com_library_.initialize_com_library()) != AnabasisResult::OK) {
			return result;
		}

		if (enable_page_heap_) {
			if ((result = global_flags_.enable_page_heap(true)) != AnabasisResult::OK) {
				return result;
			}
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::initialize_engine() {
		HRESULT status;

		if ((status = DebugCreate(__uuidof(IDebugClient), (void**)&client_.client_)) != S_OK) {
			return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
		}

		if ((status = client_.client_->QueryInterface(__uuidof(IDebugControl), (void**)&control_.control_)) != S_OK ||
			(status = client_.client_->QueryInterface(__uuidof(IDebugSymbols), (void**)&symbols_.symbols_)) != S_OK) {

			return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
		}

		if ((status = client_.client_->QueryInterface(__uuidof(IDebugSystemObjects),
			(void**)&system_objects_.system_objects_)) != S_OK) {

			return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
		}

		if ((status = client_.client_->SetOutputCallbacks(&output_callbacks_)) != S_OK) {
			return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
		}

		if ((status = client_.client_->SetEventCallbacks(&event_callbacks_)) != S_OK) {
			return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
		}

		if (symbols_path_ != "") {
			if ((status = symbols_.symbols_->SetSymbolPath(symbols_path_.c_str())) != S_OK) {
				return AnabasisResult::WINDEBUGGER_INITIALIZE_ENGINE_FAILED;
			}
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::deinitialize_engine() {
		if (system_objects_.system_objects_ != nullptr) {
			system_objects_.system_objects_->Release();
			system_objects_.system_objects_ = nullptr;
		}
		if (symbols_.symbols_ != nullptr) {
			symbols_.symbols_->Release();
			symbols_.symbols_ = nullptr;
		}
		if (control_.control_ != nullptr) {
			control_.control_->Release();
			control_.control_ = nullptr;
		}
		if (client_.client_ != nullptr) {
			client_.client_->SetOutputCallbacks(nullptr);
			client_.client_->EndSession(DEBUG_END_PASSIVE);
			client_.client_->Release();
			client_.client_ = nullptr;
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::debug() {
		AnabasisResult result;

		if (process_id_to_attach_ == 0) {
			if ((result = create_debuggee_process()) != AnabasisResult::OK) {
				return result;
			}
		} else {
			if ((result = attach_to_existing_process()) != AnabasisResult::OK) {
				return result;
			}
		}

		result = enter_debug_loop();
		return result;
	}

	AnabasisResult CWinDebugger::create_debuggee_process() {
		HRESULT status;

		ULONG flags = DEBUG_PROCESS;
		if (!debug_children_) {
			flags |= DEBUG_ONLY_THIS_PROCESS;
		}

		if ((status = client_.client_->CreateProcessAndAttach(0, (char*)command_line_.c_str(), flags, 0, 0)) != S_OK) {
			return AnabasisResult::WINDEBUGGER_CREATE_PROCESS_AND_ATTACH_FAILED;
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::attach_to_existing_process() {
		HRESULT status;

		ULONG flags = DEBUG_ATTACH_DEFAULT;

		if ((status = client_.client_->AttachProcess(0, process_id_to_attach_, flags)) != S_OK) {
			return AnabasisResult::WINDEBUGGER_ATTACH_PROCESS_FAILED;
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::enter_debug_loop() {
		AnabasisResult result = AnabasisResult::OK;
		HRESULT status;

		while (1) {
			status = control_.control_->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);

			ULONG exec_status = 0;
			control_.control_->GetExecutionStatus(&exec_status);

			if (status != S_OK) {
				// Check and see whether the session is running or not
				if (exec_status == DEBUG_STATUS_NO_DEBUGGEE) {
					// The session ended so we can quit
					result = AnabasisResult::OK;
					break;
				}

				// Unexpected engine error
				result = AnabasisResult::WINDEBUGGER_ENGINE_ERROR;
				break;
			}

			if (!exception_is_continuable_) {
				std::cout << "[Anabasis] WOW! A crash has been catched" << std::endl;

				result = crash_manager_.analyze_crash(fuzzed_file_, log_dir_, debugger_output_, control_);

				if (result == AnabasisResult::OK) {
					std::cout << "[Anabasis] The crash has been analyzed successfully" << std::endl;
				} else {
					std::cout << "[Anabasis] Error: failed to analyze the crash" << std::endl;
				}
				break;
			}
		}

		return result;
	}

	AnabasisResult CWinDebugger::is_debuggee_process_active(bool& is_active) {
		FILETIME creation_time, exit_time, kernel_time, user_time;
		if (!GetProcessTimes((HANDLE)debuggee_processes_.at(0), &creation_time, &exit_time, &kernel_time, &user_time)) {
			return AnabasisResult::WINDEBUGGER_IS_DEBUGGEE_PROCESS_ACTIVE_CHECK_FAILED;
		}

		is_active = true;

		if (kernel_time.dwLowDateTime == last_debuggee_kernel_time_.dwLowDateTime &&
			kernel_time.dwHighDateTime == last_debuggee_kernel_time_.dwHighDateTime &&
			user_time.dwLowDateTime == last_debuggee_user_time_.dwLowDateTime &&
			user_time.dwHighDateTime == last_debuggee_user_time_.dwHighDateTime) {

			is_active = false;
		}

		last_debuggee_kernel_time_ = kernel_time;
		last_debuggee_user_time_ = user_time;

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebugger::terminate_debuggee_and_children_processes() {
		HRESULT status;

		if ((status = client_.client_->TerminateProcesses()) != S_OK) {
			return AnabasisResult::WINDEBUGGER_TERMINATE_DEBUGGEE_AND_CHILDREN_PROCESSES_FAILED;
		}

		return AnabasisResult::OK;
	}

	inline void CWinDebugger::substitute_command_line_with_fuzzed_file() {
		boost::replace_first(command_line_, "@@", "\"" + fuzzed_file_ + "\"");
	}

}