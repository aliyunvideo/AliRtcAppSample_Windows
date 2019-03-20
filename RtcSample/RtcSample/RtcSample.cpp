
// RtcSample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RtcSample.h"
#include "RtcSampleDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRtcSampleApp

BEGIN_MESSAGE_MAP(CRtcSampleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRtcSampleApp construction

CRtcSampleApp::CRtcSampleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRtcSampleApp object

CRtcSampleApp theApp;


// CRtcSampleApp initialization

BOOL CRtcSampleApp::InitInstance()
{
	CWinApp::InitInstance();


	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
   
    while (1) {
        CLoginDlg loginDlg;
        INT_PTR nRes = loginDlg.DoModal();
        if (nRes == IDCANCEL) {
            //login fail
            break;
        }
        
        CRtcSampleDlg dlg;
        dlg.authinfo.appid = loginDlg.authinfo.appid;
        dlg.authinfo.channel = loginDlg.authinfo.channel;
        dlg.authinfo.nonce = loginDlg.authinfo.nonce;
        dlg.authinfo.timestamp = loginDlg.authinfo.timestamp;
        dlg.authinfo.token = loginDlg.authinfo.token;
        dlg.authinfo.user_id = loginDlg.authinfo.user_id;
        for (int i = 0; i < loginDlg.authinfo.gslb.size(); ++i) {
            dlg.authinfo.gslb.AddString(loginDlg.authinfo.gslb.at(0));
        }

        INT_PTR nResponse = dlg.DoModal();
        if (nResponse == IDCANCEL)
        {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with Cancel
            break;
        }
        else if (nResponse == -1)
        {
            TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
            break;
        }
    }


#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

