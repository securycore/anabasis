/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "TestWinDebuggerAccessViolation.hpp"
#include "TestWinDebuggerBreakpoint.hpp"
#include "TestWinDebuggerIllegalInstruction.hpp"
#include "TestWinDebuggerGuardPage.hpp"
#include "TestWinDebuggerSecondChance.hpp"
#include "TestWinDebuggerKillOnIdle.hpp"
#include "TestWinDebuggerMaxExecutionTime.hpp"
#include "TestWinDebuggerKillOnIdleAndMaxExecutionTime.hpp"
#include "TestWinDebuggerAttachProcess.hpp"

int main() {
	int tests_num = 9;
	int tests_passed = 0;
	int i = 0;

	std::cout << "[*] Running tests..." << std::endl << std::endl;

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerAccessViolation" << std::endl;
		CTestWinDebuggerAccessViolation().run();
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
		tests_passed++;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerBreakpoint" << std::endl;
		CTestWinDebuggerBreakpoint().run();
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
		tests_passed++;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerIllegalInstruction" << std::endl;
		CTestWinDebuggerIllegalInstruction().run();
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
		tests_passed++;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerGuardPage" << std::endl;
		CTestWinDebuggerGuardPage().run();
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
		tests_passed++;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerSecondChance" << std::endl;
		CTestWinDebuggerSecondChance().run();
		tests_passed++;
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerKillOnIdle" << std::endl;
		CTestWinDebuggerKillOnIdle().run();
		tests_passed++;
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerMaxExecutionTime" << std::endl;
		CTestWinDebuggerMaxExecutionTime().run();
		tests_passed++;
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerKillOnIdleAndMaxExecutionTime" << std::endl;
		CTestWinDebuggerKillOnIdleAndMaxExecutionTime().run();
		tests_passed++;
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
	} catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	try {
		std::cout << "[" << ++i << "] CTestWinDebuggerAttachProcess" << std::endl;
		CTestWinDebuggerAttachProcess().run();
		tests_passed++;
		std::cout << "[" << i << "] OK" << std::endl << std::endl;
	}
	catch (CAssert) {
		std::cout << "[" << i << "] FAILED" << std::endl << std::endl;
	}

	if (tests_passed == tests_num) {
		std::cout << "[+] All tests are passed" << std::endl;
	} else {
		std::cout << "[-] One or more tests are failed" << std::endl;
	}

	return 0;
}