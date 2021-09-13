#include "SDCardModule/SDCard.h"



int SDCard::init(int loggingIntervalSeconds, int CS_pin, String path)
{
    loggingIntervalsec = loggingIntervalSeconds;
    localPath = path;
    int ret = (SD.begin(CS_pin))?0:1;
    file = SD.open(path,FILE_APPEND);
    file.println("==== New Measurement ====");
    file.close();
    return ret;
}


int SDCard::writeValue(String val)
{
    file = SD.open(localPath,FILE_APPEND);
    if(file)
    {
        file.print(formatTime(millis()-timeStampStart));
        file.println(val);
        file.close();
        return 0;
    }
    else{
        return 1;
    }
}

int SDCard::logDataTimed(String value)
{
    int ret = 1;

    if(firstMeasurement)
    {
        timeStampStart = millis();
        ret = writeValue(value);
        timestampInterval = millis();
        firstMeasurement = false;
    }
    else{
        if(millis() - timestampInterval > loggingIntervalsec*1000)
        {
            ret = writeValue(value);
            timestampInterval = millis();
        }
    }
    return ret;
}

String SDCard::formatTime(unsigned long milliseconds)
{
    unsigned long allSeconds=milliseconds/1000;
    int runHours= allSeconds/3600;
    int secsRemaining=allSeconds%3600;
    int runMinutes=secsRemaining/60;
    int runSeconds=secsRemaining%60;
    return ((String)runHours+":"+(String)runMinutes+":"+(String)runSeconds+";");
}