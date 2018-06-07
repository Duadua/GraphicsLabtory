
// glb_lightstudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_lightstudio.h"
#include "glb_lightstudioDlg.h"
#include "afxdialogex.h"

#include "glb_lightstudio_core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGLBLightStudioDlg dialog



CGLBLightStudioDlg::CGLBLightStudioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGLBLightStudioDlg::IDD, pParent)
    , m_ProjectXML(NULL)
{
    // Using GLB icon
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_GLB);
}

void CGLBLightStudioDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    if (m_ProjectXML)
    {
        delete m_ProjectXML;
        m_ProjectXML = NULL;
    }
    DDX_Control(pDX, IDC_OUTLINE_LIST, m_OutlineList);
}

BEGIN_MESSAGE_MAP(CGLBLightStudioDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
    ON_WM_CLOSE()
    ON_COMMAND(ID_FILE_NEW, &CGLBLightStudioDlg::OnFileNew)
    ON_COMMAND(ID_FILE_SAVE, &CGLBLightStudioDlg::OnFileSave)
    ON_COMMAND(ID_ADD_SCENE, &CGLBLightStudioDlg::OnAddScene)
    ON_COMMAND(ID_ADD_LIGHT, &CGLBLightStudioDlg::OnAddLight)
END_MESSAGE_MAP()


// CGLBLightStudioDlg message handlers

BOOL CGLBLightStudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    glb::app::AppConfig config;
    config.wnd = GetDlgItem(IDC_VIEW)->GetSafeHwnd();
    RECT rect;
    GetDlgItem(IDC_VIEW)->GetClientRect(&rect);
    config.screen_width = rect.right - rect.left;
    config.screen_height = rect.bottom - rect.top;
    config.shadow_map_width = 32;
    config.shadow_map_height = 32;
    if (!glb::app::Application::Initialize(ApplicationCore::Create, AfxGetInstanceHandle(), config))
    {
        ::MessageBox(NULL, L"Initliaze GLB library failed", L"ERROR", MB_OK);
        exit(0);
    }

    // Hide Viewport first
    GetDlgItem(IDC_VIEW)->ShowWindow(SW_HIDE);

    // Disable menu
    GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_ADD_LIGHT, MF_DISABLED);

    // Hide control
    m_OutlineList.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGLBLightStudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGLBLightStudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGLBLightStudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGLBLightStudioDlg::OnKickIdle()
{
    glb::app::Application::Update();
}

void CGLBLightStudioDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    glb::app::Application::Destroy();
    CDialogEx::OnClose();
}


void CGLBLightStudioDlg::OnFileNew()
{
    // If project active
    if (m_ProjectXML)
    {
        if (::MessageBox(NULL, L"Do you want to save old project?", L"Info", MB_OKCANCEL)) {
            OnFileSave();
        }

        if (m_ProjectXML) {
            delete m_ProjectXML;
            m_ProjectXML = NULL;
        }
    }

    // Create new project
    m_ProjectXML = new TiXmlDocument();

    TiXmlElement* root = new TiXmlElement("GLB_LIGHT_STUDIO");
    root->SetAttribute("Author", "GLB");
    m_ProjectXML->LinkEndChild(root);

    // Display view
    GetDlgItem(IDC_VIEW)->ShowWindow(SW_SHOW);

    // Enable Save menu
    GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_ENABLED);
    GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_ENABLED);
    GetMenu()->EnableMenuItem(ID_ADD_LIGHT, MF_ENABLED);

    // Display control
    m_OutlineList.ShowWindow(SW_SHOW);
}


void CGLBLightStudioDlg::OnFileSave()
{
    if (!strcmp(m_ProjectXML->Value(), ""))
    {
        TCHAR szFilter[] = L"XML File(*.xml)|*.txt|All Files(*.*)|*.*||";
        CFileDialog fileDlg(FALSE, L"txt", L"Untitled.xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

        if (IDOK == fileDlg.DoModal())
        {
            CString filePath = fileDlg.GetPathName();

            char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
            memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
            wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

            m_ProjectXML->SetValue(pcstr);
            m_ProjectXML->SaveFile();

            delete[] pcstr;
            pcstr = NULL;
        }
    }
    else
    {
        m_ProjectXML->SaveFile();
    }
}


void CGLBLightStudioDlg::OnAddScene()
{
    // TODO: Add your command handler code here
}


void CGLBLightStudioDlg::OnAddLight()
{
    // TODO: Add your command handler code here
}
