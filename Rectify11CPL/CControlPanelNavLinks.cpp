#include "Rectify11CPL.h"
#include "CControlPanelNavLinks.h"
#include "CControlPanelNavLink.h"
#include "CControlPanelNavLinkCommand.h"

CControlPanelNavLinks::CControlPanelNavLinks()
	: _hdpaNavLinks(NULL)
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
	if (!_hdpaNavLinks == NULL)
	{
		DPA_DestroyCallback(_hdpaNavLinks, (PFNDAENUMCALLBACKCONST)NavLinksDPA_DeleteCB, NULL);
		_hdpaNavLinks = NULL;
	}
}

IFACEMETHODIMP CControlPanelNavLinks::QueryInterface(REFIID riid, __out void** ppv)
{
	*ppv = NULL;
	if (riid == IID_IUnknown)
	{
		*ppv = (IUnknown*)this;
		return S_OK;
	}
	return E_NOINTERFACE;
}

IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::Release()
{
	ULONG ref = InterlockedDecrement(&_cRef);
	if (ref == 0)
	{
		delete this;
	}
	return ref;
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
			pLink->GetCommand()->SetType(CPNAV_CMDTYPE_NOTIFY);
			if (nLinkId <= 0)
			{
				hr = E_INVALIDARG;
			}
			else
			{
				pLink->GetCommand()->SetId(nLinkId);
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
		*ppLink = 0;

	CControlPanelNavLink *pLink;
	HRESULT hr = CControlPanelNavLink::Create(list, &pLink);
	if (hr >= 0)
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
		if (!DPA_Create(10))
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (DPA_AppendPtr(_hdpaNavLinks, pLink) == -1)
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}