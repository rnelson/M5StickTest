#ifndef WIBBLY_WOBBLY_TIMEY_WIMEY_H
#define WIBBLY_WOBBLY_TIMEY_WIMEY_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

class WibblyWobbly
{
public:
    WibblyWobbly(const char* ntpServer);
    WibblyWobbly(const char* ntpServer, int tzOffset);
    ~WibblyWobbly();

    String getFormattedTime();
    String getFormattedDateTime();
    struct tm tm();
private:
    void refresh();

    time_t      epochTime;
    size_t      bufferSize;
    int         tzOffset;
    WiFiUDP*    ntpUdp;
    NTPClient*  ntpClient;
};

#endif /* WIBBLY_WOBBLY_TIMEY_WIMEY_H */