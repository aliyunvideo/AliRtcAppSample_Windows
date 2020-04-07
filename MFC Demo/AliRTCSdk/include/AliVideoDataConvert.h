#ifndef _ALI_VIDEO_DATA_CONVERTH_
#define _ALI_VIDEO_DATA_CONVERTH_
#include "AliRtcEngine.h"

typedef enum {
  AliRtcConvertVideoFormatUnknow,
  AliRtcConvertVideoFormatI420,
  AliRtcConvertVideoFormatABGR,
}AliRtcConvertVideoFormat;

typedef struct tagAliConvertVideoFrame {
  AliRtcConvertVideoFormat format = AliRtcConvertVideoFormatI420;
  uint8_t *frame = nullptr;
  int32_t width = 0;
  int32_t height = 0;
  int32_t stride_y = 0;
  int32_t stride_u = 0;
  int32_t stride_v = 0;
}AliConvertVideoFrame;

ALI_RTC_API int AliConvertVideoData(const AliConvertVideoFrame &srcFrame, AliConvertVideoFrame *convertFrame);

#endif
