/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <dbgeng.h>
#include <iostream>

namespace Anabasis {

	// [Taken and modified from SDK sample: \sdk\samples\dumpstk]
	class CWinDebuggerOutputCallbacks : public IDebugOutputCallbacks {
	public:
		CWinDebuggerOutputCallbacks::CWinDebuggerOutputCallbacks(std::string& debugger_output);
		CWinDebuggerOutputCallbacks::~CWinDebuggerOutputCallbacks();
		void set_verbose_output(bool verbose_output);

		// IUnknown.
		STDMETHOD(QueryInterface)(THIS_ IN REFIID interface_id, OUT PVOID* interface_ptr);
		STDMETHOD_(ULONG, AddRef)(THIS);
		STDMETHOD_(ULONG, Release)(THIS);

		// IDebugOutputCallbacks.
		STDMETHOD(Output)(THIS_ IN ULONG mask, IN PCSTR text);

	private:
		std::string& debugger_output_;
		bool verbose_output_ = false;
	};

}