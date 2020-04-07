#ifndef ALIRTCENGINE_H
#define ALIRTCENGINE_H

#include <Windows.h>
#include <stdint.h>
#include "AliRtcDefine.h"
#include "AliString.h"
#include "StringArray.h"
#include "Dictionary.h"


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
    AliRtcNetworkQualityGood   = 1,
    AliRtcNetworkQualityPoor   = 3,
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
    AliRtcVideoProfile_180_320P_15,
    AliRtcVideoProfile_180_320P_30,
    AliRtcVideoProfile_360_640P_15,
    AliRtcVideoProfile_360_640P_30,
    AliRtcVideoProfile_720_1280P_15,
    AliRtcVideoProfile_720_1280P_30,
} AliRtcVideoProfile;

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
  AliRtcOnByeChannelTerminated = 2,      //频道结束
}AliRtcOnByeType;

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
} AliRtcAuthInfo;


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
  virtual int StopTestAudioRecord() = 0;
  virtual int StartTestAudioPlayout(const char *  deviceName, int nTimeInv, const char *  wavFile) = 0;
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

	//virtual int AddVideoStream() = 0;
	//virtual int RemoveVideoStream(int stream_handle) = 0;
	//virtual int SendVideoData(int stream_handle, const VideoInputInfo *video_input_info) = 0;

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
     * @param quality 网络状况，详见AliRtcNetworkQuality
     * @note 当网络质量发生变化时触发
     */
    virtual void onNetworkQualityChanged(AliRtcNetworkQuality quality) {}

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
     * @note: 如果当前不在频道内，leaveChannel不会有任何影响。leaveChannel会产生消息通知频道内其他用户
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
     * @param onResult    当publish执行结束后调用这个回调
     * @note 需要推送的流通过API xxxx设置好
     *       如果需要手工配置所有推流和拉流，请关闭autoPub (见setAutoPublishSubscribe)
     *       这个是异步接口。结果通过result返回。返回可能在caller线程，也可能在其他线程调用
     *       如果想停止推送所有流，通过publish实现
     *       异步接口。是否成功publish，通过onResult判断
     *       onResult: app设置的回调函数，可以为NULL。第一个参数opaquePtr
     *       是app设置的值
     *       lambda表达式转换成onResult:
     * @code void (*foo)(void *, int);
     *       foo = [](void *opaquePtr, , int errCode) {
     *           ClassA *pThis = (ClassA *)opaquePtr;
     *           pThis->OutputError("publish result: %d", errCode);
     *       };
     *       publish(foo, this);
     * @endcode
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
    * @note 设置之后等到下次publish的时候才能生效。当前只支持AliRtcVideoTrackCamera
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
	virtual int muteLocalMic(bool mute) = 0;

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
	virtual void getAudioCaptures(AliRtc::StringArray &array) = 0;

    /**
     * @brief 获取使用的录音设备名称
     */
	virtual AliRtc::String getCurrentAudioCapture() = 0;

    /**
     * @brief 选择录音设备
     * @param capture 录音设备名称，从getAudioCaptures接口获取
     */
	virtual void setCurrentAudioCapture(const AliRtc::String &capture) = 0;

    /**
     * @brief 获取系统中的扬声器列表
     * @param array  audio renderer列表
     */
	virtual void getAudioRenderers(AliRtc::StringArray &array) = 0;

    /**
     * @brief 获取当前使用的扬声器名称
     */
	virtual AliRtc::String getCurrentAudioRenderer() = 0;

    /**
     * @brief 选择扬声器
     * @param renderer 扬声器名称，从getAudioRenderers接口获取
     */
	virtual void setCurrentAudioRenderer(const AliRtc::String &renderer) = 0;

  /**
  * @brief 创建音视频测试对象
  */
  virtual AliMediaDeviceTestInterface * createMediaDeviceTestInterface(AliMediaDeviceTestEventListener * pMediaDeviceEventListener) = 0;


    /**
     * @brief 获取摄像头列表
     * @param array  camera列表
     */
	virtual void getCameraList(AliRtc::StringArray &array) = 0;

    /**
     * @brief 获取当前使用的摄像头名称
     * @return 摄像头名称
     */
	virtual AliRtc::String getCurrentCamera() = 0;

    /**
     * @brief 选择摄像头
     * @param camera   摄像头名称
     */
    virtual void setCurrentCamera(const AliRtc::String &camera) = 0;

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
    * @brief 设置音频音量
    * @param audio_device  0：recorder  1：micphone  2：player 3：speaker
    * @param volume  音量值0~100
    * @return 0：成功  其他：失败
    */
    virtual int SetAudioVolume(int audio_device, int volume) = 0;

    /**
    * @brief 获取音量值
    * @param audio_device  0：recorder  1：micphone  2：player 3：speaker
    * @return 音量值0~100
    */
    virtual int GetAudioVolume(int audio_device) = 0;

    virtual AliMediaInputStreamInterface* GetMediaInputStreamInterface() = 0;

    /**
    * @brief 设置trace id
    * @param trace_id
    */
    virtual int setCustomID(const AliRtc::String& trace_id) = 0;
    
    /**
     *  @brief 获取拉流的数据
     *  @param uid  需要查询的userId
               track 需要查询的媒体流类型
               key_list 查询key值数组
               length key_list数组长度
     *  @return key-value json字符串
    */ 
    virtual AliRtc::String getMediaInfoWithKeys(const AliRtc::String& uid,
        AliRtcVideoTrack track,
        const AliRtc::String key_list[],
        int length) = 0;
protected:
    // 不允许直接创建和销毁AliRtcEngine instance
    // 使用sharedInstance和destroy
    AliRtcEngine() { _delegate = nullptr; }
    virtual ~AliRtcEngine() {}

	AliRtcEventListener *_delegate;
};

int ALI_RTC_API GetAliRtcStatusInfo(int nType, void * pData);

#endif // ALIRTCENGINE_H
