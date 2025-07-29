// This class is created by Shell32 CLayoutFolder class. This class "provides" the Element for DirectUI to render in explorer.

#include "pch.h"

#include "FrameProvider.h"

#include <shellapi.h>

#include "CElementWithSite.h"
#include "FrameModule.h"
#include "NavigateButton.h"
#include "SHFusion.h"
#include "DuiUtil.h"
#include "ElevationManager.h"
#include "resource.h"
#include "RectifyMainPage.h"
#include "RectifyThemeCfgPage.h"
#include <map>

FrameProvider::FrameProvider(HINSTANCE hinst, LPCWSTR pszCommonResidToCreate)
	: _punkSite(nullptr)
	, _prprov(nullptr)
	, _hinst(hinst)
	, _hrInit(E_FAIL)
{
	StringCchCopy(_szResID, ARRAYSIZE(_szResID), pszCommonResidToCreate);
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
		DirectUI::UnInitProcessPriv(_hinst);
	}

	DllRelease();
}

// IUnknown Implementation
IFACEMETHODIMP FrameProvider::QueryInterface(_In_ REFIID riid, _Out_ LPVOID *ppv)
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

IFACEMETHODIMP_(ULONG) FrameProvider::AddRef()
{
	return DirectUI::XProvider::AddRef();
}

IFACEMETHODIMP_(ULONG) FrameProvider::Release()
{
	int cRef = _cRef;
	if (_cRef-- == 1)
		delete this;
	return cRef;
}

// XProvider implementation
IFACEMETHODIMP FrameProvider::CreateDUI(DirectUI::IXElementCP *pcp, HWND *phwnd)
{
	UINT_PTR uCookie;
	SHActivateContext(&uCookie);

	HRESULT hr = XProvider::CreateDUI(pcp, phwnd);
	if (SUCCEEDED(hr))
	{
		DirectUI::XProvider::SetHandleEnterKey(true);
	}

	SHDeactivateContext(uCookie);

	return hr;
}

// IDUIElementProviderInit implementation
IFACEMETHODIMP FrameProvider::SetResourceID(UINT uID)
{
	// 1st Parameter: The hinstance of module in which the UIFILE is located.
	// 2nd Parameter: The Resource ID of UIFILE.
	// 3rd Parameter: The resid name of the root page element, this is usually set to "main" when creating an instance of FrameProvider.
	HRESULT hr = DirectUI::XResourceProvider::Create(_hinst, MAKEINTRESOURCE(uID), _szResID, 0, &_prprov);
	if (SUCCEEDED(hr))
	{
		hr = DirectUI::XProvider::Initialize(nullptr, _prprov);
	}

	return hr;
}

IFACEMETHODIMP FrameProvider::OptionallyTakeInitialFocus(BOOL *pbTookFocus)
{
	*pbTookFocus = FALSE;

	DirectUI::HWNDElement *peRoot = (DirectUI::HWNDElement *)DirectUI::XProvider::GetRoot();
	if (peRoot)
	{
		if (peRoot->GetClassInfo()->IsSubclassOf((DirectUI::IClassInfo *)DirectUI::HWNDElement::GetClassInfoPtr()))
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

	return S_OK;
}

// IFrameNotificationClient implementation
IFACEMETHODIMP FrameProvider::LayoutInitialized()
{
	DUI_WalkIUnknownElements(DirectUI::XProvider::GetRoot(), (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, (LPARAM)static_cast<IDUIElementProviderInit *>(this));
	DUI_WalkIUnknownElements(DirectUI::XProvider::GetRoot(), DUI_SendInitializationToUnknown, 0);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::Notify(LPCWSTR pszChangedProp)
{
	DUI_WalkIUnknownElements(DirectUI::XProvider::GetRoot(), DUI_SendNotificationToUnknown, (LPARAM)pszChangedProp);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::OnNavigateAway()
{
	SetHandleEnterKey(false);
	SetDefaultButtonTracking(false);

	// for whatever reason, OnNavigateAway is called twice. Instead of someone
	// at microsoft properly fixing the issue, they just null out some fields in XProvider
	// to prevent destroying a random memory address.

	// This is probably incorrect, but keeping in case its true ^^^

	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendNavigateAwayToUnknown, 0);
	DUI_WalkIUnknownElements(XProvider::GetRoot(), (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, 0);

	_peRoot = nullptr;
	_pe = nullptr;

	return S_OK;
}

IFACEMETHODIMP FrameProvider::OnInnerElementDestroyed()
{
	DUI_ASSERT_EXPR(!_peRoot, "We should have received an OnNavigateAway before the OnInnerElementDestroyed");
	if (_peRoot)
	{
		DUI_WalkIUnknownElements(XProvider::GetRoot(), (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, 0);
		_peRoot = nullptr;
		_pe = nullptr;
	}

	return S_OK;
}

// IFrameShellViewClient implementation
IFACEMETHODIMP FrameProvider::OnSelectedItemChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendSelectedItemChangedToUnknown, 0);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::OnSelectionChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendSelectionChangedToUnknown, 0);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::OnContentsChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendContentsChangedToUnknown, 0);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::OnFolderChanged()
{
	DUI_WalkIUnknownElements(XProvider::GetRoot(), DUI_SendFolderChangedToUnknown, 0);
	return S_OK;
}

// IServiceProvider implementation
IFACEMETHODIMP FrameProvider::QueryService(REFGUID guidService, REFIID riid, void **ppv)
{
	return IUnknown_QueryService(_punkSite, guidService, riid, ppv);
}

// IObjectWithSite implementation
IFACEMETHODIMP FrameProvider::SetSite(IUnknown *punkSite)
{
	IUnknown_Set(&_punkSite, punkSite);
	return S_OK;
}

IFACEMETHODIMP FrameProvider::GetSite(REFIID riid, void **ppv)
{
	*ppv = nullptr;
	return _punkSite ? _punkSite->QueryInterface(riid, ppv) : E_FAIL;
}
