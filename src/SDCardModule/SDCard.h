#include <SD.h>
#include <SPI.h>

class SDCard{
    File file;
    String localPath;
    int loggingIntervalsec;
    bool firstMeasurement = true;
    unsigned long timestampInterval = 0, timeStampStart= 0;
    String formatTime(unsigned long milliseconds);

    public:
    int init(int loggingIntervalSeconds = 30, int CS_pin = 5, String path = "/measurements.txt");
    int writeValue(String val);
    int logDataTimed(String value);
};