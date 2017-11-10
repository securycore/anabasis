/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "Anabasis.hpp"

using namespace Anabasis;

int main(int argc, char* argv[]) {
	AnabasisResult result;

	std::wcout << L"[#] This is an example of a simple debugger based on Anabasis library" << std::endl;

	// "@@" will be replaced with "my-notes.txt"
	char* command_line = "notepad.exe @@";
	char* fuzzed_file = "my-notes.txt";
	CWinDebugger dbg(command_line, fuzzed_file);

	// These calls are optional, you can leave a default configuration
	dbg.set_log_dir("");
	dbg.set_debuggers_dir("");
	dbg.set_symbols_path("");
	dbg.set_debug_children(false);
	dbg.set_kill_on_idle(false);
	dbg.set_max_execution_time(0);
	dbg.set_enable_page_heap(false);
	dbg.set_verbose_output(true);

	if ((result = dbg.initialize()) != AnabasisResult::OK) {
		std::cout << "[-] CWinDebugger::initialize() failed: " << result << std::endl;
		return -1;
	}

	if ((result = dbg.debug()) != AnabasisResult::OK) {
		std::cout << "[-] CWinDebugger::debug() failed: " << result << std::endl;
		return -1;
	}

	std::cout << "[+] Exiting" << std::endl;

	return 0;
}
