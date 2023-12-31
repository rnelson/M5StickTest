#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "wibblywobbly.h"
#include "funwithwifi.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

const size_t  BUFF_SIZE      = 256;
const int     TZ_OFFSET      = -5;
const char*   WIFI_NETWORK   = TOSTRING(WIFI_SSID);
const char*   WIFI_PASSWORD  = TOSTRING(WIFI_PASS);
const char*   NTP_SERVER     = TOSTRING(NTP_POOL);

WiFiMulti wifi;
WibblyWobbly timeyWimey(NTP_SERVER, TZ_OFFSET);

bool initial = true;
bool connected = false;
int currentMinute, lastUpdateMinute;

void setup(void)
{
	Serial.begin(115200);
	M5.begin(true, true, true);

	Serial.printf("Beginning setup()\n");

	M5.lcd.setRotation(1);
	M5.lcd.fillScreen(TFT_GREEN);
	M5.lcd.setTextColor(TFT_BLACK);
	M5.lcd.setCursor(12, 12);
	M5.lcd.setTextSize(2);
	M5.lcd.println("Booting...");

	wifi.addAP(WIFI_NETWORK, WIFI_PASSWORD);

	int attempts = 3;
	while (attempts > 0)
	{
		if (wifi.run() == WL_CONNECTED)
		{
			attempts = -1;
			connected = true;
		}
		else
		{
			attempts--;
			delay(1000);
		}
	}

	Serial.printf("Completed setup()\n");
}

void loop()
{
	Serial.printf("loop()\n");

	/*
	if (initial && !connected)
	{
		M5.lcd.setRotation(1);
		M5.lcd.fillScreen(TFT_BLACK);

		M5.lcd.setCursor(6, 6);
		M5.lcd.setTextSize(5);
		M5.lcd.setTextColor(TFT_RED);
		M5.lcd.println("error:");
		M5.lcd.setCursor(6, 3 + M5.lcd.getCursorY());
		M5.lcd.println("no wifi");

		M5.lcd.setCursor(125, 95);
		M5.lcd.setTextSize(3);
		M5.lcd.setTextColor(TFT_DARKGREY);
		M5.lcd.print(":(");

		initial = false;
		//M5.Beep.setBeep(4000, 2000);
	}

	if (!connected)
		return;

	struct tm now = timeyWimey.tm();
	
	if (initial)
	{
		initial = false;

		lastUpdateMinute = now.tm_hour;
		currentMinute = -1;

		Serial.print("Acquired IP: ");
		Serial.println(WiFi.localIP().toString());

		String dt = timeyWimey.getFormattedDateTime();
		Serial.print("Updated time via NTP: ");
		Serial.println(dt);
	}

	if (lastUpdateMinute != currentMinute)
	{
		M5.lcd.setRotation(1);
		M5.lcd.fillScreen(TFT_BLACK);

		M5.lcd.setCursor(6, 6);
		M5.lcd.setTextSize(1);
		M5.lcd.setTextColor(TFT_WHITE);
		M5.lcd.printf("IP: %s", WiFi.localIP().toString());

		M5.lcd.setCursor(22, 45);
		M5.lcd.setTextSize(4);
		M5.lcd.setTextColor(TFT_DARKCYAN);
		M5.lcd.println(timeyWimey.getFormattedTime());
	}
	*/

	if (initial) {
		M5.lcd.setRotation(1);
		M5.lcd.fillScreen(TFT_BLACK);
		M5.lcd.setTextColor(TFT_GREEN);
		M5.lcd.setCursor(6, 6);
		M5.lcd.setTextSize(1);
	}

	Serial.printf("Delaying 1s\n");
	delay(1000);

	Serial.printf("Spamming funny SSIDs\n");
	beaconSpamList(funnyssids);

	/*
	Serial.printf("Adding Rick Roll SSIDs\n");
	beaconSpamList(rickrollssids);
	*/
}
