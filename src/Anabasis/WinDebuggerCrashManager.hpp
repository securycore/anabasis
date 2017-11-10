/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
#include <chrono>

#include "AnabasisTypes.hpp"
#include "WinDebuggerDebugClient.hpp"
#include "WinDebuggerDebugControl.hpp"
#include "WinDebuggerDebugSymbols.hpp"
#include "Util.hpp"

using namespace boost::system;
using namespace boost::filesystem;
using namespace std::chrono;

namespace Anabasis {

	class CWinDebuggerCrashManager {
	public:
		CWinDebuggerCrashManager();
		~CWinDebuggerCrashManager();

		AnabasisResult analyze_crash(
				std::string fuzzed_file,
				std::string log_dir,
				std::string& debugger_output,
				CWinDebuggerDebugControl& control);
		AnabasisResult check_paths(path fuzzed_file_path, path log_dir_path);
		AnabasisResult get_more_crash_details(CWinDebuggerDebugControl& control);
		std::string get_crash_id(std::string& debugger_output);
		AnabasisResult save_crash_files(path fuzzed_file_path, path log_dir_path, std::string crash_id, 
				std::string& debugger_output);

	private:
		bool use_fuzzed_file_ = false;
	};

}