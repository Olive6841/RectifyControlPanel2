#pragma once

#include "CElementWithSite.h"

class CControlPanelPage
    : public CElementWithSite
    , public IFrameNotificationClient
{
public:
    CControlPanelPage();

	// == Begin IFrameNotificationClient Interface ==
    IFACEMETHODIMP LayoutInitialized();
    IFACEMETHODIMP Notify(LPCWSTR pszChangedProp);
    IFACEMETHODIMP OnNavigateAway();
    IFACEMETHODIMP OnInnerElementDestroyed();
	// == End IFrameNotificationClient Interface ==

    virtual void LogCPLPerfTrackStopEvent();

protected:
    HRESULT RegisterWindowReadyEvent();
    HRESULT UnRegisterWindowReadyEvent();

    virtual HRESULT v_GetSearchTargetServices(REFIID riid, void **ppv);

private:
    DWORD _dwProffered;
    DWORD _dwEventToken;
    IDispatch *_pDispatchView;
    IDispatch *_pPerftrackInfo;
};

class CControlPanelPerfTrackInfo : public IDispatch
{
public:
    static HRESULT Create(CControlPanelPage *pCControlPanelPage, IDispatch **ppDisp);

	// == Begin IUnknown Interface ==
    IFACEMETHODIMP QueryInterface(REFIID, void **);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
	// == End IUnknown Interface ==

	// == Begin IDispatch Interface ==
    IFACEMETHODIMP GetTypeInfoCount(UINT *pctinfo);
    IFACEMETHODIMP GetTypeInfo(UINT itinfo, LCID lcid,  ITypeInfo **pptinfo);
    IFACEMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
    IFACEMETHODIMP Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdp, VARIANT *pvarResult, EXCEPINFO *pxi, UINT *puArgErr);
	// == End IDispatch Interface ==

private:
    CControlPanelPerfTrackInfo(CControlPanelPage *pCControlPanelPage)
        : _pPage(pCControlPanelPage)
        , _cRef(1)
    {
    }

    CControlPanelPage *_pPage;
    ULONG _cRef;
};
