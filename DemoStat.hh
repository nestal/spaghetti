/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
*/

//
// Created by nestal on 1/28/17.
//

#pragma once

#include <cstddef>
#include <array>

struct DemoStat
{
	std::size_t request_count;
	std::array<char, 80> last_request;
	std::size_t last_result;
};
