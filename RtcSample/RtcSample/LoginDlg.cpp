// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RtcSample.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "AliRtcEngine.h"
#include "FakeAuthrization.h" 



// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CLoginDlg::OnInitDialog() {
    CString version = toCString(AliRtcEngine::getSdkVersion());
    GetDlgItem(IDC_STATIC_VERSION)->SetWindowTextW(version.GetBuffer());
    
    return TRUE;
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg message handlers


void CLoginDlg::OnBnClickedOk()
{
    CString txt;
    GetDlgItem(IDC_EDIT_CHANNLE)->GetWindowTextW(txt);
    authinfo.channel = toString(txt).c_str();
    if (authinfo.channel.size() == 0) {
        AfxMessageBox(_T("channel name illegal!"));
        return;
    }
    bool success = GetPassportFromAppServer(authinfo);
    if (!success) {
        AfxMessageBox(_T("Connect to Server fail!")); 
        return;
    }
    CDialogEx::OnOK();
}

