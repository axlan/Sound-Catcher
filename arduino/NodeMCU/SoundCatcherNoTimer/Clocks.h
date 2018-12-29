#pragma once

#include "Patterns.h"

class NTPClient;

class update_minimal_clock : public PatternBase{
private:
	int hour2spoke(int hour);
    int min2spoke(int min);
	int cur_hour;
	int cur_minute;
	int cur_sec;
	unsigned long start_time;
	Value s_utc_offset;
	void check_ntp();
	NTPClient* ntp_client;
public:
	update_minimal_clock();
	void update();
};
