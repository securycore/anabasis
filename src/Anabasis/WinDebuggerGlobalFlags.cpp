/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerGlobalFlags.hpp"

namespace Anabasis {

	CWinDebuggerGlobalFlags::CWinDebuggerGlobalFlags() {
	}

	CWinDebuggerGlobalFlags::~CWinDebuggerGlobalFlags() {
	}

	void CWinDebuggerGlobalFlags::set_command_line(std::string command_line) {
		image_name_ = CUtil::get_image_name_from_command_line(command_line);
	}

	void CWinDebuggerGlobalFlags::set_debuggers_dir(std::string debuggers_dir) {
		debuggers_dir_path_ = debuggers_dir;
		gflags_path_ = debuggers_dir_path_ / "gflags.exe";
	}

	AnabasisResult CWinDebuggerGlobalFlags::enable_page_heap(bool is_full_page_heap) {
		std::wstring gflags_args;

		if (!CUtil::is_user_an_admin()) {
			return AnabasisResult::WINDEBUGGER_GLOBAL_FLAGS_REQUIRE_ELEVATION;
		}

		if (is_full_page_heap) {
			gflags_args = std::wstring(L" /p /enable " + image_name_.native() + L" /full");
		} else {
			gflags_args = std::wstring(L" /p /enable " + image_name_.native());
		}

		HINSTANCE result = ShellExecuteW(nullptr, L"open", gflags_path_.c_str(), gflags_args.c_str(), nullptr, 0);
		if ((int)result <= 32) {
			return AnabasisResult::WINDEBUGGER_ENABLE_PAGE_HEAP_FAILED;
		}

		return AnabasisResult::OK;
	}

	AnabasisResult CWinDebuggerGlobalFlags::disable_page_heap() {
		std::wstring gflags_args = L" /p disable " + image_name_.native();

		if (!CUtil::is_user_an_admin()) {
			return AnabasisResult::WINDEBUGGER_GLOBAL_FLAGS_REQUIRE_ELEVATION;
		}

		HINSTANCE result = ShellExecuteW(nullptr, L"open", gflags_path_.c_str(), gflags_args.c_str(), nullptr, 0);
		if ((int)result <= 32) {
			return AnabasisResult::WINDEBUGGER_DISABLE_PAGE_HEAP_FAILED;
		}

		return AnabasisResult::OK;
	}

}