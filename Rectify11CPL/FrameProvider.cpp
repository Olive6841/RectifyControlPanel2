// This class is created by Shell32 CLayoutFolder class. This class "provides" the Element for DirectUI to render in explorer.

#include <windows.h>
#include <shlobj.h>
#include <propkey.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <shellapi.h>

#include "Rectify11CPL.h"
#include "CElementWithSite.h"
#include "FrameModule.h"
#include "NavigateButton.h"
#include "SHFusion.h"
#include "DuiUtil.h"
#include "ElevationManager.h"
#include "FrameProvider.h"
#include "resource.h"
#include "RectifyMainPage.h"
#include "RectifyThemeCfgPage.h"
#include <map>

HRESULT WINAPI DUIFramework_InitDUI()
{
	HRESULT hr = InitProcessPriv(DUI_VERSION, g_hInst, true, true, true);
	if (SUCCEEDED(hr))
	{
		hr = InitThread(DirectUI::TSM_IMMERSIVE);
		if (FAILED(hr))
		{
			UnInitProcessPriv(g_hInst);
		}
	}

	return hr;
}

FrameProvider::FrameProvider(HINSTANCE hinst, LPCWSTR pszCommonResidToCreate)
	: _punkSite(NULL)
	, _prprov(NULL)
	, _hinst(g_hInst)
	, _hrInit(E_FAIL)
{
	StringCchCopy(_szResID, ARRAYSIZE(_szResID), L"main");
	DllAddRef();
}

HRESULT FrameProvider::Init()
{
	HRESULT hr = DUIFramework_InitDUI();
	_hrInit = hr;

	if (FAILED(hr))
	{
		return hr;
	}

	if (SUCCEEDED(CElementWithSite::Register()))
	{
		hr = CNavigateButton::Register();
		if (SUCCEEDED(hr))
		{
			hr = CFrameModule::Register();
			if (SUCCEEDED(hr))
			{
				hr = S_OK;
			}
		}
	}

	return hr;
}

FrameProvider::~FrameProvider()
{
	SafeRelease(&_punkSite);

	if (_prprov)
		_prprov->DestroyCP();

	if (SUCCEEDED(_hrInit))
	{
		DirectUI::UnInitThread();
		DirectUI::UnInitProcessPriv(g_hInst);
	}

	DllRelease();
}

// IUnknown Implementation
HRESULT FrameProvider::QueryInterface(REFIID riid, __out void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(FrameProvider, IDUIElementProviderInit),
		QITABENT(FrameProvider, IFrameNotificationClient),
		QITABENT(FrameProvider, IFrameShellViewClient),
		QITABENT(FrameProvider, IObjectWithSite),
		QITABENT(FrameProvider, IServiceProvider),
		{ 0 },
	};

	HRESULT hr = QISearch(this, qit, riid, ppv);
	if (FAILED(hr))
	{
		hr = DirectUI::XProvider::QueryInterface(riid, ppv);
	}

	return hr;
}

ULONG FrameProvider::AddRef()
{
	return DirectUI::XProvider::AddRef();
}

ULONG FrameProvider::Release()
{
	int cRef = _cRef;
	if (_cRef-- == 1)
		delete this;
	return cRef;
}

// XProvider implementation
HRESULT FrameProvider::CreateDUI(DirectUI::IXElementCP *pcp, HWND *phwnd)
{
	UINT_PTR uCookie;
	SHActivateContext(&uCookie);

	HRESULT hr = XProvider::CreateDUI(pcp, phwnd);
	if (SUCCEEDED(hr))
	{
		DirectUI::XProvider::SetHandleEnterKey(true);
	}
#ifdef _DEBUG
	else
	{
		WCHAR buffer[200];
		if (hr == 0x800403ED)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Bad markup.");
		}
		else if (hr == 0x800403EF)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: A required property is missing. (are you sure that resid=main exists?)");
		}
		else if (hr == 0x800403F1)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Invaild property value");
		}
		else if (hr == 0x8004005A)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Probaby can't find the UIFILE?");
		}
		else if (hr == 0x800403EE)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Unregistered element");
		}
		else if (hr == 0x800403F0)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Something is not found");
		}
		else if (hr == 0x800403F0)
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Something is not found");
		}
		else if (hr == E_FAIL)
		{
			if (this->_pprovCP)
			{
				swprintf(buffer, 200, L"Failed to create DirectUI parser: E_FAIL. This is most likely caused by duires.dll missing in System32. Rerun the latest version of the Rectify11 installer.");
			}
			else
			{
				swprintf(buffer, 200, L"Failed to create DirectUI parser: E_FAIL as XProviderCP is 0.");
			}
		}
		else
		{
			swprintf(buffer, 200, L"Failed to create DirectUI parser: Error %X", hr);
		}

		MessageBox(NULL, buffer, TEXT("FrameProvider::CreateDUI failed"), MB_ICONERROR);
	}
#endif

	if (uCookie)
		DeactivateActCtx(0, uCookie);

	return hr;
}

// IDUIElementProviderInit implementation
HRESULT STDMETHODCALLTYPE FrameProvider::SetResourceID(UINT uID)
{
	// 1st Parameter: The hinstance of module in which the UIFILE is located
	// 2nd Parameter: The Resource ID of UIFILE
	// 3rd Parameter: The resid name of the root page element, most often just "main"
	HRESULT hr = DirectUI::XResourceProvider::Create(_hinst, MAKEINTRESOURCE(uID), _szResID, 0, &_prprov);
	if (SUCCEEDED(hr))
	{
		hr = DirectUI::XProvider::Initialize(NULL, _prprov);
#if _DEBUG
		if (!SUCCEEDED(hr))
		{
			WCHAR szResource[40] = { 0 };
			swprintf(szResource, 40, L"%d", uID);

			MessageBox(NULL, szResource, TEXT("FrameProvider::SetResourceId - failed to initialize XProvider class"), MB_ICONERROR);
		}
	}
	else
	{
		WCHAR szResource[40] = { 0 };
		swprintf(szResource, 40, L"%d", uID);

		MessageBox(NULL, szResource, TEXT("FrameProvider::SetResourceId - failed to create xprovider"), MB_ICONERROR);
#endif
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE FrameProvider::OptionallyTakeInitialFocus(BOOL *pbTookFocus)
{
	*pbTookFocus = FALSE;

#if 0
	DirectUI::HWNDElement *peRoot = (DirectUI::HWNDElement *)DirectUI::XProvider::GetRoot();
	if (peRoot)
	{
		if (peRoot->GetClassInfoW()->IsSubclassOf((DirectUI::IClassInfo *)DirectUI::HWNDElement::GetClassInfoPtr()))
		{
			HWND hwndRoot = peRoot->GetHWND();
			HWND hwndFocus = GetFocus();
			if (!SHIsChildOrSelf(hwndRoot, hwndFocus))
			{
				*pbTookFocus = TRUE;
			}
		}

		if (!*pbTookFocus && SUCCEEDED(DUIFramework_SetFocusByFocusIndicator(peRoot)))
		{
			*pbTookFocus = TRUE;
		}
	}
#endif

	return S_OK;
}

// IFrameNotificationClient implementation
HRESULT STDMETHODCALLTYPE FrameProvider::LayoutInitialized()
{
	HRESULT hr = themetool_init();

#ifdef _DEBUG	
	if (hr != S_OK && hr != HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED))
	{
		MessageBox(NULL, TEXT("Failed to initialize SecureUXTheme ThemeTool. Theme information will not be loaded. This may be due to the lack of the ThemeDll.dll in C:\\Windows\\Rectify11\\RectifyControlPanel"), TEXT("FrameProvider::LayoutInitialized"), MB_ICONERROR);
	}
#endif

	Element* root = XProvider::GetRoot();

	// Call initialization function as DirectUI does not provide an "OnLoad" method, unlike WPF/Winforms
	// This is a bit hacky as Microsoft treats elements as COM objects, and instead iterates through each element, calling
	// the appropriate method.
	if (root->FindDescendent(StrToID((LPCWSTR)L"MainPageElem")) != NULL)
	{
		RectifyMainPage* page = (RectifyMainPage*)root->FindDescendent(StrToID((LPCWSTR)L"MainPageElem"));
		page->SetSite(_punkSite);
		page->LayoutInitialized();
	}
	else if (root->FindDescendent(StrToID((LPCWSTR)L"ThemePageElem")) != NULL)
	{
		RectifyThemeCfgPage* page = (RectifyThemeCfgPage*)root->FindDescendent(StrToID((LPCWSTR)L"ThemePageElem"));
		page->SetSite(_punkSite);
		page->LayoutInitialized();
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE FrameProvider::Notify(LPCWSTR pszChangedProp)
{
#if 0 // FIXME!! FIXME!!: Figure out why this wont send notifications to CControlPanelPage properly 
	Element *peRoot = XProvider::GetRoot();
	if (peRoot)
	{
		//MessageBox(NULL, L"peRoot is valid!", L"FrameProvider::Notify", MB_OK);
		DUI_WalkIUnknownElements(peRoot, DUI_SendNotificationToUnknown, (LPARAM)pszChangedProp);
	}
	return S_OK;
#else
	if (!StrCmpCW((LPCWSTR)pszChangedProp, L"SettingsChanged"))
	{
		//This is invoked when the UI is refreshed!
	}

	if (!StrCmpCW((LPCWSTR)pszChangedProp, L"SearchText"))
	{
		//Sent when search text modified/added
		WCHAR value[264] = { 0 };
		WCHAR path[48];
		GUID IID_IFrameManager = {};
		GUID SID_STopLevelBrowser = {};
		HRESULT hr = CLSIDFromString(L"{4c96be40-915c-11cf-99d3-00aa004ae837}", (LPCLSID)&SID_STopLevelBrowser);
		if (SUCCEEDED(hr))
		{
			hr = CLSIDFromString(L"{31e4fa78-02b4-419f-9430-7b7585237c77}", (LPCLSID)&IID_IFrameManager);
			if (SUCCEEDED(hr))
			{
				// find explorer property bag
				IPropertyBag* bag = NULL;
				HRESULT hr = IUnknown_QueryService(_punkSite, IID_IFrameManager, IID_IPropertyBag, (LPVOID*)&bag);
				if (SUCCEEDED(hr))
				{
					// read search text
					if (SUCCEEDED(PSPropertyBag_ReadStr(bag, L"SearchText", value, 260)) && value[0])
					{
						if (SUCCEEDED(StringCchPrintfW(path, 41, L"::%s", L"{26ee0668-a00a-44d7-9371-beb064c98683}")))
						{
							LPITEMIDLIST pidlist;
							if (SUCCEEDED(SHParseDisplayName(path, NULL, &pidlist, 0, NULL)))
							{
								IShellBrowser* browser = NULL;
								if (SUCCEEDED(IUnknown_QueryService(_punkSite, SID_STopLevelBrowser, IID_IShellBrowser, (LPVOID*)&browser)))
								{
									// navigate to search text
									hr = browser->BrowseObject(pidlist, SBSP_ACTIVATE_NOFOCUS | SBSP_SAMEBROWSER | SBSP_CREATENOHISTORY);
									browser->Release();
								}
							}
							ILFree(pidlist);
						}
					}
				}
			}
		}
	}
	return 0;
#endif
}

HRESULT STDMETHODCALLTYPE FrameProvider::OnNavigateAway()
{
	SetHandleEnterKey(false);
	SetDefaultButtonTracking(false);

	// for whatever reason, OnNavigateAway is called twice. Instead of someone
	// at microsoft properly fixing the issue, they just null out some fields in XProvider
	// to prevent destroying a random memory address.

	// This is probably incorrect, but keeping in case its true ^^^

	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendNavigateAwayToUnknown, 0);
	DUI_WalkIUnknownElements(XProvider::GetRoot(), (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, 0);

	_peRoot = NULL;
	_pe = NULL;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE FrameProvider::OnInnerElementDestroyed()
{
#ifdef _DEBUG
	DUI_ASSERT_EXPR(!_peRoot, "We should have received an OnNavigateAway before the OnInnerElementDestroyed")
#endif
	if (_peRoot)
	{
		DUI_WalkIUnknownElements(XProvider::GetRoot(), (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, 0);
		_peRoot = NULL;
		_pe = NULL;
	}

	return S_OK;
}

// IFrameShellViewClient implementation
HRESULT STDMETHODCALLTYPE FrameProvider::OnSelectedItemChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendSelectedItemChangedToUnknown, 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE FrameProvider::OnSelectionChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendSelectionChangedToUnknown, 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE FrameProvider::OnContentsChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendContentsChangedToUnknown, 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE FrameProvider::OnFolderChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendFolderChangedToUnknown, 0);
	return S_OK;
}

// IServiceProvider implementation
HRESULT STDMETHODCALLTYPE FrameProvider::QueryService(
	REFGUID guidService,
	REFIID riid,
	void **ppv)
{
	return IUnknown_QueryService(_punkSite, guidService, riid, ppv);
}

// IObjectWithSite implementation
HRESULT STDMETHODCALLTYPE FrameProvider::SetSite(IUnknown *punkSite)
{
	IUnknown_Set(&_punkSite, punkSite);
	return S_OK;
}

HRESULT FrameProvider::GetSite(REFIID riid, void** ppvSite)
{
	if (_punkSite == NULL)
	{
		return E_FAIL;
	}

	return _punkSite->QueryInterface(riid, ppvSite);
}
