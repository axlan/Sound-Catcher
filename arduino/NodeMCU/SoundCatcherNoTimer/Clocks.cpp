#include "Clocks.h"
#include "LedController.h"
#include "LogBuffer.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

// A UDP instance to let us send and receive packets over UDP
static WiFiUDP udp;

void update_minimal_clock::check_ntp()
{
	// wait 1 seconds before asking for the time again
	if (millis() > start_time + 1000) {
	  ntp_client->update();
	}
		
	cur_hour = ntp_client->getHours();
	cur_minute = ntp_client->getMinutes();
	cur_sec = ntp_client->getSeconds();
}


update_minimal_clock::update_minimal_clock():cur_hour(1), cur_minute(10), cur_sec(15), start_time(0), s_utc_offset(-8) {
   add_val("UTC offset", &s_utc_offset);
   // By default 'pool.ntp.org' is used with 60 seconds update interval and
   // no offset
   ntp_client = new NTPClient(udp);
}

int update_minimal_clock::hour2spoke(int hour) {
	return (hour + s_utc_offset.get_int()) % 12;
}
int update_minimal_clock::min2spoke(int min) {
	return (min % 60) / 5;
}
void update_minimal_clock::update()
{
	
	check_ntp();

	char msg[64];
	sprintf(msg, "%02d:%02d:%02d", cur_hour, cur_minute, cur_sec);
	LogBuffer::Write(msg);
	
	LedController::setall(CRGB::Black);
	
    CRGB col;


	//Purple
	hsv2rgb_rainbow(CHSV(200, 255, 64), col);
	for (int i = 0; i < LedController::NUM_SPOKES; i++) {
		LedController::setled(col, i, LedController::NUM_LED_PER_SPOKE - 1);
	}
	
	//Blue
	hsv2rgb_rainbow(CHSV(160, 255, 64), col);
	LedController::setspoke(col, min2spoke(cur_sec));

	//Green
	hsv2rgb_rainbow(CHSV(96, 255, 64), col);
	LedController::setspoke(col, min2spoke(cur_minute), 0.875);

	//Red
	hsv2rgb_rainbow(CHSV(0, 255, 64), col);
	LedController::setspoke(col, hour2spoke(cur_hour), .625);


	LedController::repaint();
	
	delay(500);
}
