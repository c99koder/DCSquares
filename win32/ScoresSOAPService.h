//
// sproxy.exe generated file
// do not modify this file
//
// Created: 04/17/2005@23:14:55
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace ScoresService
{

struct ScoreEntry
{
	int rank;
	BSTR username;
	BSTR userid;
	BSTR score;
	int combo;
	float time;
	BSTR platform;
	BSTR date;
};

template <typename TClient = CSoapSocketClientT<> >
class CScoresServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CScoresServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://dcsquares.c99.org/2.0/SOAP/scores.php"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CScoresServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT getHighScore(
		int userID, 
		BSTR* score
	);

	HRESULT submitScore(
		BSTR username, 
		BSTR password, 
		int score, 
		int combo, 
		float time, 
		BSTR platform, 
		int* result
	);

	HRESULT getRank(
		int userID, 
		int* rank
	);

	HRESULT getScores(
		BSTR startDate, 
		BSTR endDate, 
		BSTR count, 
		ScoreEntry** scores, int* scores_nSizeIs
	);
};

typedef CScoresServiceT<> CScoresService;

__if_not_exists(__ScoreEntry_entries)
{
extern __declspec(selectany) const _soapmapentry __ScoreEntry_entries[] =
{
	{ 
		0x00402E6C, 
		"rank", 
		L"rank", 
		sizeof("rank")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(ScoreEntry, rank),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(ScoreEntry, username),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x1945E8AC, 
		"userid", 
		L"userid", 
		sizeof("userid")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(ScoreEntry, userid),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x085932BC, 
		"score", 
		L"score", 
		sizeof("score")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(ScoreEntry, score),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x073E34B0, 
		"combo", 
		L"combo", 
		sizeof("combo")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(ScoreEntry, combo),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x0041690F, 
		"time", 
		L"time", 
		sizeof("time")-1, 
		SOAPTYPE_FLOAT, 
		SOAPFLAG_FIELD, 
		offsetof(ScoreEntry, time),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xC1101945, 
		"platform", 
		L"platform", 
		sizeof("platform")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(ScoreEntry, platform),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(ScoreEntry, date),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __ScoreEntry_map =
{
	0x043F0D2E,
	"ScoreEntry",
	L"ScoreEntry",
	sizeof("ScoreEntry")-1,
	sizeof("ScoreEntry")-1,
	SOAPMAP_STRUCT,
	__ScoreEntry_entries,
	sizeof(ScoreEntry),
	8,
	-1,
	SOAPFLAG_NONE,
	0x7614597E,
	"urn:Scores",
	L"urn:Scores",
	sizeof("urn:Scores")-1
};
}

struct __CScoresService_getHighScore_struct
{
	int userID;
	BSTR score;
};

extern __declspec(selectany) const _soapmapentry __CScoresService_getHighScore_entries[] =
{

	{
		0x1945E46C, 
		"userID", 
		L"userID", 
		sizeof("userID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_getHighScore_struct, userID),
		NULL,
		NULL,
		-1,
	},
	{
		0x085932BC, 
		"score", 
		L"score", 
		sizeof("score")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_getHighScore_struct, score),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CScoresService_getHighScore_map =
{
	0xBFA505DC,
	"getHighScore",
	L"getHighScore",
	sizeof("getHighScore")-1,
	sizeof("getHighScore")-1,
	SOAPMAP_FUNC,
	__CScoresService_getHighScore_entries,
	sizeof(__CScoresService_getHighScore_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x7614597E,
	"urn:Scores",
	L"urn:Scores",
	sizeof("urn:Scores")-1
};


struct __CScoresService_submitScore_struct
{
	BSTR username;
	BSTR password;
	int score;
	int combo;
	float time;
	BSTR platform;
	int result;
};

extern __declspec(selectany) const _soapmapentry __CScoresService_submitScore_entries[] =
{

	{
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_submitScore_struct, username),
		NULL,
		NULL,
		-1,
	},
	{
		0x9C439233, 
		"password", 
		L"password", 
		sizeof("password")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_submitScore_struct, password),
		NULL,
		NULL,
		-1,
	},
	{
		0x085932BC, 
		"score", 
		L"score", 
		sizeof("score")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_submitScore_struct, score),
		NULL,
		NULL,
		-1,
	},
	{
		0x073E34B0, 
		"combo", 
		L"combo", 
		sizeof("combo")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_submitScore_struct, combo),
		NULL,
		NULL,
		-1,
	},
	{
		0x0041690F, 
		"time", 
		L"time", 
		sizeof("time")-1, 
		SOAPTYPE_FLOAT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_submitScore_struct, time),
		NULL,
		NULL,
		-1,
	},
	{
		0xC1101945, 
		"platform", 
		L"platform", 
		sizeof("platform")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_submitScore_struct, platform),
		NULL,
		NULL,
		-1,
	},
	{
		0x1150D23F, 
		"result", 
		L"result", 
		sizeof("result")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_submitScore_struct, result),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CScoresService_submitScore_map =
{
	0x98741D30,
	"submitScore",
	L"submitScore",
	sizeof("submitScore")-1,
	sizeof("submitScore")-1,
	SOAPMAP_FUNC,
	__CScoresService_submitScore_entries,
	sizeof(__CScoresService_submitScore_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x7614597E,
	"urn:Scores",
	L"urn:Scores",
	sizeof("urn:Scores")-1
};


struct __CScoresService_getRank_struct
{
	int userID;
	int rank;
};

extern __declspec(selectany) const _soapmapentry __CScoresService_getRank_entries[] =
{

	{
		0x1945E46C, 
		"userID", 
		L"userID", 
		sizeof("userID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_getRank_struct, userID),
		NULL,
		NULL,
		-1,
	},
	{
		0x00402E6C, 
		"rank", 
		L"rank", 
		sizeof("rank")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CScoresService_getRank_struct, rank),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CScoresService_getRank_map =
{
	0xECA935EC,
	"getRank",
	L"getRank",
	sizeof("getRank")-1,
	sizeof("getRank")-1,
	SOAPMAP_FUNC,
	__CScoresService_getRank_entries,
	sizeof(__CScoresService_getRank_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x7614597E,
	"urn:Scores",
	L"urn:Scores",
	sizeof("urn:Scores")-1
};


struct __CScoresService_getScores_struct
{
	BSTR startDate;
	BSTR endDate;
	BSTR count;
	ScoreEntry *scores;
	int __scores_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CScoresService_getScores_entries[] =
{

	{
		0x9ED2666C, 
		"startDate", 
		L"startDate", 
		sizeof("startDate")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_getScores_struct, startDate),
		NULL,
		NULL,
		-1,
	},
	{
		0x6689F575, 
		"endDate", 
		L"endDate", 
		sizeof("endDate")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_getScores_struct, endDate),
		NULL,
		NULL,
		-1,
	},
	{
		0x073E5849, 
		"count", 
		L"count", 
		sizeof("count")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_getScores_struct, count),
		NULL,
		NULL,
		-1,
	},
	{
		0x137F8AAF, 
		"scores", 
		L"scores", 
		sizeof("scores")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CScoresService_getScores_struct, scores),
		NULL,
		&__ScoreEntry_map,
		3+1,
	},
	{
		0x137F8AAF,
		"__scores_nSizeIs",
		L"__scores_nSizeIs",
		sizeof("__scores_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CScoresService_getScores_struct, __scores_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CScoresService_getScores_map =
{
	0xBE48722F,
	"getScores",
	L"getScores",
	sizeof("getScores")-1,
	sizeof("getScores")-1,
	SOAPMAP_FUNC,
	__CScoresService_getScores_entries,
	sizeof(__CScoresService_getScores_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x7614597E,
	"urn:Scores",
	L"urn:Scores",
	sizeof("urn:Scores")-1
};

extern __declspec(selectany) const _soapmap * __CScoresService_funcs[] =
{
	&__CScoresService_getHighScore_map,
	&__CScoresService_submitScore_map,
	&__CScoresService_getRank_map,
	&__CScoresService_getScores_map,
	NULL
};

template <typename TClient>
inline HRESULT CScoresServiceT<TClient>::getHighScore(
		int userID, 
		BSTR* score
	)
{
    if ( score == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CScoresService_getHighScore_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userID = userID;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:Scores#scores#getHighScore\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*score = __params.score;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CScoresServiceT<TClient>::submitScore(
		BSTR username, 
		BSTR password, 
		int score, 
		int combo, 
		float time, 
		BSTR platform, 
		int* result
	)
{
    if ( result == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CScoresService_submitScore_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.username = username;
	__params.password = password;
	__params.score = score;
	__params.combo = combo;
	__params.time = time;
	__params.platform = platform;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:Scores#scores#submitScore\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*result = __params.result;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CScoresServiceT<TClient>::getRank(
		int userID, 
		int* rank
	)
{
    if ( rank == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CScoresService_getRank_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userID = userID;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:Scores#scores#getRank\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*rank = __params.rank;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CScoresServiceT<TClient>::getScores(
		BSTR startDate, 
		BSTR endDate, 
		BSTR count, 
		ScoreEntry** scores, int* __scores_nSizeIs
	)
{
    if ( scores == NULL )
		return E_POINTER;
    if( __scores_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CScoresService_getScores_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.startDate = startDate;
	__params.endDate = endDate;
	__params.count = count;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:Scores#scores#getScores\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*scores = __params.scores;
	*__scores_nSizeIs = __params.__scores_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CScoresServiceT<TClient>::GetFunctionMap()
{
	return __CScoresService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CScoresServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CScoresService_getHighScore_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CScoresService_getHighScore_atlsoapheader_map = 
	{
		0xBFA505DC,
		"getHighScore",
		L"getHighScore",
		sizeof("getHighScore")-1,
		sizeof("getHighScore")-1,
		SOAPMAP_HEADER,
		__CScoresService_getHighScore_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x7614597E,
		"urn:Scores",
		L"urn:Scores",
		sizeof("urn:Scores")-1
	};

	static const _soapmapentry __CScoresService_submitScore_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CScoresService_submitScore_atlsoapheader_map = 
	{
		0x98741D30,
		"submitScore",
		L"submitScore",
		sizeof("submitScore")-1,
		sizeof("submitScore")-1,
		SOAPMAP_HEADER,
		__CScoresService_submitScore_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x7614597E,
		"urn:Scores",
		L"urn:Scores",
		sizeof("urn:Scores")-1
	};

	static const _soapmapentry __CScoresService_getRank_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CScoresService_getRank_atlsoapheader_map = 
	{
		0xECA935EC,
		"getRank",
		L"getRank",
		sizeof("getRank")-1,
		sizeof("getRank")-1,
		SOAPMAP_HEADER,
		__CScoresService_getRank_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x7614597E,
		"urn:Scores",
		L"urn:Scores",
		sizeof("urn:Scores")-1
	};

	static const _soapmapentry __CScoresService_getScores_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CScoresService_getScores_atlsoapheader_map = 
	{
		0xBE48722F,
		"getScores",
		L"getScores",
		sizeof("getScores")-1,
		sizeof("getScores")-1,
		SOAPMAP_HEADER,
		__CScoresService_getScores_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x7614597E,
		"urn:Scores",
		L"urn:Scores",
		sizeof("urn:Scores")-1
	};


	static const _soapmap * __CScoresService_headers[] =
	{
		&__CScoresService_getHighScore_atlsoapheader_map,
		&__CScoresService_submitScore_atlsoapheader_map,
		&__CScoresService_getRank_atlsoapheader_map,
		&__CScoresService_getScores_atlsoapheader_map,
		NULL
	};
	
	return __CScoresService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CScoresServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CScoresServiceT<TClient>::GetNamespaceUri()
{
	return L"urn:Scores";
}

template <typename TClient>
ATL_NOINLINE inline const char * CScoresServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CScoresServiceT<TClient>::GetNamespaceUriA()
{
	return "urn:Scores";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CScoresServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CScoresServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace ScoresService

template<>
inline HRESULT AtlCleanupValue<ScoresService::ScoreEntry>(ScoresService::ScoreEntry *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->rank);
	AtlCleanupValue(&pVal->username);
	AtlCleanupValue(&pVal->userid);
	AtlCleanupValue(&pVal->score);
	AtlCleanupValue(&pVal->combo);
	AtlCleanupValue(&pVal->time);
	AtlCleanupValue(&pVal->platform);
	AtlCleanupValue(&pVal->date);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<ScoresService::ScoreEntry>(ScoresService::ScoreEntry *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->rank, pMemMgr);
	AtlCleanupValueEx(&pVal->username, pMemMgr);
	AtlCleanupValueEx(&pVal->userid, pMemMgr);
	AtlCleanupValueEx(&pVal->score, pMemMgr);
	AtlCleanupValueEx(&pVal->combo, pMemMgr);
	AtlCleanupValueEx(&pVal->time, pMemMgr);
	AtlCleanupValueEx(&pVal->platform, pMemMgr);
	AtlCleanupValueEx(&pVal->date, pMemMgr);
	return S_OK;
}
