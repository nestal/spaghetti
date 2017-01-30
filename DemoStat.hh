/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
*/

//
// Created by nestal on 1/28/17.
//

#pragma once

struct DemoStat
{
	int request_count;
	char last_request[80];
	int last_result;
};

DemoStat f()
{
	DemoStat i;
	i.request_count = 0;
	return i;
}
