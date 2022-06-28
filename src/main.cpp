#define KOLOR_BLACK   0x0000
#define KOLOR_WHITE   0xFFFF
#define KOLOR_GRAY_LT 0xBDF7
#define KOLOR_GRAY_DK 0x7BEF
#define KOLOR_RED     0xF800
#define KOLOR_YELLOW  0xFFE0
#define KOLOR_ORANGE  0xFBE0
#define KOLOR_BROWN   0x79E0
#define KOLOR_GREEN   0x7E0
#define KOLOR_CYAN    0x7FF
#define KOLOR_BLUE    0x1F
#define KOLOR_PINK    0xF81F

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();
uint32_t targetTime = 0;
byte omm = 99;
bool initial = 1;
byte xcolon = 0;

uint32_t kClockForeground = KOLOR_PINK;
uint32_t kClockBackground = TFT_BLACK;
uint32_t kClockColonForeground = KOLOR_ORANGE;

static uint8_t conv2d(const char *p)
{
	uint8_t v = 0;

	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}

// TODO: ntp
uint8_t hh = conv2d(__TIME__),
        mm = conv2d(__TIME__ + 3),
		ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

void setup(void)
{
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(kClockBackground);

	tft.setTextColor(TFT_YELLOW, kClockBackground);

	targetTime = millis() + 1000;
}

void loop()
{
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
			tft.setTextColor(TFT_GREEN, kClockBackground);
			tft.setCursor(26, 52);
			tft.print(__DATE__);

			tft.setTextColor(TFT_BLUE, kClockBackground);
			tft.drawCentreString("It is windy", 120, 48, 2); // Next size up font 2
		}

		// Update digital time
		byte xpos = (TFT_HEIGHT / 2) - tft.fontHeight(7);
		byte ypos = (TFT_WIDTH / 2) - (tft.textWidth("88:88") / 2);
		//byte xpos = 25;
		//byte ypos = 25;

		// Only redraw every minute to minimise flicker
		if (omm != mm)
		{
			tft.setTextColor(kClockBackground, kClockBackground);			
			tft.drawString("88:88", xpos, ypos, 7);

			tft.setTextColor(kClockForeground);
			omm = mm;

			if (hh < 10)
				xpos += tft.drawChar('0', xpos, ypos, 7);

			xpos += tft.drawNumber(hh, xpos, ypos, 7);
			xcolon = xpos;
			tft.setTextColor(kClockColonForeground, kClockBackground);
			xpos += tft.drawChar(':', xpos, ypos, 7);
			tft.setTextColor(kClockForeground, kClockBackground);

			if (mm < 10)
				xpos += tft.drawChar('0', xpos, ypos, 7);

			tft.drawNumber(mm, xpos, ypos, 7);
		}

		if (ss % 2)
		{
			// Flash the colon
			tft.setTextColor(KOLOR_BLACK, kClockBackground);
			xpos += tft.drawChar(':', xcolon, ypos, 7);
			tft.setTextColor(kClockColonForeground, kClockBackground);
		}
		else
		{
			tft.drawChar(':', xcolon, ypos, 7);
		}
	}
}
