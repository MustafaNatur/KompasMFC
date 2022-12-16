
// KompasMFCDlg.h: файл заголовка
//

#pragma once


#include "math.h"
#include "D:\SDK\Include\ksConstants.h"
#include "D:\SDK\Include\ksConstants3D.h"

#import "D:\bin\kAPI5.tlb"
using namespace Kompas6API5;

// Диалоговое окно CKompasMFCDlg
class CKompasMFCDlg : public CDialogEx
{
// Создание
public:
	CKompasMFCDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOMPASMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnEnChangeEdit3();
//	afx_msg void OnEnChangeEdit3();
//	afx_msg void OnEnChangeEdit1();
	double fieldD;
	double fieldL;
	double fieldd;
	void startKompas();
	void buildMain();
	void buildChamfer(double detect, ksEntityPtr pEntity, double size);
	void buildBoss();
	void buildKey();
	void buildBossAssembly();
	void buildMainAssebly();
//	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

	double keyL = 6.0;
	double keyH = 3.0;
	double keyLL = 15.0;
	double keyPosOffset = 5.0;

	double holeD = 10.0;
	double holeDist = 15.0;

	double offsetInAssembly = 12.0;
};


