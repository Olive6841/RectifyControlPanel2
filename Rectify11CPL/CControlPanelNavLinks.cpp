#include "Rectify11CPL.h"
#include "CControlPanelNavLinks.h"
#include "CControlPanelNavLink.h"
#include "CControlPanelNavLinkCommand.h"

CControlPanelNavLinks::CControlPanelNavLinks()
{
	m_dpaList = NULL;
	m_refCount = 1;
}

void NavLinksDPA_DeleteCB(CControlPanelNavLink* p, void* pData)
{
	delete p;
}

CControlPanelNavLinks::~CControlPanelNavLinks()
{
	if (this->m_dpaList != NULL) {
		DPA_DestroyCallback(this->m_dpaList, (PFNDAENUMCALLBACKCONST)NavLinksDPA_DeleteCB, NULL);
		this->m_dpaList = NULL;
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
	return InterlockedIncrement(&m_refCount);
}
IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::Release()
{
	ULONG ref = InterlockedDecrement(&m_refCount);
	if (ref == 0)
	{
		delete this;
	}
	return ref;
}
HRESULT CControlPanelNavLinks::AddLinkShellEx(LPCWSTR name, LPCWSTR file, LPCWSTR arguments, CPNAV_LIST DisplayType, HICON icon)
{
	CControlPanelNavLink* link = NULL;
	HRESULT hr = CControlPanelNavLink::Create(DisplayType, &link);
	if (SUCCEEDED(hr))
	{
		link->SetName(name);

		if (icon != NULL)
		{
			link->_hIcon = icon;
		}

		link->_cmd._cmdType = CPNAV_CMDTYPE_SHELLEX;
		SHStrDupW(file, &link->_cmd._pszAppletOrCommand);
		SHStrDupW(arguments, &link->_cmd._pszAppletPageOrCommandParams);
		return Add(link);
	}
	else
	{
		return hr;
	}
}
HRESULT CControlPanelNavLinks::AddLinkControlPanel(LPCWSTR name, LPCWSTR path, LPCWSTR arguments, CPNAV_LIST DisplayType, HICON icon)
{
	CControlPanelNavLink* link = NULL;
	HRESULT hr = CControlPanelNavLink::Create(DisplayType, &link);
	if (SUCCEEDED(hr))
	{
		link->SetName(name);

		if (icon != NULL)
		{
			link->_hIcon = icon;
		}

		link->_cmd._cmdType = CPNAV_CMDTYPE_CONTROLPANEL;
		SHStrDupW(path, &link->_cmd._pszAppletOrCommand);
		SHStrDupW(arguments, &link->_cmd._pszAppletPageOrCommandParams);
		return Add(link);
	}
	else
	{
		return hr;
	}
}

HRESULT CControlPanelNavLinks::Add(CControlPanelNavLink* link)
{
	if (m_dpaList == NULL)
	{
		m_dpaList = DPA_Create(10);
		if (m_dpaList == NULL)
		{
			return E_OUTOFMEMORY;
		}
	}
	if (DPA_InsertPtr(m_dpaList, 0x7fffffff, link) != -1)
	{
		return S_OK;
	}
	else
	{
		return E_OUTOFMEMORY;
	}
}