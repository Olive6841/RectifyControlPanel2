#include "pch.h"

#include "CControlPanelNavLinks.h"

CControlPanelNavLinks::CControlPanelNavLinks()
	: _hdpaNavLinks(nullptr)
	, _cRef(1)
{
}

int CALLBACK NavLinksDPA_DeleteCB(CControlPanelNavLink *pLink, void *pData)
{
	if (pLink)
	{
		delete pLink;
	}
	return 1;
}

CControlPanelNavLinks::~CControlPanelNavLinks()
{
	if (_hdpaNavLinks)
	{
		DPA_DestroyCallback(_hdpaNavLinks, (PFNDAENUMCALLBACKCONST)NavLinksDPA_DeleteCB, nullptr);
		_hdpaNavLinks = nullptr;
	}
}

IFACEMETHODIMP CControlPanelNavLinks::QueryInterface(REFIID riid, __out void **ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(CControlPanelNavLinks, IUnknown),
		{ 0 },
	};

	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::Release()
{
	ULONG cRef = InterlockedDecrement(&_cRef);
	if (!cRef && this)
	{
		delete this;
	}

	return cRef;
}

HRESULT CControlPanelNavLinks::AddLinkNotify(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, int nLinkId, CControlPanelNavLink **ppLink)
{
	if (ppLink)
		*ppLink = nullptr;

	CControlPanelNavLink *pLink;
	HRESULT hr = CControlPanelNavLink::Create(list, &pLink);
	if (SUCCEEDED(hr))
	{
		hr = pLink->SetName(hInstance, nNameResId);
		if (SUCCEEDED(hr))
		{
			hr = pLink->SetCommandNotify(nLinkId);
			if (SUCCEEDED(hr))
			{
				hr = Add(pLink);
				if (SUCCEEDED(hr))
				{
					if (ppLink)
						*ppLink = pLink;
					pLink = nullptr;
				}
			}
		}
	}

	if (pLink)
	{
		delete pLink;
	}

	return hr;
}

HRESULT CControlPanelNavLinks::AddLinkShellEx(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, LPCWSTR pszCommand, LPCWSTR pszParams, CControlPanelNavLink **ppLink)
{
	if (ppLink)
		*ppLink = nullptr;

	CControlPanelNavLink *pLink;
	HRESULT hr = CControlPanelNavLink::Create(list, &pLink);
	if (SUCCEEDED(hr))
	{
		hr = pLink->SetName(hInstance, nNameResId);
		if (SUCCEEDED(hr))
		{
			hr = pLink->SetCommandShellEx(pszCommand, pszParams);
			if (SUCCEEDED(hr))
			{
				hr = Add(pLink);
				if (SUCCEEDED(hr))
				{
					if (ppLink)
						*ppLink = pLink;
					pLink = nullptr;
				}
			}
		}
	}

	if (pLink)
	{
		delete pLink;
	}

	return hr;
}

HRESULT CControlPanelNavLinks::AddLinkControlPanel(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, LPCWSTR pszApplet, LPCWSTR pszAppletPage, CControlPanelNavLink **ppLink)
{
	if (ppLink)
		*ppLink = nullptr;

	CControlPanelNavLink *pLink;
	HRESULT hr = CControlPanelNavLink::Create(list, &pLink);
	if (SUCCEEDED(hr))
	{
		hr = pLink->SetName(hInstance, nNameResId);
		if (SUCCEEDED(hr))
		{
			hr = pLink->SetCommandControlPanel(pszApplet, pszAppletPage, list == CPNAV_LIST_SEEALSO);
			if (SUCCEEDED(hr))
			{
				hr = Add(pLink);
				if (SUCCEEDED(hr))
				{
					if (ppLink)
						*ppLink = pLink;
					pLink = nullptr;
				}
			}
		}
	}

	if (pLink)
	{
		delete pLink;
	}

	return hr;
}

HRESULT CControlPanelNavLinks::Add(CControlPanelNavLink *pLink)
{
	HRESULT hr = S_OK;

	if (!_hdpaNavLinks)
	{
		_hdpaNavLinks = DPA_Create(10);
		if (!_hdpaNavLinks)
		{
			return E_OUTOFMEMORY;
		}
	}

	if (DPA_AppendPtr(_hdpaNavLinks, pLink) == -1)
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}