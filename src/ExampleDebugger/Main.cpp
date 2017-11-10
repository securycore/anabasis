/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include <cstdint>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include "Anabasis.hpp"

namespace po = boost::program_options;
using namespace Anabasis;

std::string command_line	= "";
uint32_t pid				= 0;
std::string fuzzed_file		= "";
std::string log_dir			= "";
std::string debuggers_dir	= "";
std::string symbols_path	= "";
bool debug_children			= false;
bool kill_on_idle			= false;
uint64_t max_execution_time	= 0;
bool enable_page_heap		= false;
bool verbose_output			= false;

int parse_command_line(int argc, char* argv[]) {
	po::options_description desc("Usage");
	desc.add_options()
		("command-line,c", 
			po::value< std::vector< std::string > >(),
			"(MANDATORY without --pid) Path to an executable with arguments. \"@@\" characters will be replaced with --fuzzed-file")
		("pid,p",
			po::value<uint32_t>(),
			"(MANDATORY without --command-line) Process ID to attach")
		("fuzzed-file,f", 
			po::value< std::vector< std::string > >(), 
			"Path to a fuzzed file that should be opened")
		("log-dir,l", 
			po::value< std::vector< std::string > >(), 
			"Path to a log directory where crash dumps and logs will be stored");
	desc.add_options()
		("debuggers-dir,d",
			po::value< std::vector< std::string > >(),
			"(MANDATORY when --enable-page-heap is used) Path to a DDK or Debugging Tools for Windows directory where gflags.exe is located")
		("symbols-path,s", 
			po::value< std::vector< std::string > >(), 
			"Path to a symbols")
		("debug-children,n", 
			"Also debug each debuggee's subprocesses")
		("kill-on-idle,k", 
			"Kill a debuggee and it's children when the debuggee's CPU usage is zero");
	desc.add_options()
		("max-execution-time,t",
			po::value<uint64_t>(),
			"Kill a debuggee and it's children when the debuggee's execution time is greater than specified, in milliseconds")
		("enable-page-heap,h", 
			"Use PageHeap feature")
		("verbose-output,v", 
			"Send output of the debugger to console");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (argc == 1) {
		std::cout << desc << std::endl;
		return 0;
	}

	std::vector<std::string> string_argument;

	if (!vm.count("command-line") && !vm.count("pid")) {
		std::cout << desc << std::endl;
		return 0;
	}

	if (vm.count("command-line") && vm.count("pid")) {
		std::cout << desc << std::endl;
		return 0;
	}

	// --command-line is a mandatory argument when --pid is not used
	if (vm.count("command-line")) {
		string_argument = vm["command-line"].as<std::vector<std::string>>();
		command_line = string_argument.at(0);
	}

	if (vm.count("pid")) {
		pid = vm["pid"].as<uint32_t>();
	}

	if (vm.count("fuzzed-file")) {
		string_argument = vm["fuzzed-file"].as<std::vector<std::string>>();
		fuzzed_file = string_argument.at(0);
	}

	if (vm.count("log-dir")) {
		string_argument = vm["log-dir"].as<std::vector<std::string>>();
		log_dir = string_argument.at(0);
	}

	if (vm.count("debuggers-dir")) {
		string_argument = vm["debuggers-dir"].as<std::vector<std::string>>();
		debuggers_dir = string_argument.at(0);
	}

	if (vm.count("symbols-path")) {
		string_argument = vm["symbols-path"].as<std::vector<std::string>>();
		symbols_path = string_argument.at(0);
	}

	if (vm.count("debug-children")) {
		debug_children = true;
	}

	if (vm.count("kill-on-idle")) {
		kill_on_idle = true;
	}

	if (vm.count("max-execution-time")) {
		max_execution_time = vm["max-execution-time"].as<uint64_t>();
	}

	if (vm.count("enable-page-heap")) {
		if (!vm.count("debuggers-dir")) {
			std::cout << desc << std::endl;
			return 0;
		}
		enable_page_heap = true;
	}

	if (vm.count("verbose-output")) {
		verbose_output = true;
	}

	return 1;
}

CWinDebugger* create_debugger() {
	if (pid == 0) {
		return new CWinDebugger(command_line.c_str(), fuzzed_file.c_str());
	} else {
		return new CWinDebugger(pid, fuzzed_file.c_str());
	}
}

int main(int argc, char* argv[]) {
	AnabasisResult result;
	
	if (!parse_command_line(argc, argv)) {
		return 0;
	}

	std::cout << "[*] Parameters:"			<< std::endl;
	std::cout << "command-line:\t\t\""		<< command_line			<< "\"" << std::endl; 
	std::cout << "fuzzed-file:\t\t\""		<< fuzzed_file			<< "\"" << std::endl;
	std::cout << "log-dir:\t\t\""			<< log_dir				<< "\"" << std::endl;
	std::cout << "debuggers-dir:\t\t\""		<< debuggers_dir		<< "\"" << std::endl;
	std::cout << "symbols-path:\t\t\""		<< symbols_path			<< "\"" << std::endl;
	std::cout << "debug-children:\t\t"		<< debug_children		<< std::endl;
	std::cout << "kill-on-idle:\t\t"		<< kill_on_idle			<< std::endl;
	std::cout << "max-execution-time:\t"	<< max_execution_time	<< std::endl;
	std::cout << "enable-page-heap:\t"		<< enable_page_heap		<< std::endl;
	std::cout << "verbose-output:\t\t"		<< verbose_output		<< std::endl;

	CWinDebugger* dbg = create_debugger();
	if (!dbg) {
		std::cout << "[-] Failed to allocate memory for the debugger" << std::endl;
		return -1;
	}

	dbg->set_log_dir(log_dir.c_str());
	dbg->set_debuggers_dir(debuggers_dir.c_str());
	dbg->set_symbols_path(symbols_path.c_str());
	dbg->set_debug_children(debug_children);
	dbg->set_kill_on_idle(kill_on_idle);
	dbg->set_max_execution_time(max_execution_time);
	dbg->set_enable_page_heap(enable_page_heap);
	dbg->set_verbose_output(verbose_output);

	if ((result = dbg->initialize()) != AnabasisResult::OK) {
		std::cout << "[-] CWinDebugger::initialize() failed: " << result << std::endl;
		delete dbg;
		return -1;
	}

	if ((result = dbg->debug()) != AnabasisResult::OK) {
		std::cout << "[-] CWinDebugger::debug() failed: " << result << std::endl;
		delete dbg;
		return -1;
	}

	std::cout << "[+] Exiting" << std::endl;
	delete dbg;
	return 0;
}