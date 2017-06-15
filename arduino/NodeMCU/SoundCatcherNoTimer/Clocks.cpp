#include "Clocks.h"
#include "LedController.h"
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

unsigned int localPort = 2390;      // local port to listen for UDP packets

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
IPAddress timeServerIP(129, 6, 15, 28); // time.nist.gov NTP server
// IPAddress timeServerIP; // time.nist.gov NTP server address
// const char* ntpServerName = "time.nist.gov";

static const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

static byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
static WiFiUDP udp;

// send an NTP request to the time server at the given address
static unsigned long sendNTPpacket(IPAddress& address)
{	
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void update_minimal_clock::check_ntp()
{
	// wait ten seconds before asking for the time again
	if (state == INIT && millis() > start_time + 1000) {
	  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
	  state = SENT;
	} else if(state == SENT) {
		  
		int cb = udp.parsePacket();
		if (!cb) {
			return;
		}
		state = INIT;
		start_time = millis();
		Serial.print("packet received, length=");
		Serial.println(cb);
		// We've received a packet, read the data from it
		udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

		//the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, esxtract the two words:

		unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
		unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;
		Serial.print("Seconds since Jan 1 1900 = " );
		Serial.println(secsSince1900);

		// now convert NTP time into everyday time:
		Serial.print("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
		// subtract seventy years:
		unsigned long epoch = secsSince1900 - seventyYears;
		// print Unix time:
		Serial.println(epoch);

		cur_hour = (epoch  % 86400L) / 3600;
		cur_minute = (epoch  % 3600) / 60;
		cur_sec = epoch % 60;
	}
}


update_minimal_clock::update_minimal_clock() :state(INIT), cur_hour(1),cur_minute(10),cur_sec(15), start_time(0){
   udp.begin(localPort);
//   //get a random server from the pool
//   WiFi.hostByName(ntpServerName, timeServerIP); 
}

int update_minimal_clock::hour2spoke(int hour) {
	return (hour + 6) % 12;
}
int update_minimal_clock::min2spoke(int min) {
	return ((min + 30) % 60) / 5;
}
void update_minimal_clock::update()
{
	
	check_ntp();
	
	LedController::setall(CRGB::Black);
	
	LedController::setspoke(CRGB::Red, hour2spoke(cur_hour));
	LedController::setspoke(CRGB::Blue, min2spoke(cur_minute));
	LedController::setspoke(CRGB::Green, min2spoke(cur_sec));

	LedController::repaint();
	
	delay(100);
}
