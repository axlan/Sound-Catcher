#pragma once

#include "Patterns.h"

class update_minimal_clock : public PatternBase{
private:
	int hour2spoke(int hour);
    int min2spoke(int min);
	int cur_hour;
	int cur_minute;
	int cur_sec;
	unsigned long start_time;
	enum State {
		INIT,
		SENT
	};
	State state;
	void check_ntp();
public:
	update_minimal_clock();
	void update();
};
