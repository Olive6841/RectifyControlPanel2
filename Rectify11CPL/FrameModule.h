#pragma once

#include "NavigateButton.h"

MIDL_INTERFACE("60CE96BB-B6F9-42DC-B84E-5E5D9C370A6F")
IFrameModule : IUnknown
{
    STDMETHOD(CreateInnerObject)(IUnknown **ppunkInner) PURE;
    STDMETHOD(SetInnerObject)(IUnknown *punkInner) PURE;
    STDMETHOD(GetModuleID)(LPWSTR *ppszModuleID) PURE;
};

class CFrameModule
    : public CElementWithSite
    , public IFrameModule
{
public:
	// == Begin IUnknown Interface ==
    IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv);
	// == End IUnknown Interface ==

    static HRESULT Create(DirectUI::Element *pParent, DWORD *pdwDeferCookie, DirectUI::Element **ppElement);
    static DirectUI::IClassInfo *Class;
    DirectUI::IClassInfo *GetClassInfo() override { return Class; }

    static HRESULT Register();
    static const DirectUI::PropertyInfo *ModuleIDProp;

	// == Begin IFrameModule Interface ==
    IFACEMETHODIMP CreateInnerObject(IUnknown **ppunkInner) PURE;
    IFACEMETHODIMP SetInnerObject(IUnknown *punkInner);
    IFACEMETHODIMP GetModuleID(LPWSTR *ppszModuleID);
	// == End IFrameModule Interface ==
};
