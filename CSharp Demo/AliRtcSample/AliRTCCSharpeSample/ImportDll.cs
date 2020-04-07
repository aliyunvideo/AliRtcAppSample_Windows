using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace AliRtcSample
{
    class ImportDll
    {
        // 回调函数
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setJoinResultCB(int iErrorCode);
        public static setJoinResultCB joinResultCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setRemoteUserOnLineNotifyCB(string strUid);
        public static setRemoteUserOnLineNotifyCB RemoteUserOnLineNotifyCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setSubscribeChangedNotifyCB(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack);
        public static setSubscribeChangedNotifyCB SubscribeChangedNotifyCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setRemoteUserOffLineNotifyCB(string strUid);
        public static setRemoteUserOffLineNotifyCB RemoteUserOffLineNotifyCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setSubscribeResultCB(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setPubResultCB(int errCode);


        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setRemoteTrackAvailableNotifyCB(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack);
        public static setRemoteTrackAvailableNotifyCB RemoteTrackAvailableNotifyCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setByeCB(int code);
        public static setByeCB ByeCB;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void setOccurErrorCB(int error);
        public static setOccurErrorCB OccurErrorCB;


        // 接口函数
        [DllImport("AliRTCEngineCWrapper.dll")] // 初始化sdk
        public static extern bool initAliSdk();

        [DllImport("AliRTCEngineCWrapper.dll")] // 释放sdk
        public static extern void releaseAliSdk();

        [DllImport("AliRTCEngineCWrapper.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)] // 为本地预览设置窗口以及绘制参数
        public static extern int setLocalViewConfig(IntPtr iControlID);

        [DllImport("AliRTCEngineCWrapper.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public static extern int startPreview();

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 加入房间回调
        public static extern void setJoinResultCallBack(setJoinResultCB jrcb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 加入房间
        public static extern bool joinChannel(String pChannel, String pAppId, String pUserId, String pNonce, String pToken, String pGlsb, String pUserName, long iTimesTamp);
        
        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置日志等级
        public static extern void setLogLevel(AliRtcLogLevel logLevel);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置H5兼容模式
        public static extern void setH5CompatibleMode(bool bEnablel);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置是否自动publish音视频流和subscribe音视频流
        public static extern int setAutoPublishSubscribe(bool bAutoPub, bool bAutoSub);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置是否允许推送audio
        public static extern void configLocalAudioPublish(bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置是否允许推送camera
        public static extern void configLocalCameraPublish(bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 设置是否允许推送screen
        public static extern void configLocalScreenPublish(bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 是否允许推送次要视频流
        public static extern int configLocalSimulcast(bool bEnabled, AliRtcVideoTrack track);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 远程入会通知回调
        public static extern void setRemoteUserOnLineNotifyCallBack(setRemoteUserOnLineNotifyCB ruoncb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 远程离会通知回调
        public static extern void setRemoteUserOffLineNotifyCallBack(setRemoteUserOffLineNotifyCB ruofncb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 订阅改变通知回调
        public static extern void setSubscribeChangedNotifyCallBack(setSubscribeChangedNotifyCB scncb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 订阅音频流
        public static extern bool configRemoteAudio(String pUid, bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 订阅视频流
        public static extern bool configRemoteCameraTrack(String pUid, bool bPreferMaster, bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 订阅共享流
        public static extern void configRemoteScreenTrack(String pUid, bool bEnable);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 开始订阅
        public static extern bool subscribe(String pUid);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 开始订阅
        public static extern bool publish();

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 订阅结果回调
        public static extern void setSubscribeResultCallBack(setSubscribeResultCB srcb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 推流结果回调
        public static extern void setPublishResultCallBack(setPubResultCB prcb);
        
        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] // 远端流改变回调
        public static extern void setRemoteTrackAvailableNotifyCallBack(setRemoteTrackAvailableNotifyCB srcb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void muteLocalCamera(bool bMute, AliRtcVideoTrack track);     //是否将停止本地视频采集

  [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.Cdecl)] // 订阅视频流
        public static extern void setRemoteViewConfig(IntPtr hWndShowID, String pUid, AliRtcVideoTrack videoTrack,
    AliRtcRenderMode RenderMode, AliRtcScaleRatio ScaleRatio, bool bIsfFlip);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)] //被服务器踢出频道的消息
        public static extern void setByeCallBack(setByeCB bcb);

        [DllImport("AliRTCEngineCWrapper.dll", CallingConvention = CallingConvention.StdCall)]  //error信息回调
        public static extern void setOccurErrorCallBack(setOccurErrorCB oecb);

    }
}
