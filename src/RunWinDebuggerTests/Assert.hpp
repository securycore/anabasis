/*
* Copyright (c) 2017 Nor Tanet
* Distributed under the MIT license.
*/

#pragma once

#include <exception>

class CAssert : public std::exception {
public:
	virtual const char* what() const throw();
};

