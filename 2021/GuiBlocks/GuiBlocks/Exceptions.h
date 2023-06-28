#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <cstdio>

//this macros define the GUIBLOCK_REPORT(<ExceptionType>) that will
//throw an exception and/or log a message to the log file
#if   defined(GUIBLOCK_DEBUG_REPORT_LOGFILE) && defined(GUIBLOCK_DEBUG_REPORT_THROW)
    #include <time.h>
    inline const std::string currentDateTime()
    {
        time_t     now = time(nullptr);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
    #define GUIBLOCK_REPORT(exp) do{if(logfile != nullptr){fprintf(logfile,"[%s in %s at %s]: %s\n",currentDateTime().c_str(),__FILE__,__FUNCTION__,decltype(exp)::msg);fflush(logfile);}throw(exp);}while(false)
#elif defined(GUIBLOCK_DEBUG_REPORT_LOGFILE)
    #include <time.h>
    inline const std::string currentDateTime()
    {
        time_t     now = time(nullptr);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
    #define GUIBLOCK_REPORT(exp) do{if(logfile != nullptr){fprintf(logfile,"[%s in %s at %s]: %s\n",currentDateTime().c_str(),__FILE__,__FUNCTION__,decltype(exp)::msg);fflush(logfile);}}while(false)
#elif defined(GUIBLOCK_DEBUG_REPORT_THROW)
    #define GUIBLOCK_REPORT(exp) throw(exp);
#else
    #define GUIBLOCK_REPORT(ext) do{}while(0);
#endif

extern FILE* logfile;
struct LogFile
{
    LogFile();
    ~LogFile();
};

class NodeInvalidAccess : public std::exception
{
public:
    static constexpr auto msg = "accessing empty node method/attribute";
private:
    virtual const char* what() const noexcept override
    {
        return msg;
    }
};

class LinkIsNowEmpty : public std::exception
{
public:
    static constexpr auto msg = "the link has become empty";
private:
    virtual const char* what() const noexcept override
    {
        return msg;
    }
};

class LinkWithNoLastLine : public std::exception
{
public:
    static constexpr auto msg = "updating/removing last line when there is not one";
private:
    virtual const char* what() const noexcept override
    {
        return msg;
    }
};

#endif // EXCEPTIONS_H
