/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "Util.hpp"

namespace Anabasis {

	void CUtil::print(char* format, ...) {
		if (format) {
			va_list args;

			va_start(args, format);
			vfprintf(stdout, format, args);
			va_end(args);
		}
	}

	path CUtil::get_image_name_from_command_line(std::string command_line) {
		boost::regex re;
		boost::smatch matches;

		// Check whether the image path is quoted
		if (command_line[0] == '"') {
			re = boost::regex("\"[\\\\]?(.*)\"", boost::regex_constants::perl);
		} else {
			re = boost::regex("[\\\\]?(.*)\\s", boost::regex_constants::perl);
		}

		std::string image_path;
		if (boost::regex_search(command_line, matches, re)) {
			image_path = std::string(matches[1].first, matches[1].second);
		} else {
			return "";
		}

		path image_name = path(image_path).filename();
		return image_name;
	}

	bool CUtil::is_user_an_admin() {
		return IsUserAnAdmin() ? true : false;
	}

}