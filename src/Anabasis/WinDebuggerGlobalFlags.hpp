/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <windows.h>
#include <boost/filesystem.hpp>

#include "AnabasisTypes.hpp"
#include "Util.hpp"

using namespace boost::filesystem;

namespace Anabasis {

	class CWinDebuggerGlobalFlags {
	public:
		CWinDebuggerGlobalFlags();
		~CWinDebuggerGlobalFlags();
		void set_command_line(std::string command_line);
		void set_debuggers_dir(std::string debuggers_dir);

		AnabasisResult enable_page_heap(bool is_full_page_heap);
		AnabasisResult disable_page_heap();

	private:
		path image_name_			= "";
		path debuggers_dir_path_	= "";
		path gflags_path_			= "";
	};

}