# Anabasis

A simple fuzzing support library for debugging, exception monitoring and crash analysis for Windows.

## Features

- Straightforward API: instantiate an object and call three methods to debug a program until a fatal exception or zero CPU usage.
- A debugger based on Windows Debugger Engine (DbgEng).
- Peach-like crash analysis: a debugger output, registers, a call stack, !exploitable output and (optionally) an input file causing a crash are stored when the crash occurred.
- Means to control a debuggee: kill-on-idle, kill-on-time-elapsed, debug subprocesses, enable PageHeap.

## Why

Many fuzzing frameworks have great abilities to debug programs and to catch crashes but those features are skulked beneath interfaces the frameworks provide. I needed a library that abstracts a piercing debugger and lets me focus on writing fuzzers with C/C++ that do not depend on framework interfaces, scripting languages, etc. As a result, Anabasis can be used from any technology or language supporting native .dll loading.

## Obtain

- Install Visual C++ Redistributable for Visual Studio 2015.
- Install Visual C++ Redistributable for Visual Studio 2012 Update 4 (required by MSEC.dll !exploitable extension).
- Download the repository with binaries:

```
> git clone https://github.com/RVRT/Anabasis
> cd Anabasis\bin32\release
```

- Run tests:

```
> RunWinDebuggerTests.exe
```

- Run an example notepad debugger:

```
> ExampleNotepadDebugger.exe
```

- Run a fully functional debugger and see how an access violation will be handled:

```
> ExampleDebugger.exe --command-line TestWinDebuggerAccessViolation.exe --verbose-output
> cd logs\<crash ID>\<timestamp>
> notepad.exe debugger.log
```

## Use

Here are two examples: the first shows how to debug a new process and the second presents the way to attach to an existing process.

### Debug New Process

```c++
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
```

### Debug Existing Process

```c++
#include "Anabasis.hpp"

using namespace Anabasis;

int main(int argc, char* argv[]) {
    AnabasisResult result;
    
    std::wcout << L"[#] This is an example of a simple debugger based on Anabasis library" << std::endl;
    
    // Create new process and get it's PID
    // (This is a dummy call)
    int pid = create_process();
    
    // No fuzzed file actually because we just attach to the process
    const char* fuzzed_file = "";
    
    CWinDebugger dbg(pid, fuzzed_file);
    
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
```

## Build on Your Own

- Download Boost 1.65.0 and unpack it into `C:\Program Files\boost\boost_1_65_0` directory.
- Run Developer Command Prompt for VS2015 and build the required libraries:

```
> cd "c:\Program Files\boost\boost_1_65_0"
> bootstrap.bat
> b2.exe --with-filesystem --with-program_options --with-regex
```

- Open Anabasis.sln in Visual Studio 2015 and build it as usual.
- If your Boost version or path is different to one shown above, change the next values in Properties menu:

```
Configuration Properties -> C/C++ -> Additional Include Directories
Configuration Properties -> Linker -> Additional Library Directories
```

The projects that should be configured in this case are: Anabasis, ExampleDebugger, ExampleNotepadDebugger, RunWinDebuggerTests.

## Drawbacks

- x64 is not ready.
- One debugger per process: I didn't found a way to use DbgEng.dll from multiple threads to debug several programs simultaneously. You have to design your fuzzer in multi-process manner rather than multi-thread if you need parallel fuzzing. (Details: for example, IDebugControl::WaitForEvent will fail when another thread already called it, even with completely different IDebugControl and IDebugClient instances.)
- Not informative errors: a user gets nothing more than error code and have to dig into Anabasis.hpp to decipher it.

## Credits

- Peach Fuzzer for a splendid example of must-have debugger features.

## License

The library is distributed under the MIT license.
