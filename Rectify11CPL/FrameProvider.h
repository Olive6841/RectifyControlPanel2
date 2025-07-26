#include <windows.h>
#include <shlobj.h>
#include <propkey.h>
#include <shlwapi.h>
#include <strsafe.h>

#include "Rectify11CPL.h"
#include "CRectifyUtil.h"

class FrameProvider
	: public DirectUI::XProvider
	, public IDUIElementProviderInit
	, public IFrameNotificationClient
	, public IFrameShellViewClient
	, public IObjectWithSite
	, public IServiceProvider
{
public:
	FrameProvider(HINSTANCE hinst, LPCWSTR pszCommonResidToCreate);

	// == Begin IUnknown Impl ==
	IFACEMETHODIMP QueryInterface(REFIID riid, __out void **ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	// == End IUnknown Impl ==

	// == Begin IDUIElementProviderInit Impl ==
	IFACEMETHODIMP SetResourceID(UINT uID);
	IFACEMETHODIMP OptionallyTakeInitialFocus(BOOL *pbTookFocus);
	// == End IDUIElementProviderInit Impl ==

	// == Begin DirectUI::XProvider Impl ==
	IFACEMETHODIMP CreateDUI(DirectUI::IXElementCP *pcp, HWND *phwnd);
	// == End DirectUI::XProvider Impl ==

	// == Begin IFrameNotificationClient Impl ==
	IFACEMETHODIMP LayoutInitialized();
	IFACEMETHODIMP Notify(LPCWSTR pszChangedProp);
	IFACEMETHODIMP OnNavigateAway();
	IFACEMETHODIMP OnInnerElementDestroyed();
	// == End IFrameNotificationClient Impl ==

	// == Begin IFrameShellViewClient Impl ==
	IFACEMETHODIMP OnSelectedItemChanged();
	IFACEMETHODIMP OnSelectionChanged();
	IFACEMETHODIMP OnContentsChanged();
	IFACEMETHODIMP OnFolderChanged();
	// == End IFrameShellViewClient Impl ==

	// == Begin IObjectWithSite Impl ==
	IFACEMETHODIMP SetSite(IUnknown *punkSite);
	IFACEMETHODIMP GetSite(REFIID riid, void **ppv);
	// == End IObjectWithSite Impl ==

	// == Begin IServiceProvider Impl ==
	IFACEMETHODIMP QueryService(REFGUID guidService, REFIID riid, void **ppv);
	// == End IServiceProvider Impl ==

	HRESULT Init();

protected:
	~FrameProvider() override;

private:
	IUnknown *_punkSite;
	HINSTANCE _hinst;
	WCHAR _szResID[MAX_PATH];
	HRESULT _hrInit;
	DirectUI::XResourceProvider *_prprov;
};
