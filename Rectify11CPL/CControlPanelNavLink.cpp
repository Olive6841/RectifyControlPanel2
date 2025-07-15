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

void CControlPanelNavLink::SetName(LPCWSTR pszName)
{
	SHStrDupW(pszName, &_pszName);
}