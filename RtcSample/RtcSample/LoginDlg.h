#pragma once
#include "AliRtcEngine.h"
// CLoginDlg dialog

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    //OK button click function
    afx_msg void OnBnClickedOk();
    AliRtcAuthInfo authinfo;
private:
    CStatic mVersion;
    CEdit mChannle;
    CString mStrChannel;
};
