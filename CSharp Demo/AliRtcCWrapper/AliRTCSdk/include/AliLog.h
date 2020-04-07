#ifndef _ALI_LOG_H_
#define _ALI_LOG_H_
#include "AliRtcEngine.h"
#include "ali_rtc_interface.h"

void AliLogEx(AliRTCSdk_Windows::Windows_AliRTCSDKInterface* engine,
    const char* file, int line, const char* tag,
    AliRtcLogLevel level, const char *fmt, ...);

void AliLog(const char *fmt, ...);


#endif
