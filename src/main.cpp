#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "wibblywobbly.h"

const size_t  BUFF_SIZE  = 256;
const int     TZ_OFFSET  = -5;
const char*   WIFI_SSID  = "intarweb";
const char*   WIFI_PASS  = "mixed compositions";
const char*   NTP_SERVER = "us.pool.ntp.org";

WiFiMulti wifi;
WibblyWobbly timeyWimey(NTP_SERVER, TZ_OFFSET);

bool initial = true;
bool connected = false;
int currentMinute, lastUpdateMinute;

void setup(void)
{
	Serial.begin(115200);
	M5.begin(true, true, true);

	wifi.addAP(WIFI_SSID, WIFI_PASS);

	int attempts = 60;
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
}

void loop()
{
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

		Serial.print("Updated time via NTP: ");
		Serial.println(timeyWimey.getFormattedDateTime());
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

	delay(1000);
}
