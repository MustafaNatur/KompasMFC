
// KompasMFCDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "KompasMFC.h"
#include "KompasMFCDlg.h"
#include "afxdialogex.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Kompas6API5;
//using namespace KompasAPI5;


KompasObjectPtr pKompasApp5;
//KompasObjectPtr pKompasApp5;

ksPartPtr pPart;// = PartDoc->GetTopPart();
ksDocument3DPtr pDoc;

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CKompasMFCDlg



CKompasMFCDlg::CKompasMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOMPASMFC_DIALOG, pParent)
	, fieldD(0)
	, fieldL(0)
	, fieldd(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKompasMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, fieldD);
	DDX_Text(pDX, IDC_EDIT2, fieldL);
	DDX_Text(pDX, IDC_EDIT1, fieldd);
}

BEGIN_MESSAGE_MAP(CKompasMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_EN_CHANGE(IDC_EDIT3, &CKompasMFCDlg::OnEnChangeEdit3)
//ON_EN_CHANGE(IDC_EDIT3, &CKompasMFCDlg::OnEnChangeEdit3)
//ON_EN_CHANGE(IDC_EDIT1, &CKompasMFCDlg::OnEnChangeEdit1)
//ON_BN_CLICKED(IDOK, &CKompasMFCDlg::OnBnClickedOk)
ON_BN_CLICKED(IDC_BUTTON1, &CKompasMFCDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений CKompasMFCDlg

BOOL CKompasMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	fieldd = 16;
	fieldD = 38;
	fieldL = 65;
	UpdateData(false);
	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CKompasMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CKompasMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CKompasMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKompasMFCDlg::startKompas() {
	CComPtr<IUnknown> pKompasAppUnk = nullptr;
	if (!pKompasApp5)
	{
		// Получаем CLSID для Компас
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &InvAppClsid);
		if (FAILED(hRes)) {
			pKompasApp5 = nullptr;
			return;
		}

		//Проверяем есть ли запущенный экземпляр Компас
	   //если есть получаем IUnknown
		hRes = ::GetActiveObject(InvAppClsid, NULL, &pKompasAppUnk);
		if (FAILED(hRes)) {
			//Приходится запускать Компас самим так как работающего нет
			//Также получаем IUnknown для только что запущенного приложения Компас

			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnk);
			if (FAILED(hRes)) {
				pKompasApp5 = nullptr;
				return;
			}
		}

		//Получаем интерфейс приложения Компас
		hRes = pKompasAppUnk->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes)) {
			return;
		}
	}

	//делаем Компас видимым
	pKompasApp5->Visible = true;
}

void CKompasMFCDlg::buildMain() {
	pDoc = pKompasApp5->Document3D();

	pDoc->Create(false, true);

	pPart = pDoc->GetPart(pTop_Part);

	ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);

	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();

	pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));

	pSketch->Create();

	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

	double point[4][2];
	point[0][0] = fieldd / 2.0;
	point[0][1] = 0;
	point[1][0] = fieldD / 2.0;
	point[1][1] = 0;
	p2DDoc->ksLineSeg(point[0][0], point[0][1], point[1][0], point[1][1], 1);

	point[2][0] = fieldD / 2.0;
	point[2][1] = fieldL;
	p2DDoc->ksLineSeg(point[1][0], point[1][1], point[2][0], point[2][1], 1);

	point[3][0] = fieldd / 2.0;
	point[3][1] = fieldL;
	p2DDoc->ksLineSeg(point[2][0], point[2][1], point[3][0], point[3][1], 1);
	p2DDoc->ksLineSeg(point[0][0], point[0][1], point[3][0], point[3][1], 1);

	p2DDoc->ksLineSeg(0, 0, 0, 100, 3);

	pSketchDef->EndEdit();

	ksEntityPtr pRotate = pPart->NewEntity(o3d_bossRotated);

	ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
	pRotDef->SetSketch(pSketch);
	pRotDef->SetSideParam(TRUE, 360);

	pRotate->Create();

	buildChamfer(fieldd, pRotate, 1.5);
	buildChamfer(fieldD, pRotate, 1.0);




	ksEntityPtr pSketch1 = pPart->NewEntity(o3d_sketch);

	ksSketchDefinitionPtr pSketchDef1 = pSketch1->GetDefinition();

	pSketchDef1->SetPlane(pPart->GetDefaultEntity(o3d_planeXOZ));

	pSketch1->Create();

	p2DDoc = pSketchDef1->BeginEdit();
	double point1[3][2];


	point1[0][0] = -1 * keyL / 2.0;
	point1[0][1] = 0;

	point1[1][0] = keyL / 2.0;
	point1[1][1] = 0;

	p2DDoc->ksLineSeg(point1[0][0], point1[0][1], point1[1][0], point1[1][1], 1);

	point1[2][0] = keyL/2.0;
	point1[2][1] = sqrt(pow(fieldd/2.0, 2) - pow(keyL/2, 2)) + keyH / 2.0;

	p2DDoc->ksLineSeg(point1[1][0], point1[1][1], point1[2][0], point1[2][1], 1);

	p2DDoc->ksLineSeg(point1[2][0], point1[2][1], -1 * point1[2][0], point1[2][1], 1);

	p2DDoc->ksLineSeg(point1[0][0], point1[0][1], -1 * point1[2][0], point1[2][1], 1);

	pSketchDef1->EndEdit();

	ksEntityPtr pExtrude = pPart->NewEntity(o3d_cutExtrusion);

	ksCutExtrusionDefinitionPtr pExDef = pExtrude->GetDefinition();
	pExDef->directionType = dtMiddlePlane;

	pExDef->SetSideParam(true, etThroughAll, 0, 0, false);
	pExDef->SetSketch(pSketch1);
	
	pExtrude->Create();



	ksEntityPtr pSketch2 = pPart->NewEntity(o3d_sketch);

	ksSketchDefinitionPtr pSketchDef2 = pSketch2->GetDefinition();

	pSketchDef2->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));

	pSketch2->Create();

	p2DDoc = pSketchDef2->BeginEdit();

	p2DDoc->ksCircle(0, holeDist, holeD / 2.f, 1);

	pSketchDef2->EndEdit();


	ksEntityPtr pExtrude2 = pPart->NewEntity(o3d_cutExtrusion);

	ksCutExtrusionDefinitionPtr pExDef2 = pExtrude2->GetDefinition();
	pExDef2->directionType = dtMiddlePlane;

	pExDef2->SetSideParam(true, etThroughAll, 0, 0, false);
	pExDef2->SetSketch(pSketch2);

	pExtrude2->Create();

	pDoc->SaveAs("D:\\Assembly\\couplings.m3d");
}
void CKompasMFCDlg::buildChamfer(double detect, ksEntityPtr pEntity, double size) {
	ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);

	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	pChamferDef->SetChamferParam(true, size, size);
	ksEntityCollectionPtr fledges = pPart->EntityCollection(o3d_edge);

	ksEntityCollectionPtr fl = pChamferDef->array();
	fl->Clear();

	for (int i = 0; i < fledges->GetCount(); i++)
	{
		ksEntityPtr ed = fledges->GetByIndex(i);
		ksEdgeDefinitionPtr def = ed->GetDefinition();
		if (def->GetOwnerEntity() == pEntity)
		{
			if (def->IsCircle())
			{
				ksVertexDefinitionPtr p1 = def->GetVertex(true);
				ksVertexDefinitionPtr p2 = def->GetVertex(false);
				double x1, y1, z1;

				p1->GetPoint(&x1, &y1, &z1);
				if (x1 == detect / 2.0) {
					fl->Add(ed);
				}
			}
		}
	}
	pChamfer->Create();
}
void CKompasMFCDlg::buildBoss(){
	pDoc = pKompasApp5->Document3D();

	pDoc->Create(false, true);

	pPart = pDoc->GetPart(pTop_Part);

	ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);

	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();

	pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));

	pSketch->Create();

	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

	p2DDoc->ksCircle(0, 0, fieldd/2.0, 1);
	pSketchDef->EndEdit();
	


	ksEntityPtr pExtrude = pPart->NewEntity(o3d_bossExtrusion);

	ksBossExtrusionDefinitionPtr pExDef = pExtrude->GetDefinition();
	pExDef->directionType = dtNormal;

	pExDef->SetSideParam(true, etBlind, fieldL, 0, false);
	pExDef->SetSketch(pSketch);

	pExtrude->Create();






	ksEntityPtr pPlane = pPart->NewEntity(o3d_planeOffset);
	ksPlaneOffsetDefinitionPtr pPlaneDef = pPlane->GetDefinition();

	pPlaneDef->direction = true;
	pPlaneDef->offset = -1 * fieldd / 2.f;
	pPlaneDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));

	pPlane->Create();


	ksEntityPtr pSketch2 = pPart->NewEntity(o3d_sketch);

	pSketchDef = pSketch2->GetDefinition();

	pSketchDef->SetPlane(pPlane);

	pSketch2->Create();

	p2DDoc = pSketchDef->BeginEdit();

	double point1[6][2];
	point1[0][0] = -keyL/2.0 - keyPosOffset;
	point1[0][1] = keyL / 2.0;

	point1[1][0] = 0 - keyPosOffset;
	point1[1][1] = 0;

	point1[2][0] = point1[0][0];
	point1[2][1] = -point1[0][1];

	point1[3][0] = -keyLL + keyL / 2.0 - keyPosOffset;
	point1[3][1] = keyL / 2.0;

	point1[4][0] = -keyLL - keyPosOffset;
	point1[4][1] = 0;

	point1[5][0] = point1[3][0];
	point1[5][1] = -point1[3][1];

	p2DDoc->ksLineSeg(point1[0][0], point1[0][1], point1[3][0], point1[3][1], 1);
	p2DDoc->ksLineSeg(point1[2][0], point1[2][1], point1[5][0], point1[5][1], 1);

	p2DDoc->ksArcBy3Points(point1[0][0], point1[0][1], point1[1][0], point1[1][1], point1[2][0], point1[2][1], 1);
	p2DDoc->ksArcBy3Points(point1[3][0], point1[3][1], point1[4][0], point1[4][1], point1[5][0], point1[5][1], 1);

	pSketchDef->EndEdit();

	ksEntityPtr pExtrude1 = pPart->NewEntity(o3d_cutExtrusion);

	ksCutExtrusionDefinitionPtr pExDef1 = pExtrude1->GetDefinition();

	pExDef1->SetSideParam(false, etBlind, keyH / 2.0, 0, false);

	pExDef1->SetSketch(pSketch2);

	pExtrude1->Create();

	pDoc->SaveAs("D:\\Assembly\\boss.m3d");






	ksEntityCollectionPtr flFaces = pPart->EntityCollection(o3d_face);

	for (int i = 0; i < flFaces->GetCount(); i++)
	{
		ksEntityPtr face = flFaces->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();


		if (def->GetOwnerEntity() == pExtrude1)
		{
			if (def->IsPlanar())
			{
				face->Putname("BossFaceForKey");
				face->Update();
			}

			if (def->IsCylinder()) {
				ksEdgeCollectionPtr col = def->EdgeCollection();
				for (int k = 0; k < col->GetCount(); k++)
				{
					ksEdgeDefinitionPtr d = col->GetByIndex(k);

					if (d->IsArc())
					{
						ksVertexDefinitionPtr p1 = d->GetVertex(true);
						ksVertexDefinitionPtr p2 = d->GetVertex(false);
						double x1, y1, z1;

						p1->GetPoint(&x1, &y1, &z1);
						if (abs(x1) < keyPosOffset + keyLL/2) {
							face->Putname("BossFaceForKey2");
							face->Update();
						}
					}
				}
			}
		}
	}


}
void CKompasMFCDlg::buildKey() {
	pDoc = pKompasApp5->Document3D();

	pDoc->Create(false, true);

	pPart = pDoc->GetPart(pTop_Part);

	ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);

	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();

	pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));

	pSketch->Create();

	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

	double point1[6][2];
	point1[0][0] = -keyL / 2.0;
	point1[0][1] = keyL / 2.0;

	point1[1][0] = 0;
	point1[1][1] = 0;

	point1[2][0] = point1[0][0];
	point1[2][1] = -point1[0][1];

	point1[3][0] = -keyLL + keyL / 2.0;
	point1[3][1] = keyL / 2.0;

	point1[4][0] = -keyLL;
	point1[4][1] = 0;

	point1[5][0] = point1[3][0];
	point1[5][1] = -point1[3][1];

	p2DDoc->ksLineSeg(point1[0][0], point1[0][1], point1[3][0], point1[3][1], 1);
	p2DDoc->ksLineSeg(point1[2][0], point1[2][1], point1[5][0], point1[5][1], 1);

	p2DDoc->ksArcBy3Points(point1[0][0], point1[0][1], point1[1][0], point1[1][1], point1[2][0], point1[2][1], 1);
	p2DDoc->ksArcBy3Points(point1[3][0], point1[3][1], point1[4][0], point1[4][1], point1[5][0], point1[5][1], 1);

	pSketchDef->EndEdit();


	ksEntityPtr pExtrude1 = pPart->NewEntity(o3d_bossExtrusion);

	ksBossExtrusionDefinitionPtr pExDef1 = pExtrude1->GetDefinition();

	pExDef1->SetSideParam(true, etBlind, keyH, 0, false);

	pExDef1->SetSketch(pSketch);

	pExtrude1->Create();

	pDoc->SaveAs("D:\\Assembly\\key.m3d");






	ksEntityCollectionPtr flFaces = pPart->EntityCollection(o3d_face);

	for (int i = 0; i < flFaces->GetCount(); i++)
	{
		ksEntityPtr face = flFaces->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();


		if (def->GetOwnerEntity() == pExtrude1)
		{
			if (def->IsPlanar())
			{
				ksEdgeCollectionPtr col = def->EdgeCollection();

				for (int k = 0; k < col->GetCount(); k++)
				{
					ksEdgeDefinitionPtr d = col->GetByIndex(k);

					if (d->IsArc()) {
						ksVertexDefinitionPtr p1 = d->GetVertex(true);

						double x1, y1, z1;

						p1->GetPoint(&x1, &y1, &z1);
						if (z1 == keyH) {
							face->Putname("KeyFaceForBoss");
							face->Update();

						}
					}


				}
			}

			if (def->IsCylinder()) {
				ksEdgeCollectionPtr col = def->EdgeCollection();
				for (int k = 0; k < col->GetCount(); k++)
				{
					ksEdgeDefinitionPtr d = col->GetByIndex(k);

					if (d->IsArc())
					{
						ksVertexDefinitionPtr p1 = d->GetVertex(true);
						ksVertexDefinitionPtr p2 = d->GetVertex(false);
						double x1, y1, z1;

						p1->GetPoint(&x1, &y1, &z1);

						int abs1 = abs(x1);
						int abs2 = abs(keyL / 2.0);

						if (abs1 == abs2) {
							face->Putname("KeyFaceForBoss2");
							face->Update();
						}
					}
				}
			}
		}
	}

}
void CKompasMFCDlg::buildBossAssembly() {
	pDoc = pKompasApp5->Document3D();

	pDoc->Create(false, false);

	pPart = pDoc->GetPart(pTop_Part);

	ksPartPtr pBoss, pKey;
	pDoc->SetPartFromFile("D:\\Assembly\\boss.m3d", pPart, true);
	pDoc->SetPartFromFile("D:\\Assembly\\key.m3d", pPart, true);

	pBoss = pDoc->GetPart(0);
	pKey = pDoc->GetPart(1);

	ksEntityCollectionPtr col = pBoss->EntityCollection(o3d_face);
	ksEntityPtr BossFace4Assemly0 = col->GetByName("BossFaceForKey", true, true);
	ksEntityPtr BossFace4Assemly2 = col->GetByName("BossFaceForKey2", true, true);

	col = pKey->EntityCollection(o3d_face);
	ksEntityPtr KeyFace4Assemly0 = col->GetByName("KeyFaceForBoss", true, true);
	ksEntityPtr KeyFace4Assemly2 = col->GetByName("KeyFaceForBoss2", true, true);

	pDoc->AddMateConstraint(mc_Coincidence, BossFace4Assemly0, KeyFace4Assemly0, -1, 1, 0);
	pDoc->AddMateConstraint(mc_Coincidence, BossFace4Assemly2, KeyFace4Assemly2, 1, 1, 0);
	pDoc->RebuildDocument();

	pDoc->SaveAs("D:\\Assembly\\BossAssem.a3d");

}

void CKompasMFCDlg::buildMainAssebly() {
	pDoc = pKompasApp5->Document3D();

	pDoc->Create(false, false);

	pPart = pDoc->GetPart(pTop_Part);

	ksPartPtr pBoss, pKey;
	pDoc->SetPartFromFile("D:\\Assembly\\BossAssem.a3d", pPart, true);
	pDoc->SetPartFromFile("D:\\Assembly\\couplings.m3d", pPart, true);
}
void CKompasMFCDlg::OnBnClickedButton1()
{
	UpdateData(true);
	startKompas();
	buildMain();
	buildBoss();
	buildKey();
	buildBossAssembly();
	buildMainAssebly();
	

}
