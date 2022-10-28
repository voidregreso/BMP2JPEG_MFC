
// BMP2JPEG_MFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "BMP2JPEG_MFC.h"
#include "BMP2JPEG_MFCDlg.h"
#include "afxdialogex.h"

#include <Gdiplus.h>
#include <Objbase.h>
#include <Objidl.h>
#include <vector>
#include <commctrl.h>
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Uuid.lib")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "comctl32.lib")

using namespace std;
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx {
public:
    CAboutDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBMP2JPEGMFCDlg 对话框



CBMP2JPEGMFCDlg::CBMP2JPEGMFCDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_BMP2JPEG_MFC_DIALOG, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMP2JPEGMFCDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMP2JPEGMFCDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON2, &CBMP2JPEGMFCDlg::OnBnClickedButton2)
    ON_EN_CHANGE(IDC_EDIT1, &CBMP2JPEGMFCDlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDOK, &CBMP2JPEGMFCDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON1, &CBMP2JPEGMFCDlg::OnBnClickedButton1)
    ON_WM_DROPFILES()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CBMP2JPEGMFCDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CBMP2JPEGMFCDlg 消息处理程序

BOOL CBMP2JPEGMFCDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    // 将“关于...”菜单项添加到系统菜单中。
    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标
    //注意一定要初始化GDIPLUS才能使用否则会appcrash!
    ULONG_PTR	gdiplusToken;	//GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    //初始化slidercontrol
    CSliderCtrl *cs = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
    cs->SetRange(0, 100);//设置范围
    cs->SetTicFreq(1);//设置显示刻度的间隔
    cs->SetPos(100);//当前停留的位置
    cs->SetLineSize(5);//一行的大小，对应键盘的方向键
    UpdateData(TRUE);
    GetDlgItem(IDC_STATIC2)->SetWindowText(L"图片质量:100");
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBMP2JPEGMFCDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else
        CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBMP2JPEGMFCDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    } else
        CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBMP2JPEGMFCDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

CString filename = L"";
vector<CString> fns;
BOOL multifiles = FALSE;

void CBMP2JPEGMFCDlg::OnBnClickedButton2() {
    // TODO: 在此添加控件通知处理程序代码
    BOOL isOpen = TRUE;
    CString filter = L"位图(*.bmp)|*.bmp|所有类型(*.*)|*.*||";
    CFileDialog openFileDlg(isOpen, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
    INT_PTR result = openFileDlg.DoModal();//是否选中文件，或取消了选择
    if (result == IDOK) {
        filename = openFileDlg.GetPathName();
        GetDlgItem(IDC_EDIT1)->SetWindowTextW(filename);
        multifiles = FALSE;
    }
}

INT GetEncoderClsid(const WCHAR *format, CLSID *pClsid) {
    UINT num = 0;          // number of image encoders
    UINT size = 0;         // size of the image encoder array in bytes
    ImageCodecInfo *pImageCodecInfo  = NULL;
    GetImageEncodersSize(&num, &size);
    if (size  == 0) return  -1;
    pImageCodecInfo  = (ImageCodecInfo *)(malloc(size));
    if (pImageCodecInfo  == NULL) return  -1;
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j  = 0; j  < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid  = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }
    free(pImageCodecInfo);
    return  - 1;  // Failure
}

void CBMP2JPEGMFCDlg::OnEnChangeEdit1() {
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
    GetDlgItem(IDC_EDIT1)->GetWindowTextW(filename);
    multifiles = false;
}


void CBMP2JPEGMFCDlg::OnBnClickedOk() {
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnOK();
    exit(0);
}

BOOL convert(CString myfn, UINT quality) {
    Bitmap fImage(myfn);
    EncoderParameters encoderParameters;
    CLSID jpgClsid;
    GetEncoderClsid(L"image/jpeg", &jpgClsid);
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &quality;
    Status status = fImage.Save(myfn + "_converted.jpg", &jpgClsid, &encoderParameters);
    if (status != Ok) return FALSE;
    return TRUE;
}

void CBMP2JPEGMFCDlg::OnBnClickedButton1() {
    // TODO: 在此添加控件通知处理程序代码
    CSliderCtrl *cs = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
    int pos = cs->GetPos();
    if (multifiles) {
        int errcnt = 0;
        for (vector<CString>::iterator it = fns.begin(); it != fns.end(); it++) {
            CString onefile = *it;
            errcnt += (!convert(onefile, pos));
        }
		fns.clear();
        if (errcnt == 0) {
            TaskDialog(m_hWnd, NULL,
                       L"提示",
                       L"所有文件转换成功",
                       NULL,
                       TDCBF_OK_BUTTON,
                       TD_INFORMATION_ICON,
                       NULL);
        } else {
            CString errmsg = L"";
            errmsg.Format(L"有%d个文件转换失败", errcnt);
            TaskDialog(m_hWnd, NULL,
                       L"提示",
                       errmsg,
                       NULL,
                       TDCBF_OK_BUTTON,
                       TD_ERROR_ICON,
                       NULL);
        }
        multifiles = FALSE;
    } else {
        CFile mFile;
        if (!mFile.Open(filename, CFile::modeRead)) {
            AfxMessageBox(L"File does not exist!");
            mFile.Close();
            return;
        }
        mFile.Close();
        if (convert(filename, pos)) {
            TaskDialog(m_hWnd, NULL,
                       L"提示",
                       L"文件转换成功",
                       NULL,
                       TDCBF_OK_BUTTON,
                       TD_INFORMATION_ICON,
                       NULL);
        }
    }
}


void CBMP2JPEGMFCDlg::OnDropFiles(HDROP hDropInfo) {
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    TCHAR fn[MAX_PATH];
    CString lists = L"";
    UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (UINT idx = 0; idx < nCount; ++idx) {
        DragQueryFile(hDropInfo, idx, fn, MAX_PATH);
        fns.push_back(CString(fn));
        lists += (CString(fn) + L"\r\n");
    }
    multifiles = TRUE;
    GetDlgItem(IDC_EDIT1)->SetWindowText(lists);
    DragFinish(hDropInfo);
    CDialogEx::OnDropFiles(hDropInfo);
}


void CBMP2JPEGMFCDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    CSliderCtrl *cs = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
    int pos = cs->GetPos();
    CString indicator = L"";
    indicator.Format(L"图片质量:%d", pos);
    GetDlgItem(IDC_STATIC2)->SetWindowText(indicator);
}
