/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <shlobj.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost::filesystem;

namespace Anabasis {

	class CUtil {
	public:
		static void print(char* format, ...);
		static path get_image_name_from_command_line(std::string command_line);
		static bool is_user_an_admin();
	};

}