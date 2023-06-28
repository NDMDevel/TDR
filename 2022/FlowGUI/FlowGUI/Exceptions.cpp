#include "Exceptions.h"
#include "Style.h"
#include <cstdio>
#include <QDebug>

namespace FlowGUI {

FILE* logfile = nullptr;

struct LogFile
{
    LogFile();
    ~LogFile();
};

static LogFile logFile;

LogFile::LogFile()
{
    logfile = fopen("../FlowGUI.log","ab+");
}

LogFile::~LogFile()
{
    if( logfile != nullptr )
        fclose(logfile);
}

}//namespace FlowGUI
