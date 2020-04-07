// AliRtcCSharp.h : AliRtcCSharp DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "AliRtcEngine.h"
#include <string>
#include <functional>
#include <atomic>
#include <vector>


#ifdef _EXPORTING 
#define API_DECLSPEC extern "C" _declspec(dllexport) 
#else 
#define API_DECLSPEC  extern "C" _declspec(dllimport) 
#endif


// CAliRtcCSharpApp
// 有关此类实现的信息，请参阅 AliRtcCSharp.cpp
//

using namespace std;

class CAliRtcCSharpApp : public CWinApp
{
public:
	CAliRtcCSharpApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/*                                         RTC  监 听 类                                       */
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
class AliRtcEngineListener :public AliRtcEventListener
{
    virtual void onRemoteUserOnLineNotify(const AliRtc::String &uid) override;                  /// 入会通知(Remote user entry notification)
    virtual void onRemoteUserOffLineNotify(const AliRtc::String &uid) override;                 /// 离会通知(Remote user departure notification)
    virtual void onRemoteTrackAvailableNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) override;                     /// 远端用户发布或停止发布(Distant user publishes or stops publishing)
    virtual void onSubscribeChangedNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) override;                     /// 当订阅情况发生变化时(When subscriptions change)
    virtual void onBye(int code) override;                                                      /// 被服务器踢出或者频道关闭时回调
    virtual void onOccurWarning(int warn) override;                                             /// warning回调
    virtual void onOccurError(int error) override;                                              /// error回调
};

// 回调函数声明
typedef void(*JoinResultCallBack)(int);
typedef void(*RemoteUserOnLineNotifyCallBack)(char*);
typedef void(*RemoteUserOffLineNotifyCallBack)(char*);
typedef void(*RemoteTrackAvailableNotifyCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*SubscribeChangedNotifyCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*OccurErrorCallBack)(int);
typedef void(*OccurWarningCallBack)(int);
typedef void(*ByeCallBack)(int);
typedef void(*SubscribeResultCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*PublishResultCallBack)(int);

// 调用函数
AliRtc::String CStringToAliString(CString &s);

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/*                                        接  口  函  数                                       */
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
extern "C" __declspec(dllexport) bool initAliSdk();                                             /// 1.初始化sdk
extern "C" __declspec(dllexport) void releaseAliSdk();                                          /// 2.释放sdk
extern "C" __declspec(dllexport) void exitChannel();                                            /// 3.退出房间
extern "C" __declspec(dllexport) bool joinChannel(char *pChannel, char *pAppId, char *pUserId,
    char *pNonce, char *pToken, char *pGlsb, char *pUserName, long iTimesTamp);                 /// 4.加入房间
extern "C" __declspec(dllexport) int setLocalViewConfig(HWND hWndControlID);                    /// 5.为本地预览设置窗口以及绘制参数
extern "C" __declspec(dllexport) int startPreview();                                            /// 6.开启本地浏览
extern "C" __declspec(dllexport) int stopPreview();                                             /// 7.停止本地预览
extern "C" __declspec(dllexport) void setLogLevel(AliRtcLogLevel logLevel);                     /// 8.设置日志等级
extern "C" __declspec(dllexport) int setAutoPublishSubscribe(bool bAutoPub, bool bAutoSub);     /// 9.设置是否自动publish音视频流和subscribe音视频流
extern "C" __declspec(dllexport) void configLocalAudioPublish(bool bEnable);                    /// 10.设置是否允许推送audio
extern "C" __declspec(dllexport) void configLocalCameraPublish(bool bEnable);                   /// 11.设置是否允许推送camera track
extern "C" __declspec(dllexport) int configLocalSimulcast(bool bEnabled,AliRtcVideoTrack track);/// 12.是否允许推送次要视频流
extern "C" __declspec(dllexport) bool configRemoteAudio(char *pUid, bool bEnable);              /// 13.设置是否订阅音频流
extern "C" __declspec(dllexport) bool configRemoteCameraTrack(char *pUid, bool bPreferMaster,
    bool bEnable);                                                                              /// 14.设置是否订阅视频流
extern "C" __declspec(dllexport) bool configRemoteScreenTrack(char *pUid, bool bEnable);        /// 15.设置是否订阅共享流
extern "C" __declspec(dllexport) bool subscribe(char *pUid);                                    /// 16.手动订阅视频流音频流共享流
extern "C" __declspec(dllexport) bool setRemoteViewConfig(HWND hWndShowID, char *pUid, AliRtcVideoTrack track,
    AliRtcRenderMode renderMode, AliRtcScaleRatio scaleRatio, bool bIsfFlip);                   /// 17.为远端的视频设置窗口以及绘制参数
extern "C" __declspec(dllexport) int setAudioOnlyMode(bool bAudioOnly);                         /// 18.设置音频模式还是音视频模式
extern "C" __declspec(dllexport) int muteLocalCamera(bool bMute, AliRtcVideoTrack track);       /// 19.是否将停止本地视频采集
extern "C" __declspec(dllexport) void configLocalScreenPublish(bool bEnable);                   /// 20.是否启用本地屏幕共享
extern "C" __declspec(dllexport) bool publish();                                                /// 21.手动推送视频和音频流共享流
extern "C" __declspec(dllexport) void setVideoProfile(AliRtcVideoProfile profile, 
    AliRtcVideoTrack track);                                                                    /// 22.设置视频track的profile
extern "C" __declspec(dllexport) void setH5CompatibleMode(bool bEnable);                        /// 23.设置H5兼容模式
extern "C" __declspec(dllexport) int muteLocalMic(bool bEnable);                                /// 24.本地音频采集（本地静音）
extern "C" __declspec(dllexport) int muteRemoteAudioPlaying(char *pUid, bool bEnable);          /// 25.远端音频采集（远端静音）

// 接口回调
extern "C" __declspec(dllexport) void CALLBACK setJoinResultCallBack(JoinResultCallBack jrcb);                                         /// 1.入会回调
extern "C" __declspec(dllexport) void CALLBACK setRemoteUserOnLineNotifyCallBack(RemoteUserOnLineNotifyCallBack ruoncb);               /// 2.远端用户入会回调
extern "C" __declspec(dllexport) void CALLBACK setRemoteUserOffLineNotifyCallBack(RemoteUserOffLineNotifyCallBack ruoncb);             /// 3.远端用户离会回调
extern "C" __declspec(dllexport) void CALLBACK setRemoteTrackAvailableNotifyCallBack(RemoteTrackAvailableNotifyCallBack rtancb);       /// 4.远端用户发布或停止发布回调
extern "C" __declspec(dllexport) void CALLBACK setSubscribeChangedNotifyCallBack(SubscribeChangedNotifyCallBack scncb);                /// 5.订阅流改变回调
extern "C" __declspec(dllexport) void CALLBACK setOccurErrorCallBack(OccurErrorCallBack oecb);                                         /// 6.Error回调
extern "C" __declspec(dllexport) void CALLBACK setOccurWarningCallBack(OccurWarningCallBack owcb);                                     /// 7.Warning回调
extern "C" __declspec(dllexport) void CALLBACK setByeCallBack(ByeCallBack bcb);                                                        /// 8.被服务器踢出或者频道关闭时回调
extern "C" __declspec(dllexport) void CALLBACK setSubscribeResultCallBack(SubscribeResultCallBack srcb);                               /// 9.手动订阅回调
extern "C" __declspec(dllexport) void CALLBACK setPublishResultCallBack(PublishResultCallBack prcb);                                   /// 10.手动推流回调
