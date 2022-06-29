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

/*
	if (targetTime < millis())
	{
		targetTime = millis() + 1000;
		ss++; // Advance second

		if (ss == 60)
		{
			ss = 0;
			omm = mm;
			mm++; // Advance minute

			if (mm > 59)
			{
				mm = 0;
				hh++; // Advance hour

				if (hh > 23)
				{
					hh = 0;
				}
			}
		}

		if (ss == 0 || initial)
		{
			initial = 0;
			
			tft.setTextColor(KOLOR_GRAY_LT, clockBackgroundColor);
			tft.setFreeFont(FSB9);

			byte dateX = (TFT_HEIGHT / 2);
			byte dateY = (TFT_WIDTH / 2) - tft.fontHeight(GFXFF);

			tft.drawCentreString(__DATE__, dateX, dateY, GFXFF);
		}

		// Update digital time
		tft.setCursor(0, 0);
		byte xpos = (TFT_WIDTH / 2) - (tft.textWidth("88:88") / 2);
		byte ypos = (TFT_HEIGHT / 2) - tft.fontHeight(7);

		// Only redraw every minute to minimise flicker
		if (omm != mm)
		{
			tft.setTextColor(clockBackgroundColor, clockBackgroundColor);			
			tft.drawString("88:88", xpos, ypos, 7);

			tft.setTextColor(clockForegroundColor);
			omm = mm;

			if (hh < 10)
				xpos += tft.drawChar('0', xpos, ypos, clockFont);

			xpos += tft.drawNumber(hh, xpos, ypos, clockFont);
			xcolon = xpos;
			tft.setTextColor(colonForegroundColor, clockBackgroundColor);
			xpos += tft.drawChar(':', xpos, ypos, clockFont);
			tft.setTextColor(clockForegroundColor, clockBackgroundColor);

			if (mm < 10)
				xpos += tft.drawChar('0', xpos, ypos, clockFont);

			tft.drawNumber(mm, xpos, ypos, clockFont);
		}
	}
*/
}
