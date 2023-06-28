#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <cstdio>

namespace FlowGUI {

//this macros define the FLOWGUI_REPORT(<ExceptionType>) that will
//throw an exception and/or log a message to the log file
#if   defined(FLOWGUI_DEBUG_REPORT_LOGFILE) && defined(FLOWGUI_DEBUG_REPORT_THROW)
    extern FILE* logfile;
    #include <time.h>
    inline const std::string currentDateTime()
    {
        time_t     now = time(nullptr);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

        return buf;
    }
    #define FLOWGUI_REPORT(exp) do{if(logfile != nullptr){fprintf(logfile,"[%s in %s at %s]: %s\n",currentDateTime().c_str(),__FILE__,__FUNCTION__,decltype(exp)::msg);fflush(logfile);}throw(exp);}while(false)
#elif defined(FLOWGUI_DEBUG_REPORT_LOGFILE)
    #include <time.h>
    extern FILE* logfile;
    inline const std::string currentDateTime()
    {
        time_t     now = time(nullptr);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

        return buf;
    }
    #define FLOWGUI_REPORT(exp) do{if(logfile != nullptr){fprintf(logfile,"[%s in %s at %s]: %s\n",currentDateTime().c_str(),__FILE__,__FUNCTION__,decltype(exp)::msg);fflush(logfile);}}while(false)
#elif defined(FLOWGUI_DEBUG_REPORT_THROW)
    #define FLOWGUI_REPORT(exp) throw(exp);
#else
    #define FLOWGUI_REPORT(ext) do{}while(0);
#endif

//use this macro to create new custom exceptions
#define MAKE_EXCEPTION(type,caption) class type : public std::exception \
    {                                                                   \
    public:                                                             \
        static constexpr auto msg = caption;                            \
    private:                                                            \
        virtual const char* what() const noexcept override              \
        {                                                               \
            return msg;                                                 \
        }                                                               \
    }


MAKE_EXCEPTION(NodeInvalidAccess    , "accessing empty node method/attribute");
MAKE_EXCEPTION(LinkIsNowEmpty       , "the link has become empty");
MAKE_EXCEPTION(LineWithNoLastLine   , "updating/removing last line when there is not one");
MAKE_EXCEPTION(IndexOutOfRange      , "accessing to an invalid position of the container");
MAKE_EXCEPTION(CodeUnreachable      , "this code should not being under excecution");
//LinkImp
MAKE_EXCEPTION(IllFormedLink        , "the nodes stored in _nodes are not propertly linked");
MAKE_EXCEPTION(UnconnectedNode      , "this node is valid but not connected to another link");
MAKE_EXCEPTION(AlreadyRemoved       , "this node was already removed");

}//namespace FlowGUI

#endif // EXCEPTIONS_H
