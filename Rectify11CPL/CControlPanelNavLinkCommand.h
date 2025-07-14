#pragma once

typedef struct _SQM_STREAM_ENTRY
{
	DWORD dwType;
	union
	{
		WCHAR *pwszVal;
		DWORD dwVal;
	} val;
} SQM_STREAM_ENTRY;

class CControlPanelNavLinkCommand
{
public:
	struct SQM_STREAM_INFO
	{
		DWORD dwDatapointId;
		DWORD cSqmStreamEntries;
		SQM_STREAM_ENTRY *pSqmStreamEntries;
	};

	CPNAVTYPE m_ExecType;
	int m_space;
	LPWSTR m_AppletOrCommand;
	LPWSTR m_Arguments;
	bool _fUseNavPaneProvider;
	bool _fLogRecentItems;
	SQM_STREAM_INFO _sqmStreamInfo;
};
