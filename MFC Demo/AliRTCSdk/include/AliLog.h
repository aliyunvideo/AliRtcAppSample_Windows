#ifndef _ALI_LOG_H_
#define _ALI_LOG_H_
#include "AliRtcEngine.h"
#include "ali_rtc_interface.h"

ALI_RTC_API void AliLogEx(const char* file, int line, const char* tag,
    AliRtcLogLevel level, const char *fmt, ...);

ALI_RTC_API void AliLog(const char *fmt, ...);


#endif
