#ifndef ALIRTCDEFINE_H
#define ALIRTCDEFINE_H


#if defined(ALIRTCSDK_EXPORTS)
#define ALI_RTC_API __declspec(dllexport)
#else
#define ALI_RTC_API __declspec(dllimport)
#endif

#endif // ALIRTCDEFINE_H
