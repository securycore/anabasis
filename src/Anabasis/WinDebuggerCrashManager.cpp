/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerCrashManager.hpp"

namespace Anabasis {

	CWinDebuggerCrashManager::CWinDebuggerCrashManager() {
		use_fuzzed_file_ = true;
	}

	CWinDebuggerCrashManager::~CWinDebuggerCrashManager() {
	}

	AnabasisResult CWinDebuggerCrashManager::analyze_crash(
			std::string fuzzed_file,
			std::string log_dir,
			std::string& debugger_output,
			CWinDebuggerDebugControl& control) {
		AnabasisResult result;

		path fuzzed_file_path = fuzzed_file;
		path log_dir_path = log_dir;
		if (fuzzed_file_path == "") {
			use_fuzzed_file_ = false;
		}

		if ((result = check_paths(fuzzed_file_path, log_dir_path)) != AnabasisResult::OK) {
			return result;
		}

		// Execute debugger commands to obtain more information about a crash
		get_more_crash_details(control);

		// Extract a unique crash identifier from !exploitable output
		std::string crash_id = get_crash_id(debugger_output);
		if (crash_id == "") {
			return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
		}

		// Save all the crash related files
		result = save_crash_files(fuzzed_file_path, log_dir_path, crash_id, debugger_output);

		return result;
	}

	AnabasisResult CWinDebuggerCrashManager::check_paths(path fuzzed_file_path, path log_dir_path) {
		error_code ec;

		if (use_fuzzed_file_) {
			// Ensure the fuzzed file exists
			if (!exists(fuzzed_file_path, ec) || !is_regular_file(fuzzed_file_path, ec)) {
				CUtil::print("[Anabasis] Error: fuzzed file '%s' not found or not a regular file\n",
					fuzzed_file_path.string().c_str());
				return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
			}
		}

		// Create log directory if doesn't exist
		if (!exists(log_dir_path, ec)) {
			if (!create_directory(log_dir_path, ec)) {
				CUtil::print("[Anabasis] Error: cannot create log directory '%s'\n", log_dir_path.string().c_str());
				return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
			}
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebuggerCrashManager::get_more_crash_details(CWinDebuggerDebugControl& control) {
		// Print registers and the call stack
		control.control_->Execute(DEBUG_OUTCTL_THIS_CLIENT, "r; .echo", DEBUG_EXECUTE_ECHO);
		control.control_->Execute(DEBUG_OUTCTL_THIS_CLIENT, "kb; .echo", DEBUG_EXECUTE_ECHO);

		// Load !exploitable extension
		path msec_extension_path = current_path() / "MSEC.dll";
		std::string command = ".load " + msec_extension_path.string() + "; .echo";
		control.control_->Execute(DEBUG_OUTCTL_THIS_CLIENT, command.c_str(), DEBUG_EXECUTE_ECHO);

		// Use !exploitable extension
		control.control_->Execute(DEBUG_OUTCTL_THIS_CLIENT, "!exploitable -m; .echo", DEBUG_EXECUTE_ECHO);

		return AnabasisResult::OK;
	}

	std::string CWinDebuggerCrashManager::get_crash_id(std::string& debugger_output) {
		// Use non-greedy regex
		boost::regex re("MAJOR_HASH:(.*?)\nMINOR_HASH:(.*?)\n.*CLASSIFICATION:(.*?)\n", boost::regex_constants::perl);
		boost::smatch matches;
		if (boost::regex_search(debugger_output, matches, re)) {
			std::string major_hash(matches[1].first, matches[1].second);
			std::string minor_hash(matches[2].first, matches[2].second);
			std::string classification(matches[3].first, matches[3].second);
			std::string crash_id = classification + "_" + major_hash + "_" + minor_hash;
			return crash_id;
		}

		CUtil::print("[Anabasis] Error: failed to obtain crash ID. This is probably because MSEC.dll was not found\n");
		return "";
	}

	AnabasisResult CWinDebuggerCrashManager::save_crash_files(path fuzzed_file_path, path log_dir_path,
			std::string crash_id, std::string& debugger_output) {

		error_code ec;
		path crash_id_dir_path = log_dir_path / crash_id;

		// Check wether a crash ID directory exists
		if (!exists(crash_id_dir_path, ec)) {
			// Doesn't exist, create it
			if (!create_directory(crash_id_dir_path, ec)) {
				CUtil::print("[Anabasis] Error: cannot create crash ID directory '%s'\n", crash_id_dir_path.string().c_str());
				return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
			}
		}
		else if (!is_directory(crash_id_dir_path, ec)) {
			// The path already exists but it's not a directory
			CUtil::print("[Anabasis] Error: crash ID directory path '%s' already in use\n", crash_id_dir_path.string().c_str());
			return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
		}

		// Create unique directory for current crash
		system_clock::duration duration_time = system_clock::now().time_since_epoch();
		long long time = duration_cast<milliseconds>(duration_time).count();

		path crash_dir_path = crash_id_dir_path / std::to_string(time);

		if (exists(crash_dir_path, ec)) {
			CUtil::print("[Anabasis] Error: crash directory path '%s' already in use (very strange!)\n",
				crash_dir_path.string().c_str());
			return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
		}
		if (!create_directory(crash_dir_path, ec)) {
			CUtil::print("[Anabasis] Error: cannot create crash directory '%s' (very strange!)\n",
				crash_dir_path.string().c_str());
			return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
		}

		// Save debugger output into a file
		path debugger_log_file_path = crash_dir_path / "debugger.log";

		boost::filesystem::ofstream debugger_log(debugger_log_file_path);
		debugger_log.write(debugger_output.c_str(), debugger_output.size());

		if (use_fuzzed_file_) {
			// Copy the fuzzed file into a crash directory
			path filename = fuzzed_file_path.filename();
			path dst_fuzzed_file_path = crash_dir_path / filename;

			copy_file(fuzzed_file_path, dst_fuzzed_file_path, copy_option::fail_if_exists, ec);
			if (ec) {
				CUtil::print("[Anabasis] Error: cannot copy the fuzzed file '%s' into the crash directory '%s'",
					fuzzed_file_path.string().c_str(), 
					crash_dir_path.string().c_str());
				return AnabasisResult::WINDEBUGGER_CRASH_ANALYSIS_FAILED;
			}
		}

		return AnabasisResult::OK;
	}

}