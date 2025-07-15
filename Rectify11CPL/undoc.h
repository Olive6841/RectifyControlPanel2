#pragma once
#include <windows.h>

MIDL_INTERFACE("E1F5EC9F-F933-492B-A242-C3CDAC6ECFBD")
IDUIElementProviderInit: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetResourceID(UINT id) = 0;
    virtual HRESULT STDMETHODCALLTYPE OptionallyTakeInitialFocus(BOOL* result) = 0;
};

MIDL_INTERFACE("1D3293BC-EC01-400F-8179-8DE5D53ABEB7")
IFrameNotificationClient : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE LayoutInitialized() = 0;
    virtual HRESULT STDMETHODCALLTYPE Notify(WORD* param) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnNavigateAway() = 0;
    virtual HRESULT STDMETHODCALLTYPE OnInnerElementDestroyed() = 0;
};

MIDL_INTERFACE("6B353825-C58B-4F03-AEC4-8DE179122661")
IFrameShellViewClient : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnSelectedItemChanged() = 0;
    virtual HRESULT STDMETHODCALLTYPE OnSelectionChanged() = 0;
    virtual HRESULT STDMETHODCALLTYPE OnContentsChanged() = 0;
    virtual HRESULT STDMETHODCALLTYPE OnFolderChanged() = 0;
};

enum CPNAV_LIST : int
{
    CPNAV_LIST_TASKS = 0,
    CPNAV_LIST_SEEALSO = 1,
};

enum CPNAV_CMDTYPE : int
{
    CPNAV_CMDTYPE_NOTIFY = 0,
    CPNAV_CMDTYPE_SHELLEX = 1,
    CPNAV_CMDTYPE_CONTROLPANEL = 2,
};

MIDL_INTERFACE("6FABDA16-031E-47E3-B2A2-2339C05CCB9E")
IMultiObjectElevationFactory : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize(HWND hwnd, GUID param) = 0;
    virtual HRESULT STDMETHODCALLTYPE InitializeNoWaitCursor(HWND hwnd, GUID param) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateElevatedObject(GUID param, GUID param2, void** ppv) = 0;
};