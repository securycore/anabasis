/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "WinDebuggerOutputCallbacks.hpp"

namespace Anabasis {

	CWinDebuggerOutputCallbacks::CWinDebuggerOutputCallbacks(std::string& debugger_output)
			: debugger_output_(debugger_output) {

		verbose_output_ = false;
	}

	CWinDebuggerOutputCallbacks::~CWinDebuggerOutputCallbacks() {
	}

	void CWinDebuggerOutputCallbacks::set_verbose_output(bool verbose_output) {
		verbose_output_ = verbose_output;
	}

	STDMETHODIMP CWinDebuggerOutputCallbacks::QueryInterface(THIS_ IN REFIID interface_id, OUT PVOID* interface_ptr) {
		*interface_ptr = NULL;

		if (IsEqualIID(interface_id, __uuidof(IUnknown)) ||
			IsEqualIID(interface_id, __uuidof(IDebugOutputCallbacks))) {
			*interface_ptr = (IDebugOutputCallbacks *)this;
			AddRef();
			return S_OK;
		}
		else {
			return E_NOINTERFACE;
		}
	}

	STDMETHODIMP_(ULONG) CWinDebuggerOutputCallbacks::AddRef(THIS) {
		return 1;
	}

	STDMETHODIMP_(ULONG) CWinDebuggerOutputCallbacks::Release(THIS) {
		return 0;
	}

	STDMETHODIMP CWinDebuggerOutputCallbacks::Output(THIS_ IN ULONG mask, IN PCSTR text) {
		UNREFERENCED_PARAMETER(mask);

		// Store output in the variable that may be used by Crash Manager later
		debugger_output_ += text;

		if (verbose_output_) {
			std::cout << text;
		}

		return S_OK;
	}

}