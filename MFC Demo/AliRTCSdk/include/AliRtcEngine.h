#ifndef ALIRTCENGINE_H
#define ALIRTCENGINE_H

#include <Windows.h>
#include <stdint.h>
#include "AliRtcDefine.h"
#include "AliString.h"
#include "StringArray.h"
#include "Dictionary.h"

/**
* @brief 频道类型
*/
typedef enum {
  AliRtcCommunication,                          ///< 通信模式
  AliRtcInteractiveLive,                        ///< 直播模式
  AliRtcInteractiveWithLowLatencyLive,          ///< 低延时互动直播错误码
} AliRtcChannelProfile;

/**
* @brief 用户角色
*/
typedef enum {
  AliRtcClientRoleInteractive,              ///< Interactive role
  AliRtcClientRoleLive,                     ///< Live role
} AliRtcClientRole;

/**
* @brief 视频track的类型
*
* 其中AliRtcVideoTrackBoth = AliRtcVideoTrackCamera | AliRtcVideoTrackScreen
*/
typedef enum {
    AliRtcVideoTrackNo     = 0,  ///< no video track
    AliRtcVideoTrackCamera = 1,  ///< video from camera, file, etc.
    AliRtcVideoTrackScreen = 2,  ///< video from screen sharing
    AliRtcVideoTrackBoth   = 3,  ///< both AliRtcVideoTrackCamera and AliRtcVideoTrackScreen
    AliRtcVideoTrackEnd    = 0xffffffff
} AliRtcVideoTrack;

/**
* @brief 音频track的类型
*/
typedef enum {
    AliRtcAudioTrackNo  = 0,  ///< no audio track
    AliRtcAudioTrackMic = 1,  ///< audio from mic, file, etc.
    AliRtcAudioTrackEnd = 0xffffffff
} AliRtcAudioTrack;

/**
* @brief 网络状况
*/
typedef enum {
    AliRtcNetworkQualityExcellent     = 0, //网络极好，流畅度清晰度质量好
    AliRtcNetworkQualityGood          = 1, //网络好，流畅度清晰度和极好差不多
    AliRtcNetworkQualityPoor          = 2, //网络较差，音视频流畅度清晰度有瑕疵，不影响沟通
    AliRtcNetworkQualityBad           = 3, //网络差，视频卡顿严重，音频能正常沟通
    AliRtcNetworkQualityVeryBad       = 4, //网络极差，基本无法沟通
    AliRtcNetworkQualityDisconnect    = 5, //网络中断
    AliRtcNetworkQualityUnknow        = 6, //未知
} AliRtcNetworkQuality;

/**
* @brief 视频窗口填充方式
*/
typedef enum {
    AliRtcRenderModeAuto    = 0, ///< Auto adjust
    AliRtcRenderModeStretch = 1, ///< Simply stretch to fill the view without keeping video aspect ratio
    AliRtcRenderModeFill    = 2, ///< Scaling while keeping video aspect ratio, and fill black bars to the view
    AliRtcRenderModeCrop    = 3, ///< Scaling while keeping video aspect ratio, and crop to fit the view
} AliRtcRenderMode;

/**
* @brief 日志级别
*/
typedef enum {
    AliRtcLogLevelDump,
    AliRtcLogLevelDebug,
    AliRtcLogLevelVerbose,
    AliRtcLogLevelInfo,
    AliRtcLogLevelWarn,
    AliRtcLogLevelError,
    AliRtcLogLevelFatal,
    AliRtcLogLevelNone,
} AliRtcLogLevel;

/**
* @brief 视频流的规格
*/
typedef enum {
    AliRtcVideoProfile_Default = 0, // let sdk decide
    AliRtcVideoProfile_180_240P_15,
    AliRtcVideoProfile_180_320P_15,
    AliRtcVideoProfile_180_320P_30,
    AliRtcVideoProfile_240_320P_15,
    AliRtcVideoProfile_360_480P_15,
    AliRtcVideoProfile_360_480P_30,
    AliRtcVideoProfile_360_640P_15,
    AliRtcVideoProfile_360_640P_30,
    AliRtcVideoProfile_480_640P_15,
    AliRtcVideoProfile_480_640P_30,
    AliRtcVideoProfile_720_960P_15,
    AliRtcVideoProfile_720_960P_30,
    AliRtcVideoProfile_720_1280P_15,
    AliRtcVideoProfile_720_1280P_30,
    AliRtcVideoProfile_1080_1920P_15,
    AliRtcVideoProfile_1080_1920P_30, 
    AliRtcVideoProfile_480_640P_15_1500Kb,
} AliRtcVideoProfile;

typedef enum {
    AliRtcAudioEffect_Reverb_Off = 0,              //关闭
    AliRtcAudioEffect_Reverb_Vocal_I = 1,          //人声 I
    AliRtcAudioEffect_Reverb_Vocal_II = 2,          //人声 II
    AliRtcAudioEffect_Reverb_Bathroom = 3,         //澡堂
    AliRtcAudioEffect_Reverb_Small_Room_Bright = 4,//明亮小房间
    AliRtcAudioEffect_Reverb_Small_Room_Dark = 5,  //黑暗小房间
    AliRtcAudioEffect_Reverb_Medium_Room = 6,      //中等房间
    AliRtcAudioEffect_Reverb_Large_Room = 7,       //大房间
    AliRtcAudioEffect_Reverb_Church_Hall = 8,      //教堂走廊
    AliRtcAudioEffect_Reverb_Cathedral = 9,        //大教堂
    AliRtcAudioEffect_Reverb_MAX,
}AliRtcAudioEffectReverbMode;

static const float RbParam[][8] =
{
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},         //0 do not use
    {70.0f, 20.0f, 40.0f, 99.0f, 100.0f, 50.0f, -5.0f, -12.0f},//Vocal I
    {50.0f, 0.0f, 50.0f, 99.0f, 50.0f, 100.0f, -5.0f, -6.0f},  //Vocal II
    {16.0f, 8.0f, 80.0f, 0.0f, 0.0f, 100.0f, -5.0f, -6.0f},    //Bathroom
    {30.0f, 10.0f, 50.0f, 50.0f, 50.0f, 100.0f, -5.0f, -1.0f}, //Small Room Bright
    {30.0f, 10.0f, 50.0f, 50.0f, 100.0f, 0.0f, -5.0f, -1.0f},  //Small Room Dark
    {75.0f, 10.0f, 40.0f, 50.0f, 100.0f, 70.0f, -5.0f, -1.0f}, //Medium Room
    {85.0f, 10.0f, 40.0f, 50.0f, 100.0f, 80.0f, -6.0f, 0.0f},  //Large Room
    {90.0f, 32.0f, 60.0f, 50.0f, 100.0f, 50.0f, -12.0f, 0.0f}, //Church Hall
    {90.0f, 16.0f, 90.0f, 50.0f, 100.0f, 0.0f, -20.0f, 0.0f},  //Cathedral
};

/**
 AudioEffect Reverb mode
 */
typedef enum {
    AliRtcAudioEffect_Reverb_Room_Size = 0, //房间大小 [0, 100]
    AliRtcAudioEffect_Reverb_Pre_Delay,     //预延时ms [0, 200]
    AliRtcAudioEffect_Reverb_Reverberance,  //混响感[0,100]
    AliRtcAudioEffect_Reverb_Hf_Damping,    //消声 [0,100]
    AliRtcAudioEffect_Reverb_Tone_Low,      //低音调[0,100]
    AliRtcAudioEffect_Reverb_Tone_High,     //高音调[0,100]
    AliRtcAudioEffect_Reverb_Dry_Gain,     //干增益[-20,10]
    AliRtcAudioEffect_Reverb_Wet_Gain,     //湿增益[-20,10]
    AliRtcAudioEffect_Reverb_Type_Max,
}AliRtcAudioEffectReverbParamType;

/**
* @brief 外接设备状态
*/
typedef enum {
  AliRtcExternalDeviceAdd = 0,      //新增外接设备
  AliRtcExternalDeviceRemove = 1,   //移除外接设备
}AliRtcExternalDeviceState;

/**
* @brief 视频缩放比例
*/
typedef enum {
  AliRtcScaleRatio_16_9 = 0,
  AliRtcScaleRatio_4_3,
}AliRtcScaleRatio;

/**
* @brief OnBye返回类型
*/
typedef enum {
  AliRtcOnByeBeKickOut = 1,               //当前用户被踢出频道
  AliRtcOnByeChannelTerminated = 2,       //频道结束
  AliRtcOnByeUserReplaced = 3,            //相同用户在其他设备加入频道，当前设备被下线
}AliRtcOnByeType;

/**
* @brief 桌面分享类型
*/
typedef enum  {
  AliRtcScreenShareDesktop,      //桌面分享
  AliRtcScreenShareWindow,       //窗口分享
}AliRtcScreenShareType;

/**
* @brief 音频声道类型
*/
typedef enum {
  AliRtcMonoAudio = 0,
  AliRtcStereoAudio = 1,
}AliRtcAudioNumChannelType;

/**
* @brief 音频采样率类型
*/
typedef enum {
  AliRtcAudioSampleRate_8000 = 0,
  AliRtcAudioSampleRate_11025 = 1,
  AliRtcAudioSampleRate_16000 = 2,
  AliRtcAudioSampleRate_22050 = 3,
  AliRtcAudioSampleRate_32000 = 4,
  AliRtcAudioSampleRate_44100 = 5,
  AliRtcAudioSampleRate_48000 = 6,
}AliRtcAudioSampleRate;

/**
* @brief 音频数据源
*/
typedef enum {
  AliRtcAudiosourcePub        = 0,
  AliRtcAudiosourceSub        = 1,
  AliRtcAudiosourceRawData    = 2,
  AliRtcAudioSourceVolume     = 3,
}AliRtcAudioSource;

/**
* @brief 视频数据源
*/
typedef enum {
  AliRtcVideoSourceCamera = 0,
  AliRtcVideoSourceScreenShare = 1,
  AliRtcVideosourceTypeMax,
}AliRtcVideoSource;

/**
* @brief 视频数据类型
*/
typedef enum {
  AliRtcVideoFormatUnknow = -1,
  AliRtcVideoFormatBGRA = 0,
  AliRtcVideoFormatI420,
  AliRtcVideoFormatNV21,
  AliRtcVideoFormatNV12,
}AliRtcVideoFormat;

/**
* @brief 视频数据类型
*/
typedef enum {
  AliRtcBufferTypeRawData = 0,
  AliRtcBufferTypeCVPixelBuffer,
  AliRtcBufferTypeTexture,
}AliRtcBufferType;

/**
* @brief 本地静音模式
*/
typedef enum 
{
  AliRtcMuteLocalAudioModeDefault = 0,
  AliRtcMuteAllLocalAudioMode = 1,
  AliRtcMuteOnlyMicAudioMode = 2,
  AliRtcMuteLocalAudioModeMAX
}AliRtcMuteLocalAudioMode;

/**
* @brief SDK错误码
*/
typedef enum {
  /////// 加入频道错误码 //////////
  ERR_JOIN_BAD_APPID                         = 0x02010201, //AppId不存在
  ERR_JOIN_INVALID_APPID                     = 0x02010202, //AppId已失效
  ERR_JOIN_BAD_CHANNEL                       = 0x02010204, //频道不存在
  ERR_JOIN_INVALID_CHANNEL                   = 0x02010203, //频道已失效
  ERR_JOIN_BAD_TOKEN                         = 0x02010205, //Token不存在
  ERR_JOIN_TIMEOUT                           = 0x01020204, //加入频道超时
  ERR_JOIN_BAD_PARAM                         = 0x01030101, //加入频道参数错误
  ///////// 设备错误码 ////////////
  ERR_MIC_OPEN_FAIL                          = 0x01040404, //采集设备初始化失败
  ERR_SPEAKER_OPEN_FAIL                      = 0x01040405, //播放设备初始化失败
  ERR_MIC_INTERRUPT                          = 0x01040406, //采集过程中出现异常
  ERR_SPEAKER_INTERRUPT                      = 0x01040407, //播放过程中出现异常
  ERR_MIC_AUTH_FAIL                          = 0x01040408, //麦克风设备授权失败
  ERR_MIC_NOT_AVAILABLE                      = 0x01040409, //麦克风设备未授权
  ERR_SPEAKER_NO_AVAILABLE                   = 0x01040410, //无可用的音频采集设备
  ERR_CAMERA_OPEN_FAIL                       = 0x01040104, //相机设备初始化失败
  ERR_CAMERA_INTERRUPT                       = 0x01040106, //相机采集中发生异常
  ERR_VIDEO_DISPLAY_OPEN_FAIL                = 0x01040201, //渲染设备初始化失败
  ERR_VIDEO_DISPLAY_INTERRUPT                = 0x01040202, //渲染过程中出现异常
  ERR_AUDIO_BUFFER_FULL                      = 0x01070101, //音频输入buffer满
  ///////// 网络错误码 ////////////
  ERR_ICE_CONNECTION_CONNECT_FAIL            = 0x01050201, //媒体通道建立失败
  ERR_ICE_CONNECTION_RECONNECT_FAI           = 0x01050202, //媒体通道重连失败
  ERR_ICE_CONNECTION_HEARTBEAT_TIMEOUT       = 0x0102020C, //信令心跳超时
  ///////// 低延时互动直播错误码 ////////////
  ERR_LOWLATENCY_LIVE_COMMUNICATION_FAIL = 0x01070003,              //低延时互动直播信令失败
  ERR_LOWLATENCY_LIVE_MEDIA_SETTING_FAIL = 0x01070004,              //低延时互动直播媒体设置错误
  ERR_LOWLATENCY_LIVE_NONE_BROADCASTER = 0x01070005,                //低延时互动直播当前房间没有主播推流
  ERR_LOWLATENCY_LIVE_NEED_RESTART = 0x01070006,                    //低延时互动直播错误，应用层需要重新创建新的SDK实例
  ERR_LOWLATENCY_LIVE_NEEDRESTART_AGAIN_LATER= 0x01070007,          //低延时互动直播错误，应用层需要稍等一定时间（自定义间隔）重新创建新的SDK实例
  ERR_LOWLATENCY_LIVE_SERVICE_UNAVAILABLE = 0x01070008,             //低延时互动直播服务不可用或者服务降级
  ERR_LOWLATENCY_LIVE_MEDIA_CONNECTION_INTERRUPTION = 0x01070009,   //低延时互动直播网络中断错误

  ///////// 其他错误码 ////////////
  ERR_SDK_INVALID_STATE                      = 0x01030204, //SDK内部状态错误
  ERR_SESSION_REMOVED                        = 0x02010105, //session已经被移除
  ERR_INNER                                  = -1,         //SDK内部错误
}ErrorCodeEnum;


/**
 * @brief 与应用服务器对接的用户建权信息，在joinChannel时使用
 *
 * AliRtc::String, 是SDK自定义的字符串类型
 * AliRtc::StringArray, 是SDK自定义的字符串数组类型
 */
typedef struct tagAliRtcAuthInfo
{
    AliRtc::String channel;
    AliRtc::String user_id;
    AliRtc::String appid;
    AliRtc::String nonce;
    AliRtc::String token;
    AliRtc::StringArray gslb;
    long long timestamp;
    AliRtc::StringArray agent;
} AliRtcAuthInfo;

typedef struct tagAliRtcScreenSourceList
{
  long long sourceNum;
  AliRtc::StringArray sourceId;
  AliRtc::StringArray sourceTitle;
} AliRtcScreenSourceList;

typedef struct tagAliRtcScreenShareRegion {
  float originX = -1.f;
  float originY = -1.f;
  float width = 0.f;
  float height = 0.f;
}AliRtcScreenShareRegion;

typedef struct tagAliRtcScreenSource
{
  AliRtcScreenShareType eType = AliRtcScreenShareDesktop;
  AliRtc::String sourceId;
  AliRtc::String sourceTitle;
  bool isShareByRegion = false;
  AliRtcScreenShareRegion shareRegion;

  void clear()
  {
    eType = AliRtcScreenShareDesktop;
    sourceId = "";
    sourceTitle = "";

    shareRegion.originX = -1.f;
    shareRegion.originY = -1.f;
    shareRegion.width = 0.f;
    shareRegion.height = 0.f;
  }

} AliRtcScreenSource;

typedef struct tagAliRtcDeviceList
{
  long long deviceCount;
  AliRtc::StringArray deviceIds;
  AliRtc::StringArray deviceNames;
} AliRtcDeviceList;

/**
* @brief 会话数据统计信息
*/
typedef struct tagAliRtcStats {
  int64_t sent_kbitrate = 0;
  int64_t rcvd_kbitrate = 0;
  int64_t sent_bytes = 0;
  int64_t rcvd_bytes = 0;
  int64_t video_rcvd_kbitrate = 0;
  int64_t video_sent_kbitrate = 0;
  uint32_t system_cpu = 0;
  uint32_t app_cpu = 0;
  uint64_t call_duration = 0;
}AliRtcStats;

/**
* @brief 视频数据
*/
typedef struct tagAliRtcVideoDataSample {
  AliRtcVideoFormat format;
  AliRtcBufferType bufferType;
  unsigned char *data = nullptr;
  size_t dataLen;
  int strideY = 0;
  int strideU = 0;
  int strideV = 0;
  int height = 0;
  int width = 0;
  int rotation = 0;
  long long timeStamp;
}AliRtcVideoDataSample;

/**
* @brief 音频数据
*/
typedef struct tagAliRtcAudioDataSample {
  unsigned char *data{nullptr};
  int numOfSamples{0};
  int bytesPerSample{0};
  int numOfChannels{0};
  int samplesPerSec{0};
}AliRtcAudioDataSample;
/*
 @brief 录制类型
 - AliRtcRecordTypeAudio: 纯音频录制
 - AliRtcRecordTypeVideo: 纯视频录制
 - AliRtcRecordTypeBoth: 音视频录制
*/
typedef enum {
    AliRtcRecordTypeAudio = 0,
    AliRtcRecordTypeVideo,
    AliRtcRecordTypeBoth,
}AliRtcRecordType;

/**
 @brief 录制格式

 - AliRtcRecordFormatAAC: AAC文件
 - AliRtcRecordFormatWAV: WAV文件
 - AliRtcRecordFormatMP4: MP4文件
*/
typedef enum {
    AliRtcRecordFormatAAC = 0,
    AliRtcRecordFormatWAV,
    AliRtcRecordFormatMP4,
}AliRtcRecordFormat;

/**
 @brief 录制音频质量

 - AliRtcAudioQualityLow: 低
 - AliRtcAudioQualityMidium: 中
 - AliRtcAudioQualityHigh: 高
*/
typedef enum {
    AliRtcAudioQualityLow = 0,
    AliRtcAudioQualityMidium,
    AliRtcAudioQualityHigh,
}AliRtcAudioQuality;

/**
 @brief 录制视频质量

 - AliRtcVideoQualityDefault: 默认，和采集保持一致
*/
typedef enum {
    AliRtcVideoQualityDefault = 0,
}AliRtcVideoQuality;

/**
 @brief 录制音频设置

 - sampleRate: 音频采样率
 - quality: 音频质量
 - externalPcmCaptureRecording: 允许外部pcm输入录制
 - externalPcmRenderRecording: : 允许外部pcm渲染录制
*/
typedef struct  {
    AliRtcAudioSampleRate sampleRate = AliRtcAudioSampleRate_16000;
    AliRtcAudioQuality quality = AliRtcAudioQualityHigh;
	bool externalPcmCaptureRecording = false;
	bool externalPcmRenderRecording = true;
}AliRtcRecordAudioConfig;

/**
 @brief 录制视频设置

 - quality: 视频质量
*/
typedef struct  {
    AliRtcVideoQuality quality; //视频质量
}AliRtcRecordVideoConfig;

/**
* @brief 本地视频统计信息
*/
typedef struct tagAliRtcLocalVideoStats {
  AliRtcVideoTrack track;
  uint32_t sent_bitrate = 0;
  uint32_t sent_fps = 0;
  uint32_t encode_fps = 0;
}AliRtcLocalVideoStats;

/**
* @brief 远端视频统计信息
*/
typedef struct tagAliRtcRemoteVideoStats {
  AliRtc::String user_id;
  AliRtcVideoTrack track;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t decode_fps = 0;
  uint32_t render_fps = 0;
  uint32_t frozen_times = 0;
}AliRtcRemoteVideoStats;

#define CMD_ACCOMPANIMENT_TYPE          (0)  // 设置、关闭 只推伴奏
#define CMD_AUDIO_MIC_SET_VOL           (1)  // 软件设置MIC音量
#define CMD_AUDIO_PLAYER_SET_VOL        (2)  // 软件设置播放器音量

#define PRIVATE_CMD_HANDLE              (-1)  // 私有接口句柄


/**
* @brief 控制命令接口 私有接口
*/
typedef struct __PrivateCmdObj
{
  /**
  * @Cmd CMD_ACCOMPANIMENT_TYPE
  *      CMD_AUDIO_MIC_SET_VOL
  *      CMD_AUDIO_PLAYER_SET_VOL
  */
  int32_t     Cmd;

  /**
  *
  * CMD_ACCOMPANIMENT_TYPE
  * @CmdValue 0/1 伴奏和MIC混音
  *
  * CMD_AUDIO_MIC_SET_VOL/CMD_AUDIO_PLAYER_SET_VOL
  * @CmdValue 0..100 音量
  */

  int32_t     CmdValue; // 0..100

  /**
  * @CmdExt 备用
  */
  int64_t     CmdExt;   // 备用 0
}PrivateCmd, *PPrivateCmd;

class ALI_RTC_API  AliMediaDeviceTestEventListener {
public:
  virtual void OnAudioDeviceRecordLevel(int level) = 0;
  virtual void OnAudioDevicePlayoutLevel(int level) = 0;
  virtual void OnAudioDevicePlayoutEnd() = 0;
  virtual ~AliMediaDeviceTestEventListener() = default;
};

class ALI_RTC_API AliMediaDeviceTestInterface {
public:
  virtual ~AliMediaDeviceTestInterface() {}
  virtual int StartTestAudioRecord(const char * deviceName, int nTimeInv) = 0;
  virtual int StartTestAudioRecordById(const char *deviceId, int nTimeInv) = 0;
  virtual int StopTestAudioRecord() = 0;
  virtual int StartTestAudioPlayout(const char * deviceName, int nTimeInv, const char *wavFile, int loopCycles = 0) = 0;
  virtual int StartTestAudioPlayoutById(const char *deviceId, int nTimeInv, const char *wavFile, int loopCycles = 0) = 0;
  virtual int StopTestAudioPlayout() = 0;
  virtual void Release() = 0;
};

class ALI_RTC_API AliMediaInputStreamInterface
{
public:
	// API for system audio recording

  /**
  * @brief 私有接口
  * @flag 是否开启系统音频录制，录制系统播放的声音
  * @result = 0 succ other fail
  */
	virtual int EnableSystemAudioRecording(bool flag) = 0;
	virtual bool IsSystemAudioRecording() = 0;
	virtual int SetSystemAudioRecordingVolScal(int volScal) = 0;
	virtual int GetSystemAudioRecordingVolScal() = 0;

	virtual int AddAudioStream(size_t num_channels, size_t sample_rate, const void* config) = 0;
	virtual int RemoveAudioStream(int stream_handle) = 0;
	virtual int SendAudioData(int stream_handle, size_t sample_length, const void* audio_samples) = 0;
	virtual int ClearAudioCache(int stream_handle) = 0;

  /**
  * @brief 私有接口
  * @stream_handle = PRIVATE_CMD_HANDLE
  * @config = PPrivateCmd  具体调用参考定义的说明
  */
	virtual int UpdateAudioStreamConfig(int stream_handle, const void* config) = 0;
	virtual int SetStreamVolScal(int stream_handle, int volScal) = 0;
	virtual int GetStreamVolScal(int stream_handle) = 0;

	virtual void Reset() = 0;
	virtual void Release() = 0;
};

/**
 * @brief 视频显示窗口设置
 */
class ALI_RTC_API AliVideoCanvas
{
public:
	AliVideoCanvas();
	AliVideoCanvas(const AliVideoCanvas &other);
    HWND hWnd;                   ///< could be NULL
    AliRtcRenderMode renderMode; ///< default is AliRenderModeFill
    AliRtcScaleRatio scaleRatio{ AliRtcScaleRatio_16_9 }; ///< preview scale ratio
    bool flip;                   ///< if true:mirror filp
};

/**
 * @brief 事件监听的基础类
 *
 * application可以继承这个基础类用来处理感兴趣的事件
 */
class ALI_RTC_API AliRtcEventListener
{
public:
    /**
    * 加入频道回调
    * @param result 加入频道结果，成功返回0，失败返回错误码
    */
    virtual void onJoinChannelResult(int result) {};

    /**
    * @brief 离开频道结果
    * @param result 离开频道结果，成功返回0，失败返回错误码
    * @note 调用leaveChannel接口后返回，如果leaveChannel后直接destroy SDK，将不会收到此回调
    */
    virtual void onLeaveChannelResult(int result) {}

    /**
    * 推流结果回调
    * @param result 回调结果，成功返回0，失败返回错误码
    * @param isPublished 是否处于推流状态 YES:推流 NO:停止推流
    * @note 此回调在自动推流完成或手动调用publish接口推流完成返回
    * @note 手动调用publish下此回调与publish接口设置回调均会触发
    * @note 自动推流模式下建议使用此回调做推流状态的监听处理
    */
    virtual void onPublishChangedNotify(int result, bool isPublished) {};

    /**
     * @brief 当远端用户上线时会返回这个消息
     * @param uid         User ID。从App server分配的唯一标示符
     * @note This message does not mean the user remains online state
     */
    virtual void onRemoteUserOnLineNotify(const AliRtc::String &uid) {}

    /**
     * @brief 当远端用户下线时会返回这个消息
     * @param uid         User ID。从App server分配的唯一标示符
     * @note This message does not mean the user remains offline state
     */
    virtual void onRemoteUserOffLineNotify(const AliRtc::String &uid) {}

    /**
     * @brief 当远端用户的流发生变化时，返回这个消息
     * @param uid         User ID。从App server分配的唯一标示符
     * @param audioTrack         音频流类型，详见AliRtcAudioTrack
     * @param videoTrack         视频流类型，详见AliRtcVideoTrack
     * @note 远方用户停止推流，也会发送这个消息
     */
    virtual void onRemoteTrackAvailableNotify(const AliRtc::String &uid,
                                              AliRtcAudioTrack audioTrack,
                                              AliRtcVideoTrack videoTrack) {}

    /**
     * @brief 当订阅情况发生变化时，返回这个消息
     * @param uid         User ID。从App server分配的唯一标示符
     * @param audioTrack         音频流类型，详见AliRtcAudioTrack
     * @param videoTrack         视频流类型，详见AliRtcVideoTrack
     */
    virtual void onSubscribeChangedNotify(const AliRtc::String &uid,
                                          AliRtcAudioTrack audioTrack,
                                          AliRtcVideoTrack videoTrack) {}

    /**
     * @brief 网络质量变化时发出的消息
     * @param uid         网络质量发生变化的用户uid
     * @param upQuality   上行网络质量，详见AliRtcNetworkQuality
     * @param downQuality 下行网络质量，详见AliRtcNetworkQuality
     * @note 当网络质量发生变化时触发，uid为空时代表用户自己的网络质量发生变化
     */
    virtual void onNetworkQualityChanged(const AliRtc::String &uid,
                                         AliRtcNetworkQuality upQuality,
                                         AliRtcNetworkQuality downQuality) {}

    /**
     * @brief 被服务器踢出频道的消息
     * param code onBye类型，详见AliRtcOnByeType
     */
    virtual void onBye(int code) {}

    /**
     * @brief 如果engine出现warning，通过这个消息通知app
     * @param warn  Warning type
     */
    virtual void onOccurWarning(int warn) {}

    /**
     * @brief 如果engine出现error，通过这个消息通知app
     * @param error  Error type
     */
    virtual void onOccurError(int error) {}

    /**
     * @brief remote user的第一帧视频帧显示时触发这个消息
     * @param uid   User ID。从App server分配的唯一标示符
     * @param videoTrack 屏幕流或者相机流
     */
    virtual void onFirstRemoteVideoFrameDrawn(const AliRtc::String &uid, AliRtcVideoTrack videoTrack) {}

    /**
    * @brief 外接设备状态变更
    * @param deviceName 外接设备名称
    * @param state 外接设备状态
    */
    virtual void onExternalDeviceStateChange(const AliRtc::String &deviceName, AliRtcExternalDeviceState state) {};

    /**
    * @brief 首包发送回调
    * @param audioTrack 是否音频首包发送
    * @param videoTrack 是否视频首包发送
    */
    virtual void onFirstPacketSent(AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) {};

    /**
    * @brief 首包接收回调
    * @param audioTrack 是否音频首包接收
    * @param videoTrack 是否视频首包接收
    */
    virtual void onFirstPacketReceived(AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) {};

    /**
    * @brief 网络断开
    */
    virtual void onConnectionLost() {};

    /**
    * @brief 开始重连
    */
    virtual void onTryToReconnect() {};

    /**
    * @brief 重连成功
    */
    virtual void onConnectionRecovery() {};

    /**
    * @brief 用户视频渲染裸数据回调
    * @param uid 视频数据对应用户
    * @param videoTrack 视频数据流类型
    * @param data 视频数据指针
    * @param width 视频数据宽度
    * @param height 视频数据高度
    * @param flip 视频数据镜像
    * @note configExternalVideoRendering接口配置打开后，会接收到此回调，返回用户视频数据，视频数据格式为ARGB
    */
    virtual void onVideoRenderData(const AliRtc::String &uid, AliRtcVideoTrack videoTrack,
      unsigned char* data, unsigned int width, unsigned int height, bool flip) {};

    /**
    * @brief 当前角色变化通知回调
    *
    * @param old_role 切换前的角色
    * @param new_role 切换后的角色
    * @note 调用setClientRole方法切换角色成功时触发此回调
    */
    virtual void onUpdateRoleNotify(const AliRtcClientRole old_role, const AliRtcClientRole new_role) {};

    /**
    * @brief 当前会话统计信息回调
    * @param stats 会话统计信息
    * @note SDK每两秒触发一次此统计信息回调
    */
    virtual void onRTCStats(const AliRtcStats& stats) {};

    /**
    * @brief 本地视频统计信息
    * @param local_video_stats 本地视频统计信息
    * @note SDK每两秒触发一次此统计信息回调
    */
    virtual void onRTCLocalVideoStats(
      const AliRtcLocalVideoStats& local_video_stats) {};

    /**
    * @brief 远端视频统计信息
    * @param remote_video_stats 远端视频统计信息
    * @note SDK每两秒触发一次此统计信息回调
    */
    virtual void onRTCRemoteVideoStats(
      const AliRtcRemoteVideoStats& remote_video_stats) {};

    /**
    * @brief 性能不足通知
    */
    virtual void onPerformanceLow() {};

    /**
    * @brief 性能恢复通知
    */
    virtual void onPermormanceRecovery() {};

    /**
    * @brief 订阅的音频数据回调
    * @param audioSource audio source
    * @param audioSample audio sample
    */
    virtual void onAudioSampleCallback(AliRtcAudioSource type, AliRtcAudioDataSample *audioSample) {};

    /**
    * @brief 用户音量大小回调
    * @param type 音频数据源类型
    * @param volume 音量大小
    * @param uid 回调音量对应的用户User ID
    * @param audiostate 本地说话人的状态， 1表示检测到本地语音，0表示未检测到本地语音
    * @note 用户User ID为"0"表示本地推流音量，"1"表示远端混音音量，其他表示用户的音量
    */
    virtual void onAudioVolumeCallback(AliRtcAudioSource type, int volume, const AliRtc::String &uid, int audiostate) {};

    /**
    * @brief 最后一公里网络质量探测回调
    * @param networkQuality 网络质量
    */
    virtual void onLastmileDetectResultWithQuality(AliRtcNetworkQuality networkQuality) {};

    /**
    * @brief 本地采集视频数据回调
    * @param videoSource 视频数据类型
    * @param videoSample 视频数据
    */
    virtual void onCaptureVideoSample(AliRtcVideoSource videoSource, AliRtcVideoDataSample *videoSample) {};

    /**
    * @brief 远端视频数据回调
    * @param uid user id
    * @param videoSource video source
    * @param videoSample video sample
    */
    virtual void onRemoteVideoSample(const AliRtc::String &uid, AliRtcVideoSource videoSource, AliRtcVideoDataSample *videoSample) {};
    
    /**
    * @brief 录制事件回调
    * @event 录制事件
    * @param ilePath 录制文件路径
    */
    virtual void onMediaRecordEvent(int event, const AliRtc::String &filePath) {};
};

/**
* @brief RTC引擎的主要控制类
*
* 实现所有RTC音视频引擎功能。用户通过创建此类的实例来调用引擎功能。
*/
class ALI_RTC_API AliRtcEngine
{
public:
    //#pragma mark - "Engine的生命周期，工作参数以及频道操作"
    /**
     * @brief 获取一个AliRtcEngine实例
     * @param listener  用于接收来自engine的消息
     * @param extras  用于接收来自客户灰度下发的参数
     */
    static AliRtcEngine *sharedInstance(AliRtcEventListener *listener, const AliRtc::String &extras);

    /**
    * @brief 销毁AliRtcEngine的实例
    */
    static void destroy();

    /**
    * @brief 获取sdk版本号
    * @return SDK版本号
    * @note 在任何时刻都可以调用
    */
    static const char *getSdkVersion();

    /**
    * @brief 设置SDK日志文件保存路径
    * @param logDirPath 日志文件保存绝对路径
    * @return 0为成功，非0失败
    * @note 如需调用此接口，请在调用所有SDK接口前先进行设置，避免日志出现丢失，同时App必须保证指定的目录已存在且可写入
    */
    static int setLogDirPath(const AliRtc::String &logDirPath);

    /**
    * @brief 上报日志
    */
    static void uploadLog();

    /**
     * @brief 设置是否自动publish音视频流和subscribe音视频流
     * @param autoPub    true表示自动推流；false表示不自动推流
     * @param autoSub    true表示自动拉流；false表示不自动拉流
     * @note 缺省是自动推流和拉流。只可以在joinChannel之前设置,joinChannel之后设置无效
     */
	virtual int setAutoPublishSubscribe(bool autoPub, bool autoSub) = 0;

    /**
    * @brief 设置音频模式还是音视频模式
    * @param audioOnly  true表示只有音频推流和拉流；false表示音视频都支持
    * @note 只可以在joinChannel之前设置
    */
    virtual int setAudioOnlyMode(bool audioOnly) = 0;

    /**
    * @brief 设置频道模式
    * @param[in] channelProfile 频道模式类型
    * @return 0为成功，非0失败
    * @note 只可以在入会之前调用，会议中不可以重新设置，LeaveChannel后可以重新设置
    * @note 设置为AliRtcInteractiveLive模式时，用户角色默认为AliRtcClientRoleLive
    */
    virtual int setChannelProfile(const AliRtcChannelProfile channelProfile) = 0;

    /**
    * @brief 设置用户角色
    * @param[in] clientRole 用户角色类型
    * @return 0为成功，非0失败
    * @note 只可以在频道模式非AliRtcChannelProfileCommunication下调用，入会前/会议中均可设置，入会后设置成功会收到onUpdateRoleNotify
    * @note 从Interactive转换为Live角色需要先停止推流，否则返回失败
    * @note 频道模式为AliRtcInteractiveLive模式时，用户角色默认为AliRtcClientRoleLive
    */
    virtual int setClientRole(const AliRtcClientRole clientRole) = 0;

    /**
     * @brief 加入频道
     * @param authInfo    认证信息，从App Server获取。
     * @param userName    任意用于显示的用户名称。不是User ID
     * @param onResult    当joinChannel执行结束后调用这个回调
     * @param opaquePtr   app提供的参数，在调用onResult时传回app
     * @note 异步接口。是否成功入会，通过onResult判断
     *       onResult: app设置的回调函数，可以为NULL。第一个参数opaquePtr
     *       是app设置的值
     *       lambda表达式转换成onResult:\
     * @code void (*foo)(void *, int);
     *       foo = [](void *opaquePtr, int errCode) {
     *           ClassA *pThis = (ClassA *)opaquePtr;
     *           if(errCode != 0) {
     *               pThis->OutputError("Failed to exeucte joinChannel.");
     *           }
	 *       };
     * @endcode
     */
    virtual void joinChannel(const AliRtcAuthInfo &authInfo,
                     const AliRtc::String &userName,
                     void (*onResult)(void *opaquePtr, int errCode),
                     void *opaquePtr) = 0;

    /**
     * @brief 离开频道
     * @note 如果当前不在频道内，leaveChannel不会有任何影响，leaveChannel会产生消息通知频道内其他用户
     * @note 离开频道结果回通过onLeaveChannelResult回调返回
     */
	virtual void leaveChannel() = 0;

    //#pragma mark - "设置和操作本地的媒体"

    //
    // 本地视频预览 (只预览AliRtcVideoTrackCamera，不预览AliRtcVideoTrackScreen)
    //

    /**
     * @brief 为本地预览设置窗口以及绘制参数
     * @param viewConfig 包含了窗口以及渲染方式
     * @param track      must be AliRtcVideoTrackCamera
     * @note 支持joinChannel之前和之后切换窗口。如果viewConfig或者viewConfig中的view为NULL，则停止渲染
     *       如果在播放过程中需要重新设置render mode，请保持canvas中其他成员变量不变，仅修改
     *       renderMode
     *       如果在播放过程中需要重新设置mirror mode，请保持canvas中其他成员变量不变，仅修改
     *       mirrorMode
     */
	virtual int setLocalViewConfig(const AliVideoCanvas &canvas, AliRtcVideoTrack track) = 0;

    /**
     * @brief 开始本地预览
     * @note 如果没有设置view，则无法预览。可以在joinChannel之前就开启预览
     *       会自动打开摄像头
     */
    virtual int startPreview() = 0;

    /**
     * @brief 停止本地预览
     * @note leaveChannel会自动停止本地预览
     *       会自动关闭摄像头 (如果正在publish camera流，则不会关闭摄像头)
     */
	virtual int stopPreview() = 0;

    /**
     * @brief 是否将停止本地视频采集
     * @param mute     true表示停止视频采集；false表示恢复正常
     * @param track    需要停止采集的track
     * @note 发送黑色的视频帧。本地预览也呈现黑色。采集，编码，发送模块仍然工作，
     *       只是视频内容是黑色帧
     */
	virtual int muteLocalCamera(bool mute, AliRtcVideoTrack track) = 0;

    //
    // 推流 (音频/视频)
    //

    /**
     * @brief 手动推送视频和音频流
     * @param onResult 当publish执行结束后调用此回调返回结果
     * @note 需要推送的流通过API configLocalAudioPublish, configLocalCameraPublish和configLocalScreenPublish进行设置
     *       如果需要手工配置所有推流和拉流，请通过setAutoPublishSubscribe接口关闭autoPub
     *       推流结果异步返回，执行结果会通过onResult中返回，同时也会通过onPublishChangedNotify回调返回，调用方可更根据情况接收处理推流结果
     *       返回结果可能在caller线程调用，也可能在SDK线程调用，需要停止推送所有流，配置所有推流流关闭并调用publish实现
     *       onResult是app设置的回调函数，可以为NULL，参数opaquePtr是app设置的透明参数，用于在回调中返回
     *       lambda表达式转换成onResult示例:
     * @code void (*foo)(void *, int);
     *       foo = [](void *opaquePtr, , int errCode) {
     *           ClassA *pThis = (ClassA *)opaquePtr;
     *           pThis->OutputError("publish result: %d", errCode);
     *       };
     *       publish(foo, this);
     * @end code
     */
	virtual void publish(void (*onResult)(void *opaquePtr, int errCode),
                 void *opaquePtr) = 0;

    //
    // 编码
    //

    /**
    * @brief 设置视频track的profile
    * @param profile     预定义的视频分辨率和帧率，详见AliRtcVideoProfile
    * @param track       视频流的类型，详见AliRtcVideoTrack
    * @note 设置之后等到下次publish的时候才能生效
    */
	virtual void setVideoProfile(AliRtcVideoProfile profile, AliRtcVideoTrack track) = 0;

    /**
     * @brief 获取视频track的profile
     * @param track       视频流的类型，详见AliRtcVideoTrack
     * @return 返回的是正在使用的（已经推流中）或者即将被使用的（下一次推流才会生效）视频分辨率和帧率，详见AliRtcVideoProfile
     * @note 返回值不一定是正在使用的video profile。另外AliRtcVideoTrackScreen是不支持的
     *
     */
    virtual AliRtcVideoProfile getVideoProfile(AliRtcVideoTrack track) = 0;

    //
    // 话筒
    //

    /**
     * @brief mute/unmute本地音频采集
     * @param mute  true表示本地音频采集空帧；false表示恢复正常
     * @note mute是指采集和发送静音帧。采集和编码模块仍然在工作
     * @return 0表示成功放入队列，-1表示被拒绝
     */
	virtual int muteLocalMic(bool mute, AliRtcMuteLocalAudioMode mode = AliRtcMuteLocalAudioModeDefault) = 0;

    //
    // 扬声器
    //

    //#pragma mark - "操作来自远端的媒体"

    /**
     * @brief 为远端的视频设置窗口以及绘制参数
     * @param canvas canvas包含了窗口以及渲染方式
     * @param uid    User ID。从App server分配的唯一标示符
     * @param track  需要设置的track，可选相机流，屏幕分享流，见AliRtcVideoTrack枚举类型
     * @note 支持joinChannel之前和之后切换窗口。如果canvas为NULL或者view为NULL，则停止渲染相应的流
     *       如果在播放过程中需要重新设置render mode，请保持canvas中其他成员变量不变，仅修改
     *       renderMode
     *       当前版本不支持远端的视频渲染mirror
     */
	virtual int setRemoteViewConfig(AliVideoCanvas *canvas, const AliRtc::String &uid, AliRtcVideoTrack track) = 0;

    /**
     * @brief mute/unmute远端的音频track的播放
     * @param uid     User ID。从App server分配的唯一标示符
     * @param mute    true表示停止播放；false表示恢复播放
     * @note 拉流和解码不受影响。支持joinChannel之前和之后设置
     */
	virtual int muteRemoteAudioPlaying(const AliRtc::String & uid, bool mute) = 0;

	/**
	* @brief mute/unmute远端的所有音频track的播放
	* @param mute    true表示停止播放；false表示恢复播放
	* @note 拉流和解码不受影响。支持joinChannel之前和之后设置
	*/
	virtual int muteAllRemoteAudioPlaying(bool mute) = 0;

    /**
     * @brief 手动拉视频和音频流
     * @param uid        从App server分配的唯一标示符。不允许为NULL
     * @param onResult   当subscribe执行结束后调用这个回调
     * @param opaquePtr  传递给onResult的用户数据
     * @note 如果需要手动选择拉取的流，调用configRemoteAudio, configRemoteCameraTrack,
     *       configRemoteScreenTrack来设置。缺省是拉取audio和camera track
     *       如果需要unsub所有的流，先通过configRemoteAudio, configRemoteCameraTrack,
     *       configRemoteScreenTrack来清除设置，然后调用subscribe
     *
     *       异步接口。是否成功subscribe，通过onResult判断
     *       onResult: app设置的回调函数，可以为NULL。第一个参数opaquePtr
     *       是app设置的值
     *       lambda表达式转换成onResult:
     *       void (*foo)(void *, const AliRtc::String &, AliRtcAudioTrack, AliRtcVideoTrack);
     *       foo = [](void *opaquePtr, const AliRtc::String &uid, AliRtcAudioTrack publishedAudioTrack, AliRtcVideoTrack publishedVideoTrack) {
     *           ClassA *pThis = (ClassA *)opaquePtr;
     *           pThis->OutputError("subscribe result: user: %s, audio: %d, video: %d",
     *                              uid.asCString(),
     *                              publishedAudioTrack,
     *                              publishedVideoTrack);
     *       };
     *       publish(foo, this);
     */
	virtual void subscribe(const AliRtc::String &uid,
                   void (*onResult)(void *opaquePtr, const AliRtc::String &uid, AliRtcVideoTrack vt, AliRtcAudioTrack at),
                   void *opaquePtr) = 0;

    //#pragma mark - "用户管理"

    /**
    * @brief 设置是否允许推送camera track
    * @param enable   是否允许
    * @note 需要调用publish才能生效
    *       缺省允许camera track推流
    */
	virtual void configLocalCameraPublish(bool enable) = 0;

    /**
     * @brief 查询是否允许推送camera track
     * @return true: 允许；false: 禁止
     */
	virtual bool isLocalCameraPublishEnabled() = 0;

    /**
     * @brief 设置是否允许推送screen track
     * @param enable   是否允许
    * @note 需要调用publish才能生效
     *       缺省不允许screen track推流
     */
	virtual void configLocalScreenPublish(bool enable) = 0;

    /**
     * @brief 查询是否允许推送screen track
     * @return true: 允许；false: 禁止
     */
	virtual bool isLocalScreenPublishEnabled() = 0;

    /**
    * @brief 设置是否允许推送audio
    * @param enable   是否允许
    * @note 需要调用publish才能生效
    *       缺省允许音频推流
    */
	virtual void configLocalAudioPublish(bool enable) = 0;

    /**
     * @brief 查询是否允许推送audio track
     * @return true: 允许；false: 禁止
     */
	virtual bool isLocalAudioPublishEnabled() = 0;

    /**
    * @brief 是否允许推送次要视频流
    * @param enabled    true表示需要推送次流；false表示不推送次流
    * @param track      must be AliRtcVideoTrackCamera
    * @note 需要调用publish才能生效
    *。     缺省enable。目前只支持camera track，不支持screen track
    */
	virtual int configLocalSimulcast(bool enabled, AliRtcVideoTrack track) = 0;

    /**
     * @brief 查询是否允许推送simulcast (camera track)
     * @return true: 允许；false: 禁止
     */
	virtual bool isLocalSimulcastEnabled() = 0;


    /**
     * @brief 设置是否拉取音频流
     * @param uid     User ID。从App server分配的唯一标示符。如果设置为NULL，则表示
     *                对当前已知的用户进行通配
     * @param enable  true: 拉取; false: 不拉取
     * @note 可以在joinChannel之前或者之后设置。需要调用subscribe才生效
     */
	virtual void configRemoteAudio(const AliRtc::String &uid, bool enable) = 0;

    /**
     * @brief 设置是否拉取camera视频流
     * @param uid     User ID。从App server分配的唯一标示符。如果设置为NULL，则表示
     *                对当前已知的用户进行通配
     * @param master  是否优先拉取大流
     * @param enable  true: 拉取; false: 不拉取
     * @note 可以在joinChannel之前或者之后设置。需要调用subscribe才生效
     */
	virtual void configRemoteCameraTrack(const AliRtc::String &uid, bool preferMaster, bool enable) = 0;

    /**
     * @brief 设置是否拉取screen视频流
     * @param uid     User ID。从App server分配的唯一标示符。如果设置为NULL，则表示
     *                对当前已知的用户进行通配
     * @param enable  true: 拉取; false: 不拉取
     * @note 可以在joinChannel之前或者之后设置。需要调用subscribe才生效
     */
	virtual void configRemoteScreenTrack(const AliRtc::String &uid, bool enable) = 0;

    /**
     * @brief 获取远端在线用户列表
     * @param array  用户列表，保存的是用户ID
     */
	virtual void getOnlineRemoteUsers(AliRtc::StringArray &array) = 0;

    /**
     * @brief 判断用户是否在线
     * @param uid     User ID。从App server分配的唯一标示符。不可为NULL
     * @return true表示在线，false表示不在线
     */
	virtual bool isUserOnline(const AliRtc::String &uid) = 0;

    //#pragma mark - "设备管理"

    /**
     * @brief 获取系统中的录音设备列表
     * @param array  capturer列表
     */
	virtual void getAudioCaptures(AliRtcDeviceList &deviceList) = 0;

    /**
     * @brief 获取使用的录音设备名称
     */
	virtual AliRtc::String getCurrentAudioCapture() = 0;

    /**
     * @brief 获取使用的录音设备id
     */
	virtual AliRtc::String getCurrentAudioCaptureId() = 0;

    /**
     * @brief 通过设备名称选择录音设备
     * @param capture 录音设备名称，从getAudioCaptures接口获取
     */
	virtual void setCurrentAudioCapture(const AliRtc::String &capture) = 0;

      /**
     * @brief 通过设备id选择录音设备
     * @param capture 录音设备id，从getAudioCaptures接口获取
     */
	virtual void setCurrentAudioCaptureById(const AliRtc::String &captureId) = 0;

    /**
     * @brief 获取系统中的扬声器列表
     * @param array  audio renderer列表
     */
	virtual void getAudioRenderers(AliRtcDeviceList &deviceList) = 0;

    /**
     * @brief 获取当前使用的扬声器名称
     */
	virtual AliRtc::String getCurrentAudioRenderer() = 0;

    /**
     * @brief 获取当前使用的扬声器Id
     */
	virtual AliRtc::String getCurrentAudioRendererId() = 0;

    /**
     * @brief 通过设备名称选择扬声器
     * @param renderer 扬声器名称，从getAudioRenderers接口获取
     */
	virtual void setCurrentAudioRenderer(const AliRtc::String &renderer) = 0;

    /**
     * @brief 通过设备Id选择扬声器
     * @param renderer 扬声器id，从getAudioRenderers接口获取
     */
	virtual void setCurrentAudioRendererById(const AliRtc::String &rendererId) = 0;

  /**
  * @brief 创建音视频测试对象
  */
  virtual AliMediaDeviceTestInterface * createMediaDeviceTestInterface(AliMediaDeviceTestEventListener * pMediaDeviceEventListener) = 0;

    /**
     * @brief 获取摄像头列表
     * @param array  camera列表
     */
	virtual void getCameraList(AliRtcDeviceList &deviceList) = 0;

    /**
     * @brief 获取当前使用的摄像头名称
     * @return 摄像头名称
     */
	virtual AliRtc::String getCurrentCamera() = 0;

      /**
     * @brief 获取当前使用的摄像头id
     * @return 摄像头id
     */
	virtual AliRtc::String getCurrentCamerId() = 0;

    /**
     * @brief 通过设备名称选择摄像头
     * @param camera   摄像头名称
     */
	virtual void setCurrentCamera(const AliRtc::String &camera) = 0;

      /**
     * @brief 通过设备id选择摄像头
     * @param camera   摄像头id
     */
	virtual void setCurrentCameraById(const AliRtc::String &cameraId) = 0;

    /**
    * @brief 开启音频采集
    * @note 此接口可以控制提前打开音频采集，如果不设置，则SDK会在合适的时机在打开音频采集
    */
    virtual void startAudioCapture() = 0;

    /**
    * @brief 关闭音频采集
    * @note 此接口可以控制关闭音频采集，与startAudioCapture对应
    */
    virtual void stopAudioCapture() = 0;

    /**
    * @brief 开启音频播放设备
    * @note 此接口可以控制提前打开音频播放，如果不设置，则SDK会在合适的时机在打开音频播放
    */
    virtual void startAudioPlayer() = 0;

    /**
    * @brief 关闭音频播放
    * @note 此接口可以控制关闭音频播放，与stopAudioPlayer对应
    */
    virtual void stopAudioPlayer() = 0;

    //#pragma mark - "其他"

    /**
     * @brief 设置log
     * @param logLevel  Log级别，详见AliRtcLogLevel枚举类型
     */
	virtual void setLogLevel(AliRtcLogLevel logLevel) = 0;

    /**
     * @brief 检查camera是否打开
     * @return true表示摄像头已打开；false表示摄像头没有打开
     */
	virtual bool isCameraOn() = 0;

    /**
     * @brief 检查当前是否在频道中
     * @return true表示加入了频道；false表示不在频道中
     */
	virtual bool isInCall() = 0;

    /**
     * @brief 查询是否自动publish
     */
	virtual bool isAutoPublish() = 0;

    /**
     * @brief 查询是否自动subscribe
     */
	virtual bool isAutoSubscribe() = 0;

    /**
     * @brief 查询是否audio only
     */
	virtual bool isAudioOnly() = 0;

    /**
     * @brief 查询远端用户的各种状态
     * @param uid        User ID。从App server分配的唯一标示符
     * @param dict       App提供的容器，用于存放用户数据
     * @return 0表示成功获取；非0表示用户不存在
     * @note Key: userID, isOnline, sessionID, callID, displayName
     *            hasAudio, hasCameraMaster, hasCameraSlave, hasScreenSharing
     *            requestAudio, requestCameraMaster, requestCameraSlave, requestScreenSharing, preferCameraMaster
     *            subScribedAudio, subScribedCameraMaster, subScribedCamearSlave, subScribedScreenSharing
     *            hasCameraView, hasScreenView
     *            muteAudioPlaying
     */
	virtual int getUserInfo(const AliRtc::String &uid, AliRtc::Dictionary &dict) = 0;

    /**
     * @brief 设置H5兼容模式
     * @param comp    true: 兼容H5; false: 传统模式，不兼容H5
     * @note 只支持在sdk实例创建之前设置。如果在sdk创建之后改这个设置会引起互通问题。缺省不兼容H5
     */
    static void setH5CompatibleMode(bool comp);

    /**
     * @brief 检查当前是否兼容H5
     * @return YES: 兼容H5; NO: 不兼容H5
     */
    static bool getH5CompatibleMode();

    /**
    * @brief 设置录音音量
    * @param volume  [0 400],0:静音，>100:放大音量，<100:减小音量
    * @return 0：成功  其他：失败
    */
    virtual int setRecordingVolume(int volume) = 0;

    /**
    * @brief 设置播放音量
    * @param volume  [0 400],0:静音，>100:放大音量，<100:减小音量
    * @return 0：成功  其他：失败
    */
    virtual int setPlayoutVolume(int volume) = 0;

    /**
    * @brief 设置混响音效模式
    * @param type 类型
    * @return 0：成功  其他：失败
    */
    virtual int setAudioEffectReverbMode(const AliRtcAudioEffectReverbMode mode) = 0;

    /**
    * @brief 设置混响音效类型和具体参数
    * @param type 类型; value 具体参数值
    * @return 0：成功  其他：失败
    */
    virtual int setAudioEffectReverbParamType(const AliRtcAudioEffectReverbParamType type, float value) = 0;

    /**
    * 设置音量回调频率和平滑系数
    *
    * @param interval 时间间隔，单位毫秒，最小值不得小于10ms
    * @param smooth 平滑系数，数值越大平滑程度越高，反之越低，实时性越好，建议设置3，范围[0, 9];
    * @param reportVad 本地语音检测开关，1：开启，通过onAudioVolumeCallback接口回调, 0:关闭
    * @return 0 表示成功，-1 表示interval设置小于10，-2 表示平滑系数超出范围.
    */
    virtual int setVolumeCallbackIntervalMs(int interval, int smooth, int reportVad) = 0;

    /**
    * @brief 设置输出音频声道数，默认单声道
    * @param audioNumChannel 声道数
    */
    virtual void setSubscribeAudioNumChannel(AliRtcAudioNumChannelType audioNumChannel) = 0;

    /**
    * @brief 设置输出音频采样率，默认44.1k
    * @param audioSampleRate 采样率
    */
    virtual void setSubscribeAudioSampleRate(AliRtcAudioSampleRate audioSampleRate) = 0;

    /**
    * @brief 订阅音频数据
    * @param audioSource 音频数据源
    * @return
    * @note 订阅音频数据输出前，需先通过setSubscribeAudioNumChannel与setSubscribeAudioSampleRate设置输出音频数据参数
    */
    virtual void subscribeAudioData(AliRtcAudioSource audioSource) = 0;

    /**
    * @brief 取消订阅音频数据
    * @param audioSource 音频数据源
    * @return
    */
    virtual void unsubscribeAudioData(AliRtcAudioSource audioSource) = 0;

    virtual AliMediaInputStreamInterface* GetMediaInputStreamInterface() = 0;

    /**
    *  @brief 配置是否启用外部渲染
    *  @param enable 是否允许
    *  @return 成功返回0，失败返回错误码
    *  @note 启用外部渲染后，可通过getVideoRenderData接口或onVideoRenderData回调获取视频数据进行外部渲染
    */
    virtual bool configExternalVideoRendering(bool enable) = 0;

    /**
    *  @brief 获取用户视频渲染数据
    *  @param[in] uid 视频数据对应用户id
    *  @param[in] track 视频数据流类型
    *  @param[out] data 视频数据指针
    *  @param[out] width 视频数据宽度
    *  @param[out] height 视频数据高度
    *  @return 成功返回0，失败返回错误码
    *  @note configExternalVideoRendering接口配置打开后才能打开此接口获取用户视频数据，视频数据格式为ARGB
    */
    virtual bool getVideoRenderData(const char* uid, AliRtcVideoTrack track, unsigned char* data, int& width, int& height) = 0;

    /**
    * @brief 获取屏幕分享源信息
    * @param[in] source_type 屏幕分享类型
    * @param[out] source_list 屏幕分享数据源信息
    * @return 0表示成功获取；非0表示失败
    */
    virtual int getScreenShareSourceInfo(AliRtcScreenShareType source_type, AliRtcScreenSourceList &source_list) = 0;

    /**
    * @brief 获取屏幕分享桌面分辨率
    * @param[in] source_id    屏幕分享数据源id
    * @param[in] source_title 屏幕分享数据源名称
    * @param[out] width       屏幕分辨率宽
    * @param[out] height      屏幕分辨率高
    * @return 0为成功，非0失败
    * @remark 只针对ScreenShare_Desktop
    */
    virtual int getDesktopResolution(const AliRtc::String& source_id, const AliRtc::String& source_title, int& width, int& height) = 0;

    /**
    * @brief 设置屏幕分享源
    * @param[in] source 屏幕分享数据源id
    * @return 0为成功，非0失败
    * @note 屏幕按区域分享时，分享区域最小分辨率为16 x 16，设置区域小于最小分辨率时重置为最小分辨率
    * @note 屏幕按区域分享时，设置分享区域超过实际桌面分辨率时，将按住完整桌面分辨率分享
    */
    virtual int setScreenShareSource(const AliRtcScreenSource& source) = 0;

    /**
    * @brief 获取屏幕分享源
    * @param[in] source 屏幕分享数据源id
    * @return 当前设置屏幕分享源
    */
    virtual AliRtcScreenSource getScreenShareSource() = 0;
    
    /**
     @brief 开始录制
     @param recordType 录制类型
     @param recordFormat 录制文件格式
     @param filePath 文件路径
     @param audioConfig 音频设置
     @param videoConfig 视频设置
     @return 成功/失败
     */
    virtual bool startRecord(AliRtcRecordType recordType, AliRtcRecordFormat recordFormat, const AliRtc::String& filePath, AliRtcRecordAudioConfig& audioConfig, AliRtcRecordVideoConfig& videoConfig) = 0;

    /**
     @brief 停止录制
     */
    virtual void stopRecord() = 0;

    /**
    * @brief 开始最后一公里网络质量探测
    * @return 0表示成功， 非0表示失败
    * @note 请在joinChannel之前调用，探测结果在onLastmileDetect回调
    */
    virtual int startLastmileDetect() = 0;

    /**
    * @brief 停止最后一公里网络质量探测
    */
    virtual int stopLastmileDetect() = 0;
    
    /**
    * @brief 启用外部视频输入源
    * @param enable YES 开启，NO 关闭
    * @param useTexture 是否使用texture 模式
    * @param type 流类型
    * @note 启用后使用pushExternalVideoFrame接口输入视频数据
    */
    virtual int setExternalVideoSource(bool enable, bool useTexture, AliRtcVideoSource sourceType) = 0;

    /**
    * @brief 输入外部输视频
    * @param frame 帧数据
    * @param type 流类型
    * @param 目前输入视频类型只支持I420
    */
    virtual int pushExternalVideoFrame(AliRtcVideoDataSample *frame, AliRtcVideoSource sourceType) = 0;

    /**
    * @brief 订阅视频数据输出
    * @note 输出数据将通过onCaptureVideoSample及onRemoteVideoSample回调返回
    */
    virtual void registerVideoSampleObserver() = 0;

    /**
    * @brief 取消订阅视频数据输出
    */
    virtual void unRegisterVideoSampleObserver() = 0;

    /**
    * @brief 设置是否启用外部音频输入推流
    * @param enable YES 开启，NO 关闭
    * @param sampleRate 采样率 16k 48k...
    * @param channelsPerFrame 采样率 16k 48k...
    * @return >=0表示成功， <0表示失败
    * @note 可通过setExternalAudioPublishVolume设置输入音频推流音量
    */
    virtual int setExternalAudioSource(bool enable, unsigned int sampleRate,  unsigned int channelsPerFrame) = 0;

    /**
    * @brief 输入外部音频数据推流
    * @param audioSamples 音频数据
    * @param sampleLength 音频数据长度
    * @param timestamp 时间戳
    * @return <0表示失败，返回值为ERR_AUDIO_BUFFER_FULL时，需要在间隔投递数据时间长度后再次重试投递
    */
    virtual int pushExternalAudioFrameRawData(const void* audioSamples, unsigned int sampleLength, long long timestamp) = 0;

    /**
    * @brief 设置外部输入音频推流是否与麦克风采集音频混合
    * @param mixed YES 混音，NO 完全替换麦克风采集数据
    */
    virtual int setMixedWithMic(bool mixed) = 0;

    /**
    * @brief 设置外部输入音频推流混音音量
    * @param vol 音量 0-100
    */
    virtual int setExternalAudioPublishVolume(int volume) = 0;

    /**
    * @brief 获取外部输入音频推流混音音量
    * @return vol 音量
    */
    virtual int getExternalAudioPublishVolume() = 0;

    /**
    * @brief 设置是否启用外部输入音频播放
    * @param enable YES 开启，NO 关闭
    * @param sampleRate 采样率 16k 48k...
    * @param channelsPerFrame 采样率 16k 48k...
    * @return >=0表示成功， <0表示失败
    * @note 可通过接口setExternalAudioPlayoutVolume设置输入音频播放音量
    */
    virtual int setExteranlAudioRender(bool enable, unsigned int sampleRate, unsigned int channelsPerFrame) = 0;

    /**
    * @brief 输入外部音频播放数据
    * @param audioSamples 音频数据
    * @param sampleLength 音频数据长度
    * @param sampleRate 音频采样率
    * @param channelsPerFrame 音频声道数
    * @param timestamp 时间戳
    * @return <0表示失败
    */
    virtual int pushExternalAudioRenderRawData(const void* audioSamples, unsigned int sampleLength, unsigned int sampleRate, unsigned int channelsPerFrame, long long timestamp) = 0;

    /**
    * @brief 设置外部音频播放音量
    * @param vol 音量 0-100
    */
    virtual int setExternalAudioPlayoutVolume(int volume) = 0;

    /**
    * @brief 获取音频播放音量
    * @return vol 音量
    */
    virtual int getExternalAudioPlayoutVolume() = 0;

    /**
    * 开始直播拉流
    * @param authInfo 入会鉴权信息
    * @param onResult 拉流结果 errCode=0为Success，其他为Failure
    */
    virtual void startLiveStreaming(const AliRtcAuthInfo &authInfo,
      void(*onResult)(void *opaquePtr, int errCode), 
      void *opaquePtr) = 0;

    /**
    * 停止直播拉流
    * @note 停止直播拉流后如需再次进入，需要先destroy销毁SDK，重新创建后再次进入
    */
    virtual int stopLiveStreaming() = 0;

    /**
    * 设置直播拉流窗口及渲染参数
    * @param canvas canvas包含了窗口以及渲染方式
    */
    virtual int setLiveStreamingViewConfig(AliVideoCanvas *canvas) = 0;

protected:
    // 不允许直接创建和销毁AliRtcEngine instance
    // 使用sharedInstance和destroy
    AliRtcEngine() { _delegate = nullptr; }
    virtual ~AliRtcEngine() {}

	AliRtcEventListener *_delegate;
};

int ALI_RTC_API GetAliRtcStatusInfo(int nType, void * pData);

#endif // ALIRTCENGINE_H
