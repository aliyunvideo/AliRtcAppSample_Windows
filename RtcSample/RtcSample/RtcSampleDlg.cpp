
// RtcSampleDlg.cpp : implementation file
//
#include "stdafx.h"
#include "RtcSample.h"
#include "RtcSampleDlg.h"
#include "afxdialogex.h"

// Temporarily demo the process of authrizing to App Server.
// Implement the "bool GetPassportFromAppServer(AliRtcAuthInfo &authinfo)" function.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRtcSampleDlg dialog
CRtcSampleDlg::CRtcSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RTCSAMPLE_DIALOG, pParent)
{
    m_pEngine = nullptr;

    m_isPreviewing = false;
    m_isInChannel = false;
    m_isSharing = false;

    m_curRemoteUserID[0] = 0;
}

void CRtcSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRtcSampleDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_JOIN, &CRtcSampleDlg::OnBnClickedBtnJoin)
    ON_BN_CLICKED(IDC_BTN_SCREENSHARE, &CRtcSampleDlg::OnBnClickedBtnScreenshare)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_RESTART, &CRtcSampleDlg::OnBnClickedButtonLeave)
END_MESSAGE_MAP()


// CRtcSampleDlg message handlers

BOOL CRtcSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    
    // create engine sdk instance
    m_pEngine = AliRtcEngine::sharedInstance(this, "");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRtcSampleDlg::OnBnClickedBtnJoin()
{
    if (m_pEngine == nullptr)
        return;
    if (m_isInChannel) 
    {
        return;
    }
    
    // If you what to automatically publish and subscribe the local audio and video after join channel
    m_pEngine->setAutoPublishSubscribe(true, true); // Config before join channel

    // Call back when join channel finished
    auto onJoinResult = [](void *opaque, int errCode) {
        CRtcSampleDlg *pThis = (CRtcSampleDlg *)opaque;
        // Join Success
        if (errCode == 0) 
        {
            pThis->m_isInChannel = true;
            AfxMessageBox(L"Join Channel success!");

            // Get display window
            AliVideoCanvas canvas;
            canvas.hWnd = pThis->GetDlgItem(IDC_WND_PREVIEW)->m_hWnd;

            // Config preview window to engine
            pThis->m_pEngine->setLocalViewConfig(canvas, AliRtcVideoTrackCamera);
            pThis->m_pEngine->startPreview();
        }
        else // Join Fail
        {
            AfxMessageBox(_T("join channel fail!"));
            pThis->GetDlgItem(IDC_BTN_JOIN)->EnableWindow(TRUE);
        }
    };
    m_pEngine->joinChannel(authinfo, "YourUserName", onJoinResult, this);
    GetDlgItem(IDC_BTN_JOIN)->EnableWindow(FALSE);       
}


void CRtcSampleDlg::UpdateSubscribe(AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
{
    // Subscribe the valid remote streams
    if (audioTrack == AliRtcAudioTrack::AliRtcAudioTrackMic)
    {
        m_pEngine->configRemoteAudio(m_curRemoteUserID, true);
    }
    else if (audioTrack == AliRtcAudioTrack::AliRtcAudioTrackNo) 
    {
        m_pEngine->configRemoteAudio(m_curRemoteUserID, false);
    }
    
    if (videoTrack == AliRtcVideoTrack::AliRtcVideoTrackCamera)
    {
        m_pEngine->configRemoteCameraTrack(m_curRemoteUserID, true, true);
        m_pEngine->configRemoteScreenTrack(m_curRemoteUserID, false);
    }
    else if (videoTrack == AliRtcVideoTrack::AliRtcVideoTrackScreen || videoTrack == AliRtcVideoTrack::AliRtcVideoTrackBoth)
    {
        m_pEngine->configRemoteCameraTrack(m_curRemoteUserID, true, false);
        m_pEngine->configRemoteScreenTrack(m_curRemoteUserID, true);  
    }
    else if (videoTrack == AliRtcVideoTrack::AliRtcVideoTrackNo) {
        m_pEngine->configRemoteCameraTrack(m_curRemoteUserID, true, false);
        m_pEngine->configRemoteScreenTrack(m_curRemoteUserID, false);
    }

    auto onSubResult = [](void *opaque, const AliRtc::String &uid, AliRtcVideoTrack vt, AliRtcAudioTrack at) {
        CRtcSampleDlg *pThis = (CRtcSampleDlg *)opaque;
        // Add the video window for subscribed video stream
        AliVideoCanvas canvas1, canvas2;
        if (vt == AliRtcVideoTrack::AliRtcVideoTrackCamera)
        {
            canvas1.hWnd = pThis->GetDlgItem(IDC_WND_USER)->m_hWnd;            
            canvas2.hWnd = nullptr;
        }
        else if (vt == AliRtcVideoTrack::AliRtcVideoTrackScreen || vt == AliRtcVideoTrack::AliRtcVideoTrackBoth)
        {
            canvas1.hWnd = nullptr;
            canvas2.hWnd = pThis->GetDlgItem(IDC_WND_USER)->m_hWnd;
        }
        else if (vt == AliRtcVideoTrack::AliRtcVideoTrackNo) 
        {           
            canvas1.hWnd = nullptr;   
            canvas2.hWnd = nullptr;
        }
        pThis->m_pEngine->setRemoteViewConfig(&canvas1, uid, AliRtcVideoTrack::AliRtcVideoTrackCamera);
        pThis->m_pEngine->setRemoteViewConfig(&canvas2, uid, AliRtcVideoTrack::AliRtcVideoTrackScreen);
    };
    m_pEngine->subscribe(m_curRemoteUserID, onSubResult, this);
}

void CRtcSampleDlg::OnBnClickedBtnScreenshare()
{
    if (m_pEngine == nullptr)
        return;

    if (!m_isInChannel)
    {
        AfxMessageBox(L"Please join channel firstly!");
        return;
    }

    m_isSharing = !m_isSharing;

    m_pEngine->configLocalScreenPublish(m_isSharing);

    auto onPubResult = [](void *opaque, int errCode) {
        CRtcSampleDlg *pThis = (CRtcSampleDlg *)opaque;

        // publish Success
        if (errCode == 0)
        {
        }
        else // publish Fail
        {
            pThis->m_isSharing = !pThis->m_isSharing;
        }
            
        pThis->GetDlgItem(IDC_BTN_SCREENSHARE)->SetWindowText(pThis->m_isSharing ? L"Í£Ö¹¹²Ïí" : L"ÆÁÄ»¹²Ïí");
    };

    m_pEngine->publish(onPubResult, this);
    GetDlgItem(IDC_BTN_SCREENSHARE)->EnableWindow(FALSE);
}

void CRtcSampleDlg::OnBnClickedButtonLeave()
{
    if (m_pEngine == nullptr)
        return;
    if (m_isInChannel) {
        m_pEngine->stopPreview();
        m_pEngine->leaveChannel();
        m_isInChannel = false;
        GetDlgItem(IDC_BTN_JOIN)->EnableWindow(TRUE);
    }
    CDialogEx::OnOK();
}

void CRtcSampleDlg::OnClose()
{
    if (m_pEngine != nullptr && m_isInChannel) {
        m_pEngine->stopPreview();
        m_pEngine->leaveChannel();
        m_pEngine = nullptr;
    }
        
    __super::OnClose();
}

// Implement of Events call back form engine
void CRtcSampleDlg::onRemoteUserOnLineNotify(const AliRtc::String & uid)
{
    CString strInfo;
    strInfo.Format(_T("user(%s) online\n"), toCString(uid.c_str()).GetBuffer());
    if (m_curRemoteUserID.isEmpty()) // Just demo the first user
    {
        m_curRemoteUserID = uid;
    }
    CString strInfo2;
    strInfo2.Format(_T("current user(%s)"), toCString(m_curRemoteUserID.c_str()).GetBuffer());
    AfxMessageBox(strInfo+strInfo2);
}

void CRtcSampleDlg::onRemoteUserOffLineNotify(const AliRtc::String & uid)
{
    CString strInfo;
    strInfo.Format(_T("user(%s) offline\n"), toCString(uid.c_str()).GetBuffer());
    if (!m_curRemoteUserID.isEmpty() && m_curRemoteUserID == uid)
    {
        m_curRemoteUserID = "";
    }
    CString strInfo2;
    strInfo2.Format(_T("current user(%s)"), toCString(m_curRemoteUserID.c_str()).GetBuffer());
    AfxMessageBox(strInfo + strInfo2);
}

void CRtcSampleDlg::onRemoteTrackAvailableNotify(const AliRtc::String & uid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
{
    CString strInfo;
    strInfo.Format(_T("Remote track\n user(%s) audio=%d video=%d\n current user(%s)"), toCString(uid.c_str()).GetBuffer(), audioTrack, videoTrack, toCString(m_curRemoteUserID.c_str()).GetBuffer());
    AfxMessageBox(strInfo);
    if (m_curRemoteUserID == uid)
    {
        UpdateSubscribe(audioTrack, videoTrack);
    }
}

void CRtcSampleDlg::onSubscribeChangedNotify(const AliRtc::String & uid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
{
    CString strInfo;
    strInfo.Format(_T("subscribe change\n user(%s) audio=%d video=%d\n current user(%s)"), toCString(uid.c_str()).GetBuffer(), audioTrack, videoTrack, toCString(m_curRemoteUserID.c_str()).GetBuffer());
    AfxMessageBox(strInfo);
    if (m_curRemoteUserID == uid)
    {
        UpdateSubscribe(audioTrack, videoTrack);
    }
}

void CRtcSampleDlg::onOccurError(int error)
{
    CString errMsg;
    errMsg.Format(_T("onOccurError: %d"), error);
    AfxMessageBox(errMsg.GetBuffer());

    if (0x0102020C == error || 0x02010105 == error) {
        AliRtcEngine::destroy();
        m_pEngine = nullptr;
        CDialogEx::OnCancel();
    }
}


