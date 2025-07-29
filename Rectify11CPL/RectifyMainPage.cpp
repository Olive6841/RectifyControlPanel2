#include "pch.h"

#include "RectifyMainPage.h"

#include "CRectifyUtil.h"
#include "ElevationManager.h"
#include "CControlPanelNavLinks.h"
#include "DuiUtil.h"
#include <new>

IClassInfo *RectifyMainPage::Class = NULL;

RectifyMainPage::RectifyMainPage()
	: _punkSite(nullptr)
	, HasAdmin(false)
	, RectifyUtil(nullptr)
	, initializing(true)
{
}

RectifyMainPage::~RectifyMainPage()
{

}

HRESULT RectifyMainPage::Register()
{
	HRESULT hr = CElementWithSite::Register();
	if (SUCCEEDED(hr))
	{
		hr = DirectUI::ClassInfo<RectifyMainPage, CElementWithSite>::RegisterGlobal(g_hInst, L"RectifyMainPage", nullptr, 0);
	}

	return hr;
}

HRESULT RectifyMainPage::Create(Element *pParent, DWORD *pdwDeferCookie, Element **ppElement)
{
	return CreateAndInit<RectifyMainPage, int>(0, pParent, pdwDeferCookie, ppElement);
}

IClassInfo *RectifyMainPage::GetClassInfo()
{
	return Class;
}

void RectifyMainPage::OnEvent(Event *iev)
{
	if (iev->nStage != GMF_BUBBLED)
		return;
	if (!iev->fHandled)
		Element::OnEvent(iev);

	if (initializing) return;
	if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"Link_EnableAdmin"))
	{
		if (iev->uidType == TouchButton::Click)
		{
			IRectifyUtil *utility = ElevationManager::Initialize(GetMainHwnd());
			TouchCheckBox *MicaForEveryoneCheckbox = (TouchCheckBox *)FindDescendent(StrToID((LPCWSTR)L"MicaChk"));
			TouchCheckBox *TabbedCheckbox = (TouchCheckBox *)FindDescendent(StrToID((LPCWSTR)L"TabChk"));
			TouchButton *ThemetoolInstall = (TouchButton *)FindDescendent(StrToID((LPCWSTR)L"ThemetoolInstall"));
			if (utility != NULL)
			{
				// Destroy old class
				if (RectifyUtil != NULL)
				{
					RectifyUtil->Release();
				}

				RectifyUtil = utility;
				HasAdmin = TRUE;

				DWORD dwDefer = 0;
				Element::StartDefer(&dwDefer);
				iev->peTarget->SetLayoutPos(-3);
				iev->peTarget->SetVisible(FALSE);
				ThemetoolInstall->SetEnabled(TRUE);

				MicaForEveryoneCheckbox->SetEnabled(TRUE);
				if (MicaForEveryoneCheckbox->GetCheckedState() != CSF_Unchecked)
					TabbedCheckbox->SetEnabled(TRUE);


				CCRadioButton *Win11DefaultMenus = (CCRadioButton *)FindDescendent(StrToID((LPCWSTR)L"Win11DefaultMenus"));
				CCRadioButton *NilesoftSmall = (CCRadioButton *)FindDescendent(StrToID((LPCWSTR)L"NilesoftSmall"));
				CCRadioButton *NilesoftFull = (CCRadioButton *)FindDescendent(StrToID((LPCWSTR)L"NilesoftFull"));
				CCRadioButton *Classic = (CCRadioButton *)FindDescendent(StrToID((LPCWSTR)L"Classic"));
				CCRadioButton *ClassicTransparent = (CCRadioButton *)FindDescendent(StrToID((LPCWSTR)L"ClassicTransparent"));

				CCRadioButton *Options[] =
				{
					Win11DefaultMenus,
					NilesoftSmall,
					NilesoftFull,
					Classic,
					ClassicTransparent
				};

				for (size_t i = 0; i < 5; i++)
				{
					Options[i]->SetEnabled(TRUE);
				}

				Element::EndDefer(dwDefer);
			}
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"BtnRestartExplorer"))
	{
		if (iev->uidType == TouchButton::Click)
		{
			CRectifyUtil::RestartExplorer();

			// hide restart explorer button
			iev->peTarget->SetLayoutPos(-3);
			iev->peTarget->SetVisible(FALSE);
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"ThemeCmb"))
	{
		if (iev->uidType == Combobox::SelectionChange)
		{
			TouchCheckBox *MicaForEveryoneCheckbox = (TouchCheckBox *)FindDescendent(StrToID((LPCWSTR)L"MicaChk"));
			TouchCheckBox *TabbedCheckbox = (TouchCheckBox *)FindDescendent(StrToID((LPCWSTR)L"TabChk"));
			int selection = ((Combobox *)iev->peTarget)->GetSelection();

			ULONG apply_flags = 0;

			// load appy flags
			HKEY Rectify11;
			if (RegCreateKey(HKEY_CURRENT_USER, Rectify11PrefsKey, &Rectify11))
			{
				SHOW_ERROR("Failed to create Rectify11Prefs key");
				return;
			}

			DWORD size = 4;

			DWORD IgnoreBgVal = 0;
			DWORD IgnoreCursorsVal = 0;
			DWORD IgnoreIconsVal = 0;
			DWORD IgnoreColorsVal = 0;
			DWORD IgnoreSoundsVal = 0;
			DWORD IgnoreScreensaversVal = 0;

			RegQueryValueExW(Rectify11, L"IgnoreBg", 0, NULL, (LPBYTE)&IgnoreBgVal, &size);
			RegQueryValueExW(Rectify11, L"IgnoreCursors", 0, NULL, (LPBYTE)&IgnoreCursorsVal, &size);
			RegQueryValueExW(Rectify11, L"IgnoreIcons", 0, NULL, (LPBYTE)&IgnoreIconsVal, &size);
			RegQueryValueExW(Rectify11, L"IgnoreColors", 0, NULL, (LPBYTE)&IgnoreColorsVal, &size);
			RegQueryValueExW(Rectify11, L"IgnoreSounds", 0, NULL, (LPBYTE)&IgnoreSoundsVal, &size);
			RegQueryValueExW(Rectify11, L"IgnoreScreensavers", 0, NULL, (LPBYTE)&IgnoreScreensaversVal, &size);
			RegCloseKey(Rectify11);

			if (IgnoreBgVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_BACKGROUND;
			}
			if (IgnoreCursorsVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_CURSOR;
			}
			if (IgnoreIconsVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_DESKTOP_ICONS;
			}
			if (IgnoreColorsVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_COLOR;
			}
			if (IgnoreSoundsVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_SOUND;
			}
			if (IgnoreSoundsVal)
			{
				apply_flags |= THEMETOOL_APPLY_FLAG_IGNORE_SCREENSAVER;
			}

			// apply the theme
			themetool_set_active(NULL, themes[selection], TRUE, apply_flags, 0);
			UpdateThemeGraphic();

			// update mica
			if (HasAdmin)
			{
				BOOL hasMica = FALSE;
				BOOL hasTabbed = FALSE;
				RectifyUtil->GetMicaSettings(&hasMica, &hasTabbed);
				RectifyUtil->SetMicaForEveryoneEnabled(hasMica, hasTabbed);

				// update checkboxes in case we aren't using a mica theme anymore
				MicaForEveryoneCheckbox->SetCheckedState(hasMica ? CSF_Checked : CSF_Unchecked);
				TabbedCheckbox->SetCheckedState(hasTabbed ? CSF_Checked : CSF_Unchecked);
			}
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"buttonHelp"))
	{
		if (iev->uidType == Button::Click)
		{
			ShellExecute(0, 0, TEXT("http://rectify11.net"), 0, 0, SW_SHOW);
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"MicaChk"))
	{
		TouchCheckBox *MicaForEveryoneCheckbox = (TouchCheckBox *)iev->peTarget;
		TouchCheckBox *TabbedCheckbox = (TouchCheckBox *)GetRoot()->FindDescendent(StrToID((LPCWSTR)L"TabChk"));
		Combobox *ThemeCombo = (Combobox *)GetRoot()->FindDescendent(StrToID((LPCWSTR)L"ThemeCmb"));
		if (iev->uidType == TouchButton::Click)
		{
			CheckedStateFlags MicaEnabled2 = MicaForEveryoneCheckbox->GetCheckedState();
			CheckedStateFlags TabbedEnabled = TabbedCheckbox->GetCheckedState();

			RectifyUtil->SetMicaForEveryoneEnabled(MicaEnabled2 == CSF_Checked ? TRUE : FALSE, TabbedEnabled ? CSF_Checked : CSF_Unchecked);

			// Enable/disable the tabbed checkbox
			if (TabbedCheckbox != NULL)
				TabbedCheckbox->SetEnabled(MicaEnabled2 == CSF_Checked ? TRUE : FALSE);
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"TabChk"))
	{
		TouchCheckBox *TabbedCheckbox = (TouchCheckBox *)iev->peTarget;
		Combobox *ThemeCombo = (Combobox *)FindDescendent(StrToID((LPCWSTR)L"ThemeCmb"));

		if (iev->uidType == TouchButton::Click)
		{
			RectifyUtil->SetMicaForEveryoneEnabled(TRUE, TabbedCheckbox->GetCheckedState() == CSF_Checked ? TRUE : FALSE);
		}
	}
	else if (iev->peTarget->GetID() == StrToID((LPCWSTR)L"ThemetoolInstall"))
	{
		iev->peTarget->SetEnabled(FALSE);
		HRESULT hr = RectifyUtil->InstallThemeTool();
		if (FAILED(hr))
		{
			CHAR buffer[1024];
			std::string message = std::system_category().message(hr);

			snprintf(buffer, sizeof(buffer), "Failed to install SecureUxTheme. Error code is %x, which translates to %s.", hr, message.c_str());
			MessageBoxA(GetMainHwnd(), buffer, "Error during SecureUxTheme install", MB_ICONERROR);
		}

		UpdateThemetoolStatus();
		iev->peTarget->SetEnabled(TRUE);
	}
	// handle menu section
	if (iev->uidType == Button::Click && !wcscmp((const wchar_t *)iev->peTarget->GetClassInfoW()->GetName(), (const wchar_t *)CCRadioButton::GetClassInfoPtr()->GetName()))
	{
		CCRadioButton *chkbox = (CCRadioButton *)iev->peTarget;
		if (chkbox->GetSelected())
		{
			HRESULT hr = E_ACTIVATIONDENIED_SHELLERROR;
			if (chkbox->GetID() == StrToID((LPCWSTR)L"Win11DefaultMenus"))
			{
				hr = RectifyUtil->SetCurrentMenuByIndex(Normal);
			}
			else if (chkbox->GetID() == StrToID((LPCWSTR)L"NilesoftSmall"))
			{
				hr = RectifyUtil->SetCurrentMenuByIndex(NilesoftSmall);
			}
			else if (chkbox->GetID() == StrToID((LPCWSTR)L"NilesoftFull"))
			{
				hr = RectifyUtil->SetCurrentMenuByIndex(NilesoftFull);
			}
			else if (chkbox->GetID() == StrToID((LPCWSTR)L"Classic"))
			{
				hr = RectifyUtil->SetCurrentMenuByIndex(Classic);
			}
			else if (chkbox->GetID() == StrToID((LPCWSTR)L"ClassicTransparent"))
			{
				hr = RectifyUtil->SetCurrentMenuByIndex(ClassicTransparent);
			}

			if (FAILED(hr))
			{
				WCHAR buffer[200];
				swprintf(buffer, 199, L"Failed to update menu settings. HRESULT is %x", hr);
				MessageBox(NULL, buffer, TEXT("ClassicTransparent_OnEvent"), MB_ICONERROR);
			}
			else
			{
				ShowRestartExplorer();
			}
		}
	}
}

void RectifyMainPage::ShowRestartExplorer()
{
	TouchButton *BtnRestartExplorer = (TouchButton *)FindDescendent(StrToID((LPCWSTR)L"BtnRestartExplorer"));
	BtnRestartExplorer->SetLayoutPos(0);
	BtnRestartExplorer->SetVisible(TRUE);
}

void RectifyMainPage::UpdateThemeGraphic()
{
	LPCWSTR id = IsDarkTheme() ? MAKEINTRESOURCE(IDB_DARKPREVIEW) : MAKEINTRESOURCE(IDB_LIGHTPREVIEW);
	HBITMAP bmp = (HBITMAP)LoadImage(g_hInst, id, IMAGE_BITMAP, 256, 256, 0);
	if (bmp == NULL)
	{
		return;
	}
	Value *bitmap = DirectUI::Value::CreateGraphic(bmp, 3, 0xffffffff, false, false, false);
	Element *PreviewElement = FindDescendent(StrToID((LPCWSTR)L"ThemePreview"));
	if (PreviewElement != NULL)
		PreviewElement->SetValue(Element::ContentProp, 1, bitmap);
	bitmap->Release();
}

void RectifyMainPage::InitNavLinks()
{
	auto pLinks = new CControlPanelNavLinks();

	WCHAR themePrefString[1024];
	if (FAILED(LoadStringW(g_hInst, IDS_UPDATE, themePrefString, 1023)))
	{
		wcscpy_s(themePrefString, L"[ THEME APPLY PREF ]");
	}
	WCHAR sysInfoString[1024];
	if (FAILED(LoadStringW(g_hInst, IDS_SYSINFO, sysInfoString, 1023)))
	{
		wcscpy_s(sysInfoString, L"[ SYS INFO ]");
	}
	WCHAR uninstallString[1024];
	if (FAILED(LoadStringW(g_hInst, IDS_UNINSTALL, uninstallString, 1023)))
	{
		wcscpy_s(uninstallString, L"[ UNINSTALL ]");
	}

	pLinks->AddLinkControlPanel(CPNAV_LIST_TASKS, g_hInst, IDS_UPDATE, L"Rectify11.SettingsCPL", L"pageThemePref", NULL);
	pLinks->AddLinkShellEx(CPNAV_LIST_TASKS, g_hInst, IDS_UNINSTALL, L"C:\\Windows\\Rectify11\\Uninstall.exe", L"", NULL);
	pLinks->AddLinkControlPanel(CPNAV_LIST_SEEALSO, g_hInst, IDS_SYSINFO, L"Microsoft.System", L"", NULL);


	GUID SID_PerLayoutPropertyBag = {};
	HRESULT hr = CLSIDFromString(L"{a46e5c25-c09c-4ca8-9a53-49cf7f865525}", (LPCLSID)&SID_PerLayoutPropertyBag);
	if (SUCCEEDED(hr))
	{
		IPropertyBag *bag = NULL;
		hr = IUnknown_QueryService(_punkSite, SID_PerLayoutPropertyBag, IID_IPropertyBag, (LPVOID *)&bag);
		if (SUCCEEDED(hr))
		{
			hr = PSPropertyBag_WriteUnknown(bag, L"ControlPanelNavLinks", pLinks);
			if (SUCCEEDED(hr))
			{
				hr = S_OK;
			}
			else
			{
				MessageBox(NULL, TEXT("Failed to write property bag for navigation links"), TEXT("CElementProvider::InitNavLinks"), 0);
			}
			bag->Release();
		}
		else
		{
			MessageBox(NULL, TEXT("Failed to get property bag for navigation links"), TEXT("CElementProvider::InitNavLinks"), 0);
		}
	}
	else
	{
		MessageBox(NULL, TEXT("Failed to parse hardcoded GUID (SID_PerLayoutPropertyBag)"), TEXT("CElementProvider::InitNavLinks"), 0);
	}
}

void RectifyMainPage::UpdateThemetoolStatus()
{
	Element *status = (TouchButton *)FindDescendent(StrToID((LPCWSTR)L"ThemetoolStatus"));

	ULONG flags = secureuxtheme_get_state_flags();
	wstring statusText;

	WCHAR buffer1[1024];
	if (FAILED(LoadStringW(g_hInst, IDS_THEMETOOLSTATUS, buffer1, 1023)))
	{
		wcscpy_s(buffer1, L"[SECURE UX STATUS STRING MISSING]: ");
	}

	statusText += buffer1;


	if (flags & SECUREUXTHEME_STATE_INSTALLED)
	{
		if (flags & SECUREUXTHEME_STATE_CURRENT)
		{
			if (FAILED(LoadStringW(g_hInst, IDS_OK, buffer1, 1023)))
			{
				wcscpy_s(buffer1, L"OK STRING MISSING");
			}

			statusText += buffer1;

			status->SetForegroundStdColor(44); // forest green
		}
		else {
			if (FAILED(LoadStringW(g_hInst, IDS_OUTDATED, buffer1, 1023)))
			{
				wcscpy_s(buffer1, L"OUTDATED STRING MISSING");
			}

			statusText += buffer1;

			status->SetForegroundStdColor(138); // yellow
		}
	}
	else {
		if (FAILED(LoadStringW(g_hInst, IDS_NOTINSTALLED, buffer1, 1023)))
		{
			wcscpy_s(buffer1, L"NOT INSTALLED STRING MISSING");
		}

		statusText += buffer1;

		status->SetForegroundStdColor(113); // red
	}

	status->SetContentString((LPCWSTR)statusText.c_str());
}

IFACEMETHODIMP RectifyMainPage::QueryInterface(REFIID riid, void **ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(CControlPanelPage, IFrameNotificationClient),
		{ 0 },
	};

	HRESULT hr = QISearch(this, qit, riid, ppv);
	if (FAILED(hr))
	{
		hr = CElementWithSite::QueryInterface(riid, ppv);
	}

	return hr;
}

HRESULT RectifyMainPage::LayoutInitialized()
{
	CControlPanelPage::LayoutInitialized();

	HRESULT hr = IUnknown_GetSite(static_cast<IObjectWithSite *>(this), IID_PPV_ARGS(&_punkSite));
	if (SUCCEEDED(hr))
	{
		DUI_WalkIUnknownElements(this, (PFNELEMENTCALLBACK)DUI_SetSiteOnUnknown, (LPARAM)_punkSite);
	}

	hr = themetool_init();
	if (hr != S_OK && hr != HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED))
	{
#ifdef ERROR_MESSAGES
		MessageBox(NULL, TEXT("Failed to initialize SecureUXTheme ThemeTool. Theme information will not be loaded. This may be due to the lack of the ThemeDll.dll in C:\\Windows\\Rectify11\\RectifyControlPanel"), TEXT("FrameProvider::LayoutInitialized"), MB_ICONERROR);
#endif
	}

	Element *root = GetRoot();
	RectifyUtil = (IRectifyUtil *)new CRectifyUtil();
	InitNavLinks();

	Combobox *ThemeCombo = (Combobox *)root->FindDescendent(StrToID((LPCWSTR)L"ThemeCmb"));
	Button *HelpButton = (Button *)root->FindDescendent(StrToID((LPCWSTR)L"buttonHelp"));
	TouchCheckBox *MicaForEveryoneCheckbox = (TouchCheckBox *)root->FindDescendent(StrToID((LPCWSTR)L"MicaChk"));
	TouchCheckBox *TabbedCheckbox = (TouchCheckBox *)root->FindDescendent(StrToID((LPCWSTR)L"TabChk"));
	Element *version = (Element *)root->FindDescendent(StrToID((LPCWSTR)L"RectifyVersion"));
	TouchButton *enableAdmin = (TouchButton *)root->FindDescendent(StrToID((LPCWSTR)L"Link_EnableAdmin"));
	TouchButton *BtnRestartExplorer = (TouchButton *)root->FindDescendent(StrToID((LPCWSTR)L"BtnRestartExplorer"));
	TouchButton *ThemetoolInstall = (TouchButton *)root->FindDescendent(StrToID((LPCWSTR)L"ThemetoolInstall"));

	CCRadioButton *Win11DefaultMenus = (CCRadioButton *)root->FindDescendent(StrToID((LPCWSTR)L"Win11DefaultMenus"));
	CCRadioButton *NilesoftSmall = (CCRadioButton *)root->FindDescendent(StrToID((LPCWSTR)L"NilesoftSmall"));
	CCRadioButton *NilesoftFull = (CCRadioButton *)root->FindDescendent(StrToID((LPCWSTR)L"NilesoftFull"));
	CCRadioButton *Classic = (CCRadioButton *)root->FindDescendent(StrToID((LPCWSTR)L"Classic"));
	CCRadioButton *ClassicTransparent = (CCRadioButton *)root->FindDescendent(StrToID((LPCWSTR)L"ClassicTransparent"));

	CCRadioButton *Options[] = { Win11DefaultMenus, NilesoftSmall, NilesoftFull, Classic, ClassicTransparent };

	if (ThemeCombo != NULL)
	{
		WCHAR value[255] = { 0 };
		PVOID pvData = value;
		DWORD size = sizeof(value);
		RegGetValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager", L"DllName", RRF_RT_REG_SZ, 0, pvData, &size);
		std::wstring msstylePath = std::wstring((LPCWSTR)pvData);
		int k = 0;
		ULONG themeCount = 0;
		if (SUCCEEDED(themetool_get_theme_count(&themeCount)))
		{
			for (ULONG i = 0; i < themeCount; i++)
			{
				ITheme *theme = NULL;
				if (SUCCEEDED(themetool_get_theme(i, &theme)))
				{
					std::wstring nameBuffer = std::wstring(255, '\0');
					theme->GetDisplayName(nameBuffer);

					if (nameBuffer.starts_with(L"Rectify11"))
					{
						ThemeCombo->AddString((LPWSTR)nameBuffer.c_str());
						std::wstring pathBuff = std::wstring();
						theme->GetVisualStyle(pathBuff);
						std::wstring msstylePath = std::wstring((LPCWSTR)pvData);

						std::wstring msstylePathClean = msstylePath;

						const size_t last_slash_idx = msstylePathClean.find_last_of(L"\\/");
						if (std::string::npos != last_slash_idx)
						{
							msstylePathClean.erase(0, last_slash_idx + 1);
						}

						// Remove extension if present.
						const size_t period_idx = msstylePathClean.rfind('.');
						if (std::string::npos != period_idx)
						{
							msstylePathClean.erase(period_idx);
						}

						ThemesMap[k] = msstylePathClean;
						if (pathBuff == msstylePath)
						{
							ThemeCombo->SetSelection(k);
						}
						themes.push_back(i);
						k++;
					}
				}
				themetool_theme_release(theme);
			}
		}
		else
		{
#ifdef ERROR_MESSAGES
			MessageBox(NULL, TEXT("Failed to count the amount of themes"), TEXT("CElementProvider::LayoutInitialized"), MB_ICONERROR);
#endif // 0
		}

		if (version != NULL)
		{
			WCHAR value[255] = { 0 };
			PVOID pvData = value;
			DWORD size = sizeof(value);
			LONG result = RegGetValue(HKEY_LOCAL_MACHINE, L"Software\\Rectify11", L"Version", RRF_RT_REG_SZ, 0, pvData, &size);
			std::wstring vstr = std::wstring(L"");
			if (result == 0)
			{
				WCHAR versionstr[1024];
				if (FAILED(LoadStringW(g_hInst, IDS_VERSION, versionstr, 1023)))
				{
					wcscpy_s(versionstr, L"[VERSION STRING MISSING]: ");
				}
				vstr += versionstr;
				vstr += L" ";
				vstr += value;
			}
			else
			{
				WCHAR versionstr[1024];
				WCHAR notapplicable[1024];
				if (FAILED(LoadStringW(g_hInst, IDS_VERSION, versionstr, 1023)))
				{
					wcscpy_s(versionstr, L"[VERSION STRING MISSING]: ");
				}
				if (FAILED(LoadStringW(g_hInst, IDS_NA, notapplicable, 1023)))
				{
					wcscpy_s(notapplicable, L"[N/A STRING MISSING]: ");
				}

				vstr += versionstr;
				vstr += L" ";
				vstr += notapplicable;
			}
			version->SetContentString((LPCWSTR)vstr.c_str());
		}
	}

	if (MicaForEveryoneCheckbox != NULL)
	{
		MicaForEveryoneCheckbox->SetToggleOnClick(true);
		BOOL MicaEnabled;
		BOOL TabbedEnabled;
		RectifyUtil->GetMicaSettings(&MicaEnabled, &TabbedEnabled);

		MicaForEveryoneCheckbox->SetCheckedState(MicaEnabled ? CSF_Checked : CSF_Unchecked);

		if (!MicaEnabled && TabbedCheckbox != NULL)
		{
			TabbedCheckbox->SetEnabled(FALSE);
		}
	}

	if (TabbedCheckbox != NULL)
	{
		BOOL MicaEnabled;
		BOOL TabbedEnabled;
		RectifyUtil->GetMicaSettings(&MicaEnabled, &TabbedEnabled);

		TabbedCheckbox->SetToggleOnClick(true);
		TabbedCheckbox->SetCheckedState(MicaEnabled ? CSF_Checked : CSF_Unchecked);
	}

	if (BtnRestartExplorer != NULL)
	{
		BtnRestartExplorer->SetLayoutPos(-3);
		BtnRestartExplorer->SetVisible(FALSE);
	}

	DWORD menuIndex;

	for (size_t i = 0; i < 5; i++)
	{
		if (!HasAdmin)
			Options[i]->SetEnabled(FALSE);
		else
			Options[i]->SetEnabled(TRUE);
	}
	if (SUCCEEDED(RectifyUtil->GetCurrentMenuIndex(&menuIndex)))
	{
		Options[menuIndex]->SetSelected(true);
	}

	if (HasAdmin)
	{
		enableAdmin->SetLayoutPos(-3);
		enableAdmin->SetVisible(FALSE);
		ThemetoolInstall->SetEnabled(TRUE);
	}
	else
	{
		MicaForEveryoneCheckbox->SetEnabled(FALSE);
		TabbedCheckbox->SetEnabled(FALSE);
		ThemetoolInstall->SetEnabled(FALSE);
	}

	UpdateThemetoolStatus();

	UpdateThemeGraphic();
	initializing = false;

	return S_OK;
}

HRESULT RectifyMainPage::OnInnerElementDestroyed()
{
	return S_OK;
}

HWND RectifyMainPage::GetMainHwnd()
{
	GUID SID_STopLevelBrowser = {}, IID_IFrameManager = {};
	HRESULT hr = CLSIDFromString(L"{4c96be40-915c-11cf-99d3-00aa004ae837}", (LPCLSID)&SID_STopLevelBrowser);
	HWND result = NULL;
	if (SUCCEEDED(hr))
	{
		hr = CLSIDFromString(L"{31e4fa78-02b4-419f-9430-7b7585237c77}", (LPCLSID)&IID_IFrameManager);
		if (SUCCEEDED(hr))
		{
			IShellBrowser *browser = NULL;
			if (SUCCEEDED(IUnknown_QueryService(_punkSite, SID_STopLevelBrowser, IID_IShellBrowser, (LPVOID *)&browser)))
			{
				browser->GetWindow(&result);
				browser->Release();
			}
		}
	}

	return result;
}

void RectifyMainPage::OnDestroy()
{
	if (RectifyUtil != NULL)
	{
		RectifyUtil->Release();
		RectifyUtil = NULL;
	}

	Element::OnDestroy();
}