
// KompasMFC.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CKompasMFCApp:
// Сведения о реализации этого класса: KompasMFC.cpp
//

class CKompasMFCApp : public CWinApp
{
public:
	CKompasMFCApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CKompasMFCApp theApp;
