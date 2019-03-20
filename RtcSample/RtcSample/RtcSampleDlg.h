
// RtcSampleDlg.h : header file

#pragma once

// Reference for RTCEngine header file and libs
#include "AliRtcEngine.h"
#pragma comment(lib, "AliRTCSdk.lib")


// CRtcSampleDlg dialog
class CRtcSampleDlg : public CDialogEx, AliRtcEventListener // Implement the event callback from RTCEngine
{
// Construction
public:
	CRtcSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RTCSAMPLE_DIALOG };
#endif

protected:
virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    //join buttion click function
    afx_msg void OnBnClickedBtnJoin();
    //share screen click function
    afx_msg void OnBnClickedBtnScreenshare();
    //close button click function 
    afx_msg void OnClose();
    //leave button click function
    afx_msg void OnBnClickedButtonLeave();
    AliRtcAuthInfo authinfo;
private:
    // The only instance of RTCEngine
    AliRtcEngine *m_pEngine;
    bool m_isPreviewing;
    bool m_isInChannel;
    bool m_isSharing;
    AliRtc::String m_curRemoteUserID;
    void UpdateSubscribe(AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack);

private:
    // Interested Events from engine
    virtual void onRemoteUserOnLineNotify(const AliRtc::String &uid) override;
    virtual void onRemoteUserOffLineNotify(const AliRtc::String &uid) override;
    virtual void onRemoteTrackAvailableNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack,
        AliRtcVideoTrack videoTrack) override;
    virtual void onSubscribeChangedNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack,
        AliRtcVideoTrack videoTrack) override;
    virtual void onOccurError(int error) override;
};
