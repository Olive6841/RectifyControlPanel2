#include "Rectify11CPL.h"
#include "CControlPanelNavLinkCommand.h"
#include "CControlPanelNavLink.h"

CControlPanelNavLink::CControlPanelNavLink(CPNAV_LIST list)
	: _list(list)
	, _pszName(NULL)
	, _pszNameAcc(NULL)
	, _hIcon(NULL)
	, _fDisabled(false)
{
}

CControlPanelNavLink::~CControlPanelNavLink()
{
	CoTaskMemFree(_pszName);
	CoTaskMemFree(_pszNameAcc);
	DestroyIcon(_hIcon);
}

HRESULT CControlPanelNavLink::Create(CPNAV_LIST list, CControlPanelNavLink **ppLink)
{
	*ppLink = new (std::nothrow) CControlPanelNavLink(list);
	return *ppLink != nullptr ? S_OK : E_OUTOFMEMORY;
}

HRESULT CControlPanelNavLink::SetName(HINSTANCE hInstance, UINT nResId)
{
	WCHAR szName[MAX_PATH];
	HRESULT hr = LoadString(hInstance, nResId, szName, ARRAYSIZE(szName)) != 0 ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		hr = SetName(szName);
	}

	return hr;
}

HRESULT CControlPanelNavLink::SetName(LPCWSTR pszName)
{
	return SHStrDup(pszName, &_pszName);
}

HRESULT CControlPanelNavLink::SetNameAcc(HINSTANCE hInstance, UINT nResId)
{
	return E_NOTIMPL;
}

HRESULT CControlPanelNavLink::SetNameAcc(LPCWSTR pszNameAcc)
{
	return SHStrDup(pszNameAcc, &_pszNameAcc);
}

HRESULT CControlPanelNavLink::SetIcon(HICON hIcon)
{
	_hIcon = hIcon;
	return S_OK;
}

HRESULT CControlPanelNavLink::SetIcon(HINSTANCE hInstance, int nIconId)
{
	SHSTOCKICONINFO sii = { sizeof(sii) };
	HRESULT hr = SHGetStockIconInfo(SHSTOCKICONID(nIconId), (SHGFI_SMALLICON | SHGFI_ICON), &sii);
	if (SUCCEEDED(hr))
	{
		hr = SetIcon(sii.hIcon);
	}

	return hr;
}