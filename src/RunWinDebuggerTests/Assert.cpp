/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#include "Assert.hpp"

const char* CAssert::what() const throw() {
	return "Assert!";
}