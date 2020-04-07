using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Collections;
using System.Diagnostics;

struct AuthInfo
{
    public string strAppId;
    public string strUserId;
    public string strNonce;
    public string strToken;
    public long   iTimesTamp;
    public string strGlsb;
    public string strUserName;
    public string strChannel;
}

public struct ParamInfo
{
    public string strAppSer;
    public string strChannel;
    public string strUserName;
    public bool bAutoPush;
    public bool bAutoSub;
    public bool bH5Mode;
    public bool bPushAudio;
    public bool bPushCamera;
    public bool bPushScreen;
}

public struct viewInfo
{
    public string strUid;
    public AliRtcVideoTrack videoTrack;
    public Panel showPanel;
}

/**
* @brief 日志级别
*/
public enum AliRtcLogLevel
{
    AliRtcLogLevelDump,
    AliRtcLogLevelDebug,
    AliRtcLogLevelVerbose,
    AliRtcLogLevelInfo,
    AliRtcLogLevelWarn,
    AliRtcLogLevelError,
    AliRtcLogLevelFatal,
    AliRtcLogLevelNone,
}

/**
* @brief 视频track的类型
*
* 其中AliRtcVideoTrackBoth = AliRtcVideoTrackCamera | AliRtcVideoTrackScreen
*/
public enum AliRtcVideoTrack
{
    AliRtcVideoTrackNo = 0,  ///< no video track
    AliRtcVideoTrackCamera = 1,  ///< video from camera, file, etc.
    AliRtcVideoTrackScreen = 2,  ///< video from screen sharing
    AliRtcVideoTrackBoth = 3,  ///< both AliRtcVideoTrackCamera and AliRtcVideoTrackScreen
    AliRtcVideoTrackEnd = 4
}

/**
* @brief 音频track的类型
*/
public enum AliRtcAudioTrack
{
    AliRtcAudioTrackNo = 0,  ///< no audio track
    AliRtcAudioTrackMic = 1,  ///< audio from mic, file, etc.
    AliRtcAudioTrackEnd = 2
}

public enum AliRtcRenderMode
{
    AliRtcRenderModeAuto = 0, ///< Auto adjust
    AliRtcRenderModeStretch = 1, ///< Simply stretch to fill the view without keeping video aspect ratio
    AliRtcRenderModeFill = 2, ///< Scaling while keeping video aspect ratio, and fill black bars to the view
    AliRtcRenderModeCrop = 3, ///< Scaling while keeping video aspect ratio, and crop to fit the view
}

public enum AliRtcScaleRatio
{
    AliRtcScaleRatio_16_9 = 0,
    AliRtcScaleRatio_4_3,
}

public enum AliRtcVideoProfile
{
    AliRtcVideoProfile_Default = 0, // let sdk decide
    AliRtcVideoProfile_180_320P_15,
    AliRtcVideoProfile_180_320P_30,
    AliRtcVideoProfile_360_640P_15,
    AliRtcVideoProfile_360_640P_30,
    AliRtcVideoProfile_720_1280P_15,
    AliRtcVideoProfile_720_1280P_30,
}

public enum AliRtcOnByeType
{
    AliRtcOnByeBeKickOut = 1,               
    AliRtcOnByeChannelTerminated = 2,       
    AliRtcOnByeUserReplaced = 3,            
}

namespace AliRtcSample
{
    public partial class AliRtcSample : Form
    {
        public static AliRtcSample mainFrm = null;
        public static string m_strUid = "";
        public static ParamInfo m_ParamInfo;
        public static Hashtable m_camaraUid2panelHash;
        public static Hashtable m_screenUid2panelHash;
        public static LinkedList<string> m_uidList;
        public static bool m_bInitialItem = false;
        public static int m_nsmallViewSize = 4;
        public delegate void SubscribeResultDelegate
            (string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack);
        public static LinkedList<viewInfo> m_viewInfoList;

        public AliRtcSample()
        {
            InitializeComponent();
            m_camaraUid2panelHash = new Hashtable();
            m_screenUid2panelHash = new Hashtable();
            m_uidList = new LinkedList<string>();
            m_viewInfoList = new LinkedList<viewInfo>();
            initialItem();
            updateParamInfo();
            Control.CheckForIllegalCrossThreadCalls = false;
            mainFrm = this;
            m_bInitialItem = true;
            Layout_ParamItem();
            
        }

        private void initialItem()
        {
            this.richTextBox1.Clear();
            this.richTextBox1.AppendText("127.0.0.1");
            this.check_autoPush.Checked = true;
            this.checkAutoSub.Checked = true;
            this.checkAudio.Checked = true;
            this.checkCamera.Checked = true;
            this.checkH5.Checked = false;
            this.checkScreen.Checked = false;
        }

        private void updateParamInfo()
        {
            m_ParamInfo.bPushAudio = this.checkAudio.Checked;
            m_ParamInfo.bPushCamera = this.checkCamera.Checked;
            m_ParamInfo.bPushScreen = this.checkScreen.Checked;
            m_ParamInfo.bAutoPush = this.check_autoPush.Checked;
            m_ParamInfo.bAutoSub = this.checkAutoSub.Checked;
            m_ParamInfo.bH5Mode = this.checkH5.Checked;
            m_ParamInfo.strAppSer = this.richTextBox1.Text;
            m_ParamInfo.strChannel = this.textBox_ChannelNum.Text;
            m_ParamInfo.strUserName = this.textBox_UserName.Text;
        }

        private void GetPassportFromAppServer(string strAppSrv, string strChannel, string strUserName, string strPassword, out AuthInfo authinfo)
        {
            string requestUrl = strAppSrv + "login?passwd=" + strPassword + "&user=" + strUserName + "&room=" + strChannel;

            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(requestUrl);
            request.ContentType = "application/json";
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream stream = response.GetResponseStream();
            string result = string.Empty;
            StreamReader sr = new StreamReader(stream);
            result = sr.ReadToEnd();
            sr.Close();

            JObject jo = (JObject)JsonConvert.DeserializeObject(result);
            authinfo.strAppId = jo["data"]["appid"].ToString();
            authinfo.strUserId = jo["data"]["userid"].ToString();
            authinfo.strNonce = jo["data"]["nonce"].ToString();
            authinfo.strToken = jo["data"]["token"].ToString();
            string timestamp = jo["data"]["timestamp"].ToString();
            authinfo.iTimesTamp = Convert.ToInt64(timestamp);
            JArray gslbArray = JArray.Parse(jo["data"]["gslb"].ToString());

            authinfo.strGlsb = "";
            for (int index = 0; index < gslbArray.Count(); index++)
            {
                if (index == 0)
                {
                    authinfo.strGlsb = gslbArray[index].ToString();
                    continue;
                }

                authinfo.strGlsb += "|";
                authinfo.strGlsb += gslbArray[index].ToString();
            }

            authinfo.strChannel = strChannel;
            authinfo.strUserName = strUserName;
        }

        public static void OnJoinResult(int iCode)
        {
            if (0 == iCode)
            {
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("加入房间成功...");
            }
            else
            {
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("加入房间失败...");
            }
        }

        public static void onRemoteUserOnLineNotify(string strUid)
        {
            AliRtcSample.mainFrm.listBox_Tips.Items.Add("入会通知：" + strUid);
            if (m_strUid == "")
            {
                m_strUid = strUid;
            }
        }

        public static void onBye(int code)
        {
            AliRtcOnByeType OnByeType = (AliRtcOnByeType)code;
            if (OnByeType == AliRtcOnByeType.AliRtcOnByeBeKickOut)
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("当前用户被踢出频道");
            else if(OnByeType == AliRtcOnByeType.AliRtcOnByeChannelTerminated)
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("频道结束");
            else if(OnByeType == AliRtcOnByeType.AliRtcOnByeUserReplaced)
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("相同用户在其他设备加入频道，当前设备被下线");
            else
                AliRtcSample.mainFrm.listBox_Tips.Items.Add("其他原因离开频道");
            foreach (viewInfo viewInfo_ in m_viewInfoList)
                viewInfo_.showPanel.Hide();
            m_viewInfoList.Clear();
        }

        public static void onOccurError(int error)
        {

        }

        public static void onRemoteUserOffLineNotify(string strUid)
        {
            AliRtcSample.mainFrm.listBox_Tips.Items.Add("离会通知：" + strUid);

            if (m_strUid == strUid)
            {
                m_strUid = "";
            }
        }

        public static void DeleteViewInfo(string struUid, AliRtcVideoTrack videoTrack)
        {
            Debug.Assert(videoTrack == AliRtcVideoTrack.AliRtcVideoTrackCamera ||
                videoTrack == AliRtcVideoTrack.AliRtcVideoTrackScreen);
            List<viewInfo> deleteViewList = new List<viewInfo>();
            foreach (viewInfo viewInfo_ in m_viewInfoList)
            {
                if (viewInfo_.strUid == struUid && 
                    viewInfo_.videoTrack == videoTrack)
                    deleteViewList.Add(viewInfo_);
            }
            foreach(viewInfo deleteView in deleteViewList)
            {
                deleteView.showPanel.Hide();
                m_viewInfoList.Remove(deleteView);
            }
               
        }

        public static void findOrCreatPanel_(string strUid, AliRtcVideoTrack videoTrack, out Panel panel_)
        {
            panel_ = null;
            foreach(viewInfo viewInfo_ in m_viewInfoList)
            {
                if (viewInfo_.strUid == strUid &&
                    viewInfo_.videoTrack == videoTrack)
                {
                    panel_ = viewInfo_.showPanel;
                    return;
                }
            }
            viewInfo tempViewInfo;
            tempViewInfo.strUid = strUid;
            tempViewInfo.videoTrack = videoTrack;
            tempViewInfo.showPanel = new System.Windows.Forms.Panel();
            tempViewInfo.showPanel.BackColor = System.Drawing.SystemColors.ControlDark;
            panel_ = tempViewInfo.showPanel;
            m_viewInfoList.AddLast(tempViewInfo);
        }

        public static void onSubscribeResultDE_(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
        {
            if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackNo ||
                 videoTrack == AliRtcVideoTrack.AliRtcVideoTrackEnd)
            {
                DeleteViewInfo(strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera);
                DeleteViewInfo(strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen);
            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackCamera)
            {
                Panel panel_camera;
                DeleteViewInfo(strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen);
                findOrCreatPanel_(strUid, videoTrack, out panel_camera);
                Debug.Assert(panel_camera != null);
                ImportDll.setRemoteViewConfig(panel_camera.Handle, strUid, videoTrack,
                    AliRtcRenderMode.AliRtcRenderModeFill, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackScreen)
            {
                Panel panel_screen;
                DeleteViewInfo(strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera);
                findOrCreatPanel_(strUid, videoTrack, out panel_screen);
                Debug.Assert(panel_screen != null);
                ImportDll.setRemoteViewConfig(panel_screen.Handle, strUid, videoTrack,
                   AliRtcRenderMode.AliRtcRenderModeFill, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackBoth)
            {
                Panel panel_camera;
                Panel panel_screen;
                findOrCreatPanel_(strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera, out panel_camera);
                findOrCreatPanel_(strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen, out panel_screen);
                Debug.Assert(panel_camera != null);
                Debug.Assert(panel_screen != null);
                ImportDll.setRemoteViewConfig(panel_camera.Handle, strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera,
                    AliRtcRenderMode.AliRtcRenderModeFill, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
                ImportDll.setRemoteViewConfig(panel_screen.Handle, strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen,
                    AliRtcRenderMode.AliRtcRenderModeFill, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
            }
            Layout_RemoteView_();
        }

        public static void onSubscribeResultDE(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
        {
            if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackNo ||
                 videoTrack == AliRtcVideoTrack.AliRtcVideoTrackEnd)
            {
                
                if (m_uidList.Contains(strUid))
                    m_uidList.Remove(strUid);
                if (m_camaraUid2panelHash.Contains(strUid))
                    m_camaraUid2panelHash.Remove(strUid);
                if (m_screenUid2panelHash.Contains(strUid))
                    m_screenUid2panelHash.Remove(strUid);
            }

            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackCamera)
            {
                if (!m_uidList.Contains(strUid))
                    m_uidList.AddLast(strUid);
                if (m_screenUid2panelHash.Contains(strUid))
                    m_screenUid2panelHash.Remove(strUid);
                Panel panel_camera;
                findOrCreatPanel(strUid, videoTrack, out panel_camera);
                Debug.Assert(panel_camera != null);
                ImportDll.setRemoteViewConfig(panel_camera.Handle, strUid, videoTrack,
                    AliRtcRenderMode.AliRtcRenderModeAuto, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackScreen)
            {
                if (!m_uidList.Contains(strUid))
                    m_uidList.AddLast(strUid);
                if (m_camaraUid2panelHash.Contains(strUid))
                    m_camaraUid2panelHash.Remove(strUid);
                Panel panel_screen;
                findOrCreatPanel(strUid, videoTrack, out panel_screen);
                Debug.Assert(panel_screen != null);
                ImportDll.setRemoteViewConfig(panel_screen.Handle, strUid, videoTrack,
                   AliRtcRenderMode.AliRtcRenderModeAuto, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);

            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackBoth)
            {
                if (!m_uidList.Contains(strUid))
                    m_uidList.AddLast(strUid);
                Panel panel_camera;
                Panel panel_screen;
                findOrCreatPanel(strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera, out panel_camera);
                findOrCreatPanel(strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen, out panel_screen);
                Debug.Assert(panel_camera != null);
                Debug.Assert(panel_screen != null);
                ImportDll.setRemoteViewConfig(panel_camera.Handle, strUid, AliRtcVideoTrack.AliRtcVideoTrackCamera,
                    AliRtcRenderMode.AliRtcRenderModeAuto, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
                ImportDll.setRemoteViewConfig(panel_screen.Handle, strUid, AliRtcVideoTrack.AliRtcVideoTrackScreen,
                    AliRtcRenderMode.AliRtcRenderModeAuto, AliRtcScaleRatio.AliRtcScaleRatio_16_9, false);
            }
            Layout_RemoteView();
        }

        public static void onSubscribeResult(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
        {
            AliRtcSample.mainFrm.listBox_Tips.Items.Add("订阅成功：" + strUid);
            AliRtcSample.mainFrm.listBox_Tips.Items.Add("订阅视频流：" + audioTrack.ToString());
            AliRtcSample.mainFrm.listBox_Tips.Items.Add("订阅音频流：" + videoTrack.ToString());
            if (mainFrm.InvokeRequired)
            {
                Object[] parametors = new Object[] { strUid, audioTrack, videoTrack };
                mainFrm.Invoke(new SubscribeResultDelegate(onSubscribeResultDE_), parametors);
            }

        }

        public static void onPubResult(int errCode)
        {
            
        }

        public static void onLayout()
        {
            Rectangle clientRect;
            Rectangle smallViewRect = new Rectangle();
            Rectangle bigViewRect = new Rectangle();
            Rectangle ItemRect = new Rectangle();
            Rectangle remoteDlgRect = new Rectangle();
            clientRect = mainFrm.ClientRectangle;
            smallViewRect.X = 0;
            smallViewRect.Y = 0;
            smallViewRect.Width = clientRect.Width * 3 / 4;
            smallViewRect.Height = clientRect.Height / 4;

            bigViewRect.X = 0;
            bigViewRect.Y = clientRect.Height / 4 + 5;
            bigViewRect.Width = clientRect.Width * 3 / 4;
            bigViewRect.Height = clientRect.Height * 2 / 4;

            ItemRect.X = 0;
            ItemRect.Y = clientRect.Height * 3 / 4;
            ItemRect.Width = clientRect.Width * 3 / 4;
            ItemRect.Height = clientRect.Height / 4;

            remoteDlgRect.X = clientRect.Width * 3 / 4 + 5;
            remoteDlgRect.Y = 0;
            remoteDlgRect.Width = clientRect.Width - clientRect.Width * 3 / 4 - 5;
            remoteDlgRect.Height = clientRect.Height;
        }

        public static void Layout_RemoteView()
        {
            int nsmallViewSize = 0;
            Rectangle clientRect;
            Rectangle smallViewRect = new Rectangle();
            Rectangle bigViewRect = new Rectangle();
            clientRect = mainFrm.ClientRectangle;
            smallViewRect.X = 10 + 25;
            smallViewRect.Y = 10;
            smallViewRect.Width = clientRect.Width * 3 / 4 - 25;
            smallViewRect.Height = clientRect.Height / 6;

            bigViewRect.X = 10;
            bigViewRect.Y = clientRect.Height / 6 + 5;
            bigViewRect.Width = clientRect.Width * 3 / 4;
            bigViewRect.Height = clientRect.Height * 4 / 6;

            if (m_uidList.Count == 0)
                return;
            foreach(string tempUid in m_uidList)
            {
                Debug.Assert(m_camaraUid2panelHash.Contains(tempUid) ||
                    m_screenUid2panelHash.Contains(tempUid));
                if (tempUid == m_uidList.First())
                {
                    //排队1用户
                    if (m_camaraUid2panelHash.Contains(tempUid) &&
                        m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel camera_panel = (Panel)m_camaraUid2panelHash[tempUid];
                        Panel screen_panel = (Panel)m_screenUid2panelHash[tempUid];
                        Debug.Assert(camera_panel != null);
                        Debug.Assert(screen_panel != null);
                        camera_panel.Location = new System.Drawing.Point(bigViewRect.X, bigViewRect.Y);
                        camera_panel.Size = new System.Drawing.Size(bigViewRect.Width, bigViewRect.Height);
                        mainFrm.Controls.Add(camera_panel);

                        Rectangle subSmallViewRect = new Rectangle();
                        if (nsmallViewSize > m_nsmallViewSize){
                            screen_panel.Hide();
                            continue;
                        }
                        subSmallViewRect.X = 
                            smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                        subSmallViewRect.Y = smallViewRect.Y;
                        subSmallViewRect.Width = smallViewRect.Width * (nsmallViewSize + 1) / m_nsmallViewSize;
                        subSmallViewRect.Height = smallViewRect.Height;
                        screen_panel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                        screen_panel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                        mainFrm.Controls.Add(screen_panel);
                        ++nsmallViewSize;
                    }
                    else if (m_camaraUid2panelHash.Contains(tempUid) &&
                        !m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel camera_panel = (Panel)m_camaraUid2panelHash[tempUid];
                        Debug.Assert(camera_panel != null);
                        camera_panel.Location = new System.Drawing.Point(bigViewRect.X, bigViewRect.Y);
                        camera_panel.Size = new System.Drawing.Size(bigViewRect.Width, bigViewRect.Height);
                        mainFrm.Controls.Add(camera_panel);
                    }
                    else if (!m_camaraUid2panelHash.Contains(tempUid) &&
                        m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel screen_panel = (Panel)m_screenUid2panelHash[tempUid];
                        Debug.Assert(screen_panel != null);
                        screen_panel.Location = new System.Drawing.Point(bigViewRect.X, bigViewRect.Y);
                        screen_panel.Size = new System.Drawing.Size(bigViewRect.Width, bigViewRect.Height);
                        mainFrm.Controls.Add(screen_panel);
                    }

                }
                else
                {   //排队非1用户
                    if (m_camaraUid2panelHash.Contains(tempUid) &&
                        m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel camera_panel = (Panel)m_camaraUid2panelHash[tempUid];
                        Panel screen_panel = (Panel)m_screenUid2panelHash[tempUid];
                        Debug.Assert(camera_panel != null);
                        Debug.Assert(screen_panel != null);
                        if (nsmallViewSize > m_nsmallViewSize)
                        {
                            camera_panel.Hide();
                            screen_panel.Hide();
                            continue;
                        }
                        Rectangle subSmallViewRect = new Rectangle();
                        subSmallViewRect.X =
                            smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                        subSmallViewRect.Y = smallViewRect.Y;
                        subSmallViewRect.Width = smallViewRect.Width * (nsmallViewSize + 1) / m_nsmallViewSize;
                        subSmallViewRect.Height = smallViewRect.Height;

                        camera_panel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                        camera_panel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                        mainFrm.Controls.Add(camera_panel);
                        ++nsmallViewSize;

                        if (nsmallViewSize > m_nsmallViewSize)
                        {
                            screen_panel.Hide();
                            continue;
                        }
                        subSmallViewRect.X =
                              smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                        subSmallViewRect.Y = smallViewRect.Y;
                        subSmallViewRect.Width = smallViewRect.Width * (nsmallViewSize + 1) / m_nsmallViewSize;
                        subSmallViewRect.Height = smallViewRect.Height;

                        screen_panel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                        screen_panel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                        mainFrm.Controls.Add(screen_panel);
                        ++nsmallViewSize;
                    }
                    else if (m_camaraUid2panelHash.Contains(tempUid) &&
                        !m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel camera_panel = (Panel)m_camaraUid2panelHash[tempUid];
                        Debug.Assert(camera_panel != null);
                        if (nsmallViewSize > m_nsmallViewSize){
                            camera_panel.Hide();
                            continue;
                        }

                        Rectangle subSmallViewRect = new Rectangle();
                        subSmallViewRect.X =
                            smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                        subSmallViewRect.Y = smallViewRect.Y;
                        subSmallViewRect.Width = smallViewRect.Width * (nsmallViewSize + 1) / m_nsmallViewSize;
                        subSmallViewRect.Height = smallViewRect.Height;
                        //测试
                        camera_panel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                        camera_panel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                        mainFrm.Controls.Add(camera_panel);
                        ++nsmallViewSize;

                    }

                    else if (!m_camaraUid2panelHash.Contains(tempUid) &&
                        m_screenUid2panelHash.Contains(tempUid))
                    {
                        Panel screen_panel = (Panel)m_screenUid2panelHash[tempUid];
                        Debug.Assert(screen_panel != null);
                        if (nsmallViewSize > m_nsmallViewSize)
                        {
                            screen_panel.Hide();
                            continue;
                        }
                        Rectangle subSmallViewRect = new Rectangle();
                        subSmallViewRect.X =
                            smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                        subSmallViewRect.Y = smallViewRect.Y;
                        subSmallViewRect.Width = smallViewRect.Width * (nsmallViewSize + 1) / m_nsmallViewSize;
                        subSmallViewRect.Height = smallViewRect.Height;

                        screen_panel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                        screen_panel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                        mainFrm.Controls.Add(screen_panel);
                        ++nsmallViewSize;
                    }

                }
                   

            }

        }

        public static void hideAllPanel()
        {
            foreach (viewInfo viewInfo_ in m_viewInfoList)
                viewInfo_.showPanel.Hide();
        }

        public static void Layout_RemoteView_()
        {
            int nsmallViewSize = 0;
            Rectangle clientRect;
            Rectangle subSmallViewRect = new Rectangle();
            Rectangle smallViewRect = new Rectangle();
            Rectangle bigViewRect = new Rectangle();
            clientRect = mainFrm.ClientRectangle;
            smallViewRect.X = 10 + 25;
            smallViewRect.Y = 10;
            smallViewRect.Width = clientRect.Width * 3 / 4 - 25;
            smallViewRect.Height = clientRect.Height / 6;

            bigViewRect.X = 10;
            bigViewRect.Y = clientRect.Height / 6 + 5;
            bigViewRect.Width = clientRect.Width * 3 / 4;
            bigViewRect.Height = clientRect.Height * 4 / 6;

            if (m_viewInfoList.Count() == 0)
                return;
            //hideAllPanel();
            viewInfo FirstViewInfo = m_viewInfoList.First();
            foreach (viewInfo viewInfo_ in m_viewInfoList)
            {
                if (viewInfo_.strUid == FirstViewInfo.strUid &&
                    viewInfo_.videoTrack == FirstViewInfo.videoTrack &&
                    viewInfo_.showPanel == FirstViewInfo.showPanel)
                {
                    viewInfo_.showPanel.Location =
                        new System.Drawing.Point(bigViewRect.X, bigViewRect.Y);
                    viewInfo_.showPanel.Size = 
                        new System.Drawing.Size(bigViewRect.Width, bigViewRect.Height);
                    mainFrm.Controls.Add(viewInfo_.showPanel);
                }
                else
                {
                    subSmallViewRect.X =
                         smallViewRect.X + smallViewRect.Width * nsmallViewSize / m_nsmallViewSize;
                    subSmallViewRect.Y = smallViewRect.Y;
                    subSmallViewRect.Width = smallViewRect.Width / m_nsmallViewSize;
                    subSmallViewRect.Height = smallViewRect.Height;
                    viewInfo_.showPanel.Location = new System.Drawing.Point(subSmallViewRect.X, subSmallViewRect.Y);
                    viewInfo_.showPanel.Size = new System.Drawing.Size(subSmallViewRect.Width, subSmallViewRect.Height - 5);
                    mainFrm.Controls.Add(viewInfo_.showPanel);
                    ++nsmallViewSize;
                }
            }

        }

        public static void Layout_ParamItem()
        {
            Rectangle clientRect = mainFrm.ClientRectangle;
            Rectangle ParamItemRect = new Rectangle();
            ParamItemRect.X = 10;
            ParamItemRect.Y = clientRect.Height * 5 / 6;
            ParamItemRect.Width = clientRect.Width * 3 / 4;
            ParamItemRect.Height = clientRect.Height / 6;

            Rectangle smallViewRect = new Rectangle();
            smallViewRect.X = 10;
            smallViewRect.Y = 10;
            smallViewRect.Width = clientRect.Width * 3 / 4;
            smallViewRect.Height = clientRect.Height / 6;

            Rectangle remoteDlgRect = new Rectangle();
            remoteDlgRect.X = clientRect.Width * 3 / 4 + 5;
            remoteDlgRect.Y = 10;
            remoteDlgRect.Width = clientRect.Width / 4 - 5;
            remoteDlgRect.Height = clientRect.Height;


            mainFrm.btn_left.Location = 
                new System.Drawing.Point(smallViewRect.X, smallViewRect.Y);
            mainFrm.btn_left.Size = new System.Drawing.Size(25, smallViewRect.Height - 10);

            mainFrm.btn_right.Location =
                new System.Drawing.Point(clientRect.Width * 3 / 4 - 25, smallViewRect.Y);
            mainFrm.btn_right.Size = new System.Drawing.Size(25, smallViewRect.Height - 10);

            mainFrm.groupBox_LocalView.Location =
                new System.Drawing.Point(remoteDlgRect.X + 10, remoteDlgRect.Y);
            mainFrm.groupBox_LocalView.Size =
                new System.Drawing.Size(remoteDlgRect.Width - 10, (remoteDlgRect.Width - 10) / 2);
            mainFrm.groupBox_LocalView.Controls.Add(mainFrm.panel_LocalView);
            mainFrm.panel_LocalView.Location = new System.Drawing.Point(5, 15);
            mainFrm.panel_LocalView.Size =
                new System.Drawing.Size(remoteDlgRect.Width - 20, (remoteDlgRect.Width - 10) / 2 - 20);

            Rectangle tempRect = new Rectangle();
            tempRect.X = ParamItemRect.X;
            tempRect.Y = ParamItemRect.Y + 20;

            mainFrm.label_ChannelNum.Location = 
                new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.label_ChannelNum.Size = new System.Drawing.Size(89, 18);

            tempRect.X += 65;
            mainFrm.textBox_ChannelNum.Location = 
                new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.textBox_ChannelNum.Size = new System.Drawing.Size(ParamItemRect.Width / 6, 28);

            tempRect.X = ParamItemRect.X;
            tempRect.Y = ParamItemRect.Y + 50;
            mainFrm.labelUserName.Location =
                new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.labelUserName.Size = new System.Drawing.Size(89, 18);

            tempRect.X += 65;
            mainFrm.textBox_UserName.Location =
                new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.textBox_UserName.Size = new System.Drawing.Size(ParamItemRect.Width / 6, 28);

            tempRect.X += ParamItemRect.Width / 6 + 5;
            tempRect.Y = ParamItemRect.Y + 10;

            mainFrm.groupBox1.Controls.Add(mainFrm.richTextBox1);
            mainFrm.groupBox1.Location = 
                new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.groupBox1.Size = new System.Drawing.Size(ParamItemRect.Width / 4, 78);

            mainFrm.richTextBox1.Location = new System.Drawing.Point(6, 12);
            mainFrm.richTextBox1.Size = new System.Drawing.Size(ParamItemRect.Width / 4 - 10, 63);


            tempRect.X += ParamItemRect.Width / 4 + 5;
            mainFrm.groupBox2.Controls.Add(mainFrm.check_autoPush);
            mainFrm.groupBox2.Controls.Add(mainFrm.checkAutoSub);
            mainFrm.groupBox2.Controls.Add(mainFrm.checkH5);

            mainFrm.groupBox2.Location = new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.groupBox2.Size = new System.Drawing.Size(ParamItemRect.Width / 8, 78);

            tempRect.X += ParamItemRect.Width / 8 + 5;
            mainFrm.groupBox3.Controls.Add(mainFrm.checkAudio);
            mainFrm.groupBox3.Controls.Add(mainFrm.checkCamera);
            mainFrm.groupBox3.Controls.Add(mainFrm.checkScreen);
            mainFrm.groupBox3.Location = new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.groupBox3.Size = new System.Drawing.Size(ParamItemRect.Width / 8, 78);

            tempRect.X += ParamItemRect.Width / 8 + 5;
            tempRect.Y += 10;

            tempRect.Y += 22;

            tempRect.Y += 22;
            tempRect.Y = ParamItemRect.Y + 10;
            mainFrm.button_Join.Location = new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.button_Join.Size = new System.Drawing.Size(80, 30);

            tempRect.Y += 40;
            mainFrm.button_Leave.Location = new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.button_Leave.Size = new System.Drawing.Size(80, 30);

            tempRect.X += 85;
            tempRect.Y = ParamItemRect.Y + 10;
            mainFrm.listBox_Tips.Location = new System.Drawing.Point(tempRect.X, tempRect.Y);
            mainFrm.listBox_Tips.Size = new System.Drawing.Size(150, 78);
        }

        public static void findOrCreatPanel(string strUid, AliRtcVideoTrack videoTrack, out Panel panel_)
        {
            if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackCamera)
            {
                if (m_camaraUid2panelHash.Contains(strUid)){
                    panel_ = (Panel)m_camaraUid2panelHash[strUid];
                }
                else{
                    panel_ = new System.Windows.Forms.Panel();
                    panel_.BackColor = System.Drawing.SystemColors.ControlDark;
                    m_camaraUid2panelHash.Add(strUid, panel_);
                }
            }
            else if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackScreen)
            {
                if (m_screenUid2panelHash.Contains(strUid)){
                    panel_ = (Panel)m_screenUid2panelHash[strUid];
                }
                else{
                    panel_ = new System.Windows.Forms.Panel();
                    panel_.BackColor = System.Drawing.SystemColors.ControlDark;
                    m_screenUid2panelHash.Add(strUid, panel_);
                }
            }
            else
            {
                panel_ = null;
            }
        }

        public static void onSubscribeChangedNotify(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
        {
            
        }

        public static void onRemoteTrackAvailableNotify(string strUid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
        {
            if (!m_ParamInfo.bAutoSub){
                //当自动订阅开关关闭时，所有用户都手动订阅
                if (audioTrack == AliRtcAudioTrack.AliRtcAudioTrackMic)
                    ImportDll.configRemoteAudio(strUid, true);
                else
                    ImportDll.configRemoteAudio(strUid, false);
                if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackCamera ||
                    videoTrack == AliRtcVideoTrack.AliRtcVideoTrackBoth)
                    ImportDll.configRemoteCameraTrack(strUid, true, true);
                else
                    ImportDll.configRemoteCameraTrack(strUid, true, false);

                if (videoTrack == AliRtcVideoTrack.AliRtcVideoTrackScreen ||
                    videoTrack == AliRtcVideoTrack.AliRtcVideoTrackBoth)
                    ImportDll.configRemoteScreenTrack(strUid, true);
                else
                    ImportDll.configRemoteScreenTrack(strUid, false);
                ImportDll.setSubscribeResultCB SubscribeResultCB;
                SubscribeResultCB = new ImportDll.setSubscribeResultCB(onSubscribeResult);
                ImportDll.setSubscribeResultCallBack(SubscribeResultCB);

                bool bRet = ImportDll.subscribe(strUid);

                if (bRet == false)
                {
                    AliRtcSample.mainFrm.listBox_Tips.Items.Add("订阅失败,是否调用onSubscribeResultCallBack回调？");
                }
            }
            else
            {
                //开启自动订阅开关时，订阅所有有流的用户
                onSubscribeResult(strUid, audioTrack, videoTrack);
            }
        }

        private void dllJoinChannel(AuthInfo autoinfo)
        {
            ImportDll.joinResultCB = new ImportDll.setJoinResultCB(OnJoinResult);
            ImportDll.setJoinResultCallBack(ImportDll.joinResultCB);

            ImportDll.RemoteUserOnLineNotifyCB = new ImportDll.setRemoteUserOnLineNotifyCB(onRemoteUserOnLineNotify);
            ImportDll.setRemoteUserOnLineNotifyCallBack(ImportDll.RemoteUserOnLineNotifyCB);
            ImportDll.RemoteUserOffLineNotifyCB = new ImportDll.setRemoteUserOffLineNotifyCB(onRemoteUserOffLineNotify);
            ImportDll.setRemoteUserOffLineNotifyCallBack(ImportDll.RemoteUserOffLineNotifyCB);
            ImportDll.SubscribeChangedNotifyCB = new ImportDll.setSubscribeChangedNotifyCB(onSubscribeChangedNotify);
            ImportDll.setSubscribeChangedNotifyCallBack(ImportDll.SubscribeChangedNotifyCB);
            ImportDll.RemoteTrackAvailableNotifyCB = new ImportDll.setRemoteTrackAvailableNotifyCB(onRemoteTrackAvailableNotify);
            ImportDll.setRemoteTrackAvailableNotifyCallBack(ImportDll.RemoteTrackAvailableNotifyCB);

            ImportDll.ByeCB = new ImportDll.setByeCB(onBye);
            ImportDll.setByeCallBack(ImportDll.ByeCB);
            ImportDll.OccurErrorCB = new ImportDll.setOccurErrorCB(onOccurError);
            ImportDll.setOccurErrorCallBack(ImportDll.OccurErrorCB);


            ImportDll.setLogLevel(AliRtcLogLevel.AliRtcLogLevelInfo);
            ImportDll.setAutoPublishSubscribe(m_ParamInfo.bAutoPush, m_ParamInfo.bAutoSub);
            ImportDll.configLocalAudioPublish(m_ParamInfo.bPushAudio);
            ImportDll.configLocalCameraPublish(m_ParamInfo.bPushCamera);
            ImportDll.configLocalScreenPublish(m_ParamInfo.bPushScreen);
            ImportDll.configLocalSimulcast(true, AliRtcVideoTrack.AliRtcVideoTrackCamera);
            
            bool bRet = ImportDll.joinChannel(autoinfo.strChannel, autoinfo.strAppId, autoinfo.strUserId, autoinfo.strNonce, autoinfo.strToken, autoinfo.strGlsb, autoinfo.strUserName, autoinfo.iTimesTamp);

            if (bRet == false)
            {
                listBox_Tips.Items.Add("加入房间失败...");
            }
        }

        private void button_Join_Click(object sender, EventArgs e)
        {
            ImportDll.setH5CompatibleMode(m_ParamInfo.bH5Mode);
            if (false == ImportDll.initAliSdk())
            {
                MessageBox.Show("sdk初始化失败...");
                return;
            }
            updateParamInfo();
            string appServerUrl = m_ParamInfo.strAppSer;
            string channel = m_ParamInfo.strChannel;
            string userName = m_ParamInfo.strUserName;
            string passward = "hello1234";
            AuthInfo autoinfo;

            GetPassportFromAppServer(appServerUrl, channel, userName, passward, out autoinfo);

            ImportDll.setLocalViewConfig(this.panel_LocalView.Handle);
 
            if (0 < ImportDll.startPreview())
                listBox_Tips.Items.Add("加载本地浏览成功...");
            else
                listBox_Tips.Items.Add("加载本地浏览失败...");
        

            dllJoinChannel(autoinfo);
        }

        private void button_Leave_Click(object sender, EventArgs e)
        {
            ImportDll.releaseAliSdk();

            listBox_Tips.Items.Add("离开房间...");
        }

        private void AliRtcSample_SizeChanged(object sender, EventArgs e)
        {
            if (!m_bInitialItem)
                return;
            Layout_ParamItem();
            Layout_RemoteView_();
        }

        private void AliRtcSample_FormClosing(object sender, EventArgs e)
        {
            mainFrm = null;
        }

        private void btn_left_Click(object sender, EventArgs e)
        {
            if (m_viewInfoList.Count() == 0)
                return;
            viewInfo FirstViewInfo = m_viewInfoList.First();
            m_viewInfoList.RemoveFirst();
            m_viewInfoList.AddLast(FirstViewInfo);
            Layout_RemoteView_();
        }

        private void btn_right_Click(object sender, EventArgs e)
        {
            if (m_viewInfoList.Count() == 0)
                return;
            viewInfo LastViewInfo = m_viewInfoList.Last();
            m_viewInfoList.RemoveLast();
            m_viewInfoList.AddFirst(LastViewInfo);
            Layout_RemoteView_();
        }

        private void checkAudio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bInitialItem)
                return;
            m_ParamInfo.bPushAudio = mainFrm.checkAudio.Checked;
            ImportDll.configLocalAudioPublish(m_ParamInfo.bPushAudio);
            ImportDll.publish();
        }

        private void checkCamera_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bInitialItem)
                return;
            m_ParamInfo.bPushCamera = mainFrm.checkCamera.Checked;
            ImportDll.configLocalCameraPublish(m_ParamInfo.bPushCamera);

            ImportDll.setPubResultCB PubResultCB;
            PubResultCB = new ImportDll.setPubResultCB(onPubResult);
            ImportDll.setPublishResultCallBack(PubResultCB);
            ImportDll.publish();
        }

        private void checkScreen_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bInitialItem)
                return;
            m_ParamInfo.bPushScreen = mainFrm.checkScreen.Checked;
            ImportDll.configLocalScreenPublish(m_ParamInfo.bPushScreen);

            ImportDll.setPubResultCB PubResultCB;
            PubResultCB = new ImportDll.setPubResultCB(onPubResult);
            ImportDll.setPublishResultCallBack(PubResultCB);
            ImportDll.publish();
        }
    }
}