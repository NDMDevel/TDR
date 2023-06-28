#include "Exceptions.h"

FILE* logfile = nullptr;

static LogFile logFile;

LogFile::LogFile()
{
    logfile = fopen("../logfile.log","ab+");
}

LogFile::~LogFile()
{
    if( logfile != nullptr )
        fclose(logfile);
}
