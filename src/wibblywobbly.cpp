#include "wibblywobbly.h"

WibblyWobbly::WibblyWobbly(const char* ntpServer)
{
    this->bufferSize = 256;
    this->tzOffset = 0;

    this->ntpUdp = new WiFiUDP();
    this->ntpClient = new NTPClient(*ntpUdp);
    this->ntpClient->setPoolServerName(ntpServer);
    this->ntpClient->setUpdateInterval(6000);
}

WibblyWobbly::WibblyWobbly(const char* ntpServer, int tzOffset)
{
    this->bufferSize = 256;
    this->tzOffset = tzOffset;

    this->ntpUdp = new WiFiUDP();
    this->ntpClient = new NTPClient(*ntpUdp);
    this->ntpClient->setPoolServerName(ntpServer);
    this->ntpClient->setUpdateInterval(6000);
}

WibblyWobbly::~WibblyWobbly()
{
    this->ntpClient->end();
    this->ntpUdp->stop();

    if (this->ntpClient != NULL)
        delete this->ntpClient;
    if (this->ntpUdp != NULL)
        delete this->ntpUdp;
}

String WibblyWobbly::getFormattedTime()
{
    refresh();

	char buffer[this->bufferSize];
	time_t epoch = this->ntpClient->getEpochTime() + (this->tzOffset * 3600);
	strftime(buffer, this->bufferSize, "%H:%M:%S", localtime(&epoch));

	return String(buffer);
}

String WibblyWobbly::getFormattedDateTime()
{
    refresh();

	char buffer[this->bufferSize];
	time_t epoch = this->ntpClient->getEpochTime() + (this->tzOffset * 3600);
	strftime(buffer, this->bufferSize, "%Y-%m-%d %H:%M:%S", localtime(&epoch));

	return String(buffer);
}

void WibblyWobbly::refresh()
{
    tzset();

    this->ntpClient->begin();

    while (!ntpClient->isTimeSet())
    {
        this->ntpClient->update();
        delay(5000);
    }
}

struct tm WibblyWobbly::tm()
{
    struct tm *tm;

    time_t now = (time_t) this->ntpClient->getEpochTime();
    tm = localtime(&now);

    return *tm;
}