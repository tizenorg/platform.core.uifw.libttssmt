/*
* Samsung TTS
* Copyright 2012-2016  Samsung Electronics Co., Ltd
*
* Licensed under the Flora License, Version 1.1 (the License);
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://floralicense.org/license/
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "SMTApis.h"

#include "_SlpSmtApis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <thread>
#include <mutex>
#include <unistd.h>

#define SMT_SAMPLING_RATE	16000
#define SMT_FRAME_PERIOD	80

#define PCM_BUFFER_SIZE    640000

// Not supported yet {
#define TTSPE_LANG_CANTONESE    "zh_HK"
#define TTSPE_LANG_TAIWAN       "zh_TW"
#define TTSPE_LANG_RUSSIAN      "ru_RU"
#define TTSPE_LANG_JAPANESE     "ja_JP"
#define TTSPE_LANG_NETHERLANDS  "nl_NL"
#define TTSPE_LANG_PORTUGUESE   "pt_PT"
#define TTSPE_LANG_GREEK        "el_GR"
#define TTSPE_LANG_TURKEY       "tr_TR"
// Not supported yet }


typedef struct __TypeVoiceInfo _TypeVoiceInfo;
struct __TypeVoiceInfo
{
	char const         * const pszLanguage;
	unsigned int          const sszLanguage;
	int const eVoiceType;

};


static char const _pszKorean              [] = "ko_KR";
static char const _pszUSEnglish           [] = "en_US";
static char const _pszChinese             [] = "zh_CN";
static char const _pszUKEnglish           [] = "en_GB";
static char const _pszGerman              [] = "de_DE";
static char const _pszSpanish             [] = "es_ES";
static char const _pszFrench              [] = "fr_FR";
static char const _pszItalian             [] = "it_IT";
static char const _pszRussian             [] = "ru_RU";
static char const _pszJapanese            [] = "ja_JP";
static char const _pszBRPortuguese        [] = "pt_BR";
static char const _pszPTPortuguese        [] = "pt_PT";
static char const _pszMXSpanish           [] = "es_US";

static char const _pszGreek               [] = "el_GR";
static char const _pszTWChinese           [] = "zh_TW";
static char const _pszNorwegian           [] = "nb_NO";
static char const _pszHKChinese           [] = "zh_HK";
static char const _pszPolish              [] = "pl_PL";
static char const _pszHungarian           [] = "hu_HU";
static char const _pszCzech               [] = "cs_CZ";
static char const _pszFinnish             [] = "fi_FI";
static char const _pszHindi               [] = "hi_IN";
static char const _pszINEnglish           [] = "en_IN";
static char const _pszSwedish             [] = "sv_SE";
static char const _pszDanish              [] = "da_DK";
static char const _pszTurkish             [] = "tr_TR";
static char const _pszSlovak              [] = "sk_SK";
static char const _pszDutch               [] = "nl_NL";

static unsigned int const _sszKorean                 = sizeof(_pszKorean);
static unsigned int const _sszUSEnglish              = sizeof(_pszUSEnglish);
static unsigned int const _sszChinese                = sizeof(_pszChinese);
static unsigned int const _sszUKEnglish              = sizeof(_pszUKEnglish);
static unsigned int const _sszGerman                 = sizeof(_pszGerman);
static unsigned int const _sszSpanish                = sizeof(_pszSpanish);
static unsigned int const _sszFrench                 = sizeof(_pszFrench);
static unsigned int const _sszItalian                = sizeof(_pszItalian);
static unsigned int const _sszRussian                = sizeof(_pszRussian);
static unsigned int const _sszJapanese               = sizeof(_pszJapanese);
static unsigned int const _sszBRPortuguese           = sizeof(_pszBRPortuguese);
static unsigned int const _sszPTPortuguese           = sizeof(_pszPTPortuguese);
static unsigned int const _sszMXSpanish              = sizeof(_pszMXSpanish);

static unsigned int const _sszGreek                  = sizeof(_pszGreek );
static unsigned int const _sszTWChinese              = sizeof(_pszTWChinese );
static unsigned int const _sszNorwegian              = sizeof(_pszNorwegian );
static unsigned int const _sszHKChinese              = sizeof(_pszHKChinese );
static unsigned int const _sszPolish                 = sizeof(_pszPolish );
static unsigned int const _sszHungarian              = sizeof(_pszHungarian );
static unsigned int const _sszCzech                  = sizeof(_pszCzech );
static unsigned int const _sszFinnish                = sizeof(_pszFinnish );
static unsigned int const _sszHindi                  = sizeof(_pszHindi );
static unsigned int const _sszINEnglish              = sizeof(_pszINEnglish );
static unsigned int const _sszSwedish                = sizeof(_pszSwedish );
static unsigned int const _sszDanish                 = sizeof(_pszDanish );
static unsigned int const _sszTurkish                = sizeof(_pszTurkish );
static unsigned int const _sszSlovak                 = sizeof(_pszSlovak ); 
static unsigned int const _sszDutch                  = sizeof(_pszDutch ); 


static const _TypeVoiceInfo _pVoiceInfos[] =
{
	{ _pszKorean		, _sszKorean		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszKorean		, _sszKorean		, TTSP_VOICE_TYPE_MALE },
	{ _pszUSEnglish		, _sszUSEnglish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszUSEnglish		, _sszUSEnglish		, TTSP_VOICE_TYPE_MALE },
	{ _pszChinese		, _sszChinese		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszChinese		, _sszChinese		, TTSP_VOICE_TYPE_MALE },
	{ _pszUKEnglish		, _sszUKEnglish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszUKEnglish		, _sszUKEnglish		, TTSP_VOICE_TYPE_MALE },
	{ _pszGerman		, _sszGerman		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszGerman		, _sszGerman		, TTSP_VOICE_TYPE_MALE },
	{ _pszSpanish		, _sszSpanish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszSpanish		, _sszSpanish		, TTSP_VOICE_TYPE_MALE },
	{ _pszFrench		, _sszFrench		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszFrench		, _sszFrench		, TTSP_VOICE_TYPE_MALE },
	{ _pszItalian		, _sszItalian		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszItalian		, _sszItalian		, TTSP_VOICE_TYPE_MALE },
	{ _pszRussian		, _sszRussian		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszJapanese		, _sszJapanese		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszJapanese		, _sszJapanese		, TTSP_VOICE_TYPE_MALE },
	{ _pszBRPortuguese	, _sszBRPortuguese	, TTSP_VOICE_TYPE_FEMALE },
	{ _pszBRPortuguese	, _sszBRPortuguese	, TTSP_VOICE_TYPE_MALE },
	{ _pszPTPortuguese	, _sszPTPortuguese	, TTSP_VOICE_TYPE_FEMALE },
	{ _pszPTPortuguese	, _sszPTPortuguese	, TTSP_VOICE_TYPE_MALE },
	{ _pszMXSpanish		, _sszMXSpanish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszMXSpanish		, _sszMXSpanish		, TTSP_VOICE_TYPE_MALE },

	{ _pszGreek		, _sszGreek		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszTWChinese		, _sszTWChinese		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszNorwegian		, _sszNorwegian		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszHKChinese		, _sszHKChinese		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszHKChinese		, _sszHKChinese		, TTSP_VOICE_TYPE_MALE },
	{ _pszPolish		, _sszPolish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszHungarian		, _sszHungarian		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszCzech		, _sszCzech		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszFinnish		, _sszFinnish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszHindi		, _sszHindi		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszINEnglish		, _sszINEnglish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszSwedish		, _sszSwedish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszDanish		, _sszDanish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszTurkish		, _sszTurkish		, TTSP_VOICE_TYPE_FEMALE },
	{ _pszSlovak		, _sszSlovak		, TTSP_VOICE_TYPE_FEMALE }, 
	{ _pszDutch		, _sszDutch		, TTSP_VOICE_TYPE_FEMALE }
};

#define _nVoiceInfos   (sizeof(_pVoiceInfos) / sizeof(_pVoiceInfos[0]))

// index of VoiceInfos
#define VOICE_INDEX_KOREAN_WOMAN	0
#define VOICE_INDEX_KOREAN_MAN		1
#define VOICE_INDEX_USENGLISH_WOMAN	2
#define VOICE_INDEX_USENGLISH_MAN	3
#define VOICE_INDEX_CHINESE_WOMAN	4
#define VOICE_INDEX_CHINESE_MAN		5
#define VOICE_INDEX_UKENGLISH_WOMAN	6
#define VOICE_INDEX_UKENGLISH_MAN	7
#define VOICE_INDEX_GERMAN_WOMAN	8
#define VOICE_INDEX_GERMAN_MAN		9
#define VOICE_INDEX_SPANISH_WOMAN	10
#define VOICE_INDEX_SPANISH_MAN		11
#define VOICE_INDEX_FRENCH_WOMAN	12
#define VOICE_INDEX_FRENCH_MAN		13
#define VOICE_INDEX_ITALIAN_WOMAN	14
#define VOICE_INDEX_ITALIAN_MAN		15
#define VOICE_INDEX_RUSSIAN_WOMAN	16
#define VOICE_INDEX_JAPANESE_WOMAN	17
#define VOICE_INDEX_JAPANESE_MAN	18
#define VOICE_INDEX_BRPORTUGUESE_WOMAN	19
#define VOICE_INDEX_BRPORTUGUESE_MAN	20
#define VOICE_INDEX_PTPORTUGUESE_WOMAN	21
#define VOICE_INDEX_PTPORTUGUESE_MAN	22
#define VOICE_INDEX_MXSPANISH_WOMAN	23
#define VOICE_INDEX_MXSPANISH_MAN	24

#define VOICE_INDEX_GREEK_WOMAN		25
#define VOICE_INDEX_TWCHINESE_WOMAN	26
#define VOICE_INDEX_NORWEGIAN_WOMAN	27
#define VOICE_INDEX_HKCHINESE_WOMAN	28
#define VOICE_INDEX_HKCHINESE_MAN	29
#define VOICE_INDEX_POLISH_WOMAN	30
#define VOICE_INDEX_HUNGARIAN_WOMAN	31
#define VOICE_INDEX_CZECH_WOMAN		32
#define VOICE_INDEX_FINNISH_WOMAN	33
#define VOICE_INDEX_HINDI_WOMAN		34
#define VOICE_INDEX_INENGLISH_WOMAN	35
#define VOICE_INDEX_SWEDISH_WOMAN	36
#define VOICE_INDEX_DANISH_WOMAN	37
#define VOICE_INDEX_TURKISH_WOMAN	38
#define VOICE_INDEX_SLOVAK_WOMAN	39
#define VOICE_INDEX_DUTCH_WOMAN		40

#define VOICE_INDEX_MAX			41

#define DATA_DIR1	"/usr/share/voice/tts/smt_vdata/"

//static const char* DATA_DIR2 = app_get_data_path();
#define DATA_DIR2	tzplatform_mkpath(TZ_USER_HOME, "/share/.voice/tts/engine-info/")

static ttspe_voice_info_s * _gpVoiceInfos  = NULL;

typedef struct _TypeThreadQueueNode TypeThreadQueueNode;
struct _TypeThreadQueueNode
{
	int                   iVoiceInfo;
	char                * pszTextUtf8;
	void                * pUserParam;
	TypeThreadQueueNode * pNext;
};

static struct _global
{
	ttspe_result_cb      pfnCallback;

	int        eSpeechSpeed;
	int                  iVoiceInfo;
	bool                 bStop;
	std::mutex           bStopMutex;
	bool                 bSentenceDone;
	int                  iSamplingRate;
	// thread
	int                  ThreadId;
	pthread_t            Thread;
	pthread_mutex_t      ThreadLock;
	pthread_mutex_t      MainThreadFinalizeLock;
	TypeThreadQueueNode* ThreadQueue_pHead;
	TypeThreadQueueNode* ThreadQueue_pTail;
} _g =
{
	NULL                     , // pfnCallback

	8                        , // eSpeechSpeed
	-1                       , // iVoiceInfo, initial value means INVALID INDEX
	false                    , // bStop
	{}                       , // bStopMutex
	true                     , // bSentenceDone
	SMT_SAMPLING_RATE        , // iSamplingRate

	-1                       , // ThreadId
	0                        , // Thread
	PTHREAD_MUTEX_INITIALIZER, // ThreadLock
	PTHREAD_MUTEX_INITIALIZER, // MainThreadFinalizeLock
	NULL                     , // ThreadQueue_pHead
	NULL                     , // ThreadQueue_pTail
};

static void                 _PushThreadData (int const iVoiceInfo, char const * pszTextUtf8, void* pUserParam);
static TypeThreadQueueNode* _PopThreadData  (void);
static bool                 _bEmptyThreadData(void);
static void                 _CleanThreadData(void);
static void *               _Synthesize     (void* NotUsed); // Thread routine


static int  _ChangeVoice         (int const iVoiceInfo);
static void _SetSpeechSpeed      (void);
static void _CallBack            (ttsp_result_event_e eEvent, unsigned int const nPCMs, void * pPCMs, void* pUserParam);
static int _Synthesize_SamsungTTS(char const * const pszTextUtf8, void* pUserParam, int const FramePeriod);

char const * SLPSMT_GetPszKorean					(void) { return _pszKorean              ; }
char const * SLPSMT_GetPszUSEnglish                (void) { return _pszUSEnglish           ; }
char const * SLPSMT_GetPszChinese             (void) { return _pszChinese             ; }
char const * SLPSMT_GetPszUKEnglish           (void) { return _pszUKEnglish           ; }
char const * SLPSMT_GetPszGerman              (void) { return _pszGerman              ; }
char const * SLPSMT_GetPszSpanish             (void) { return _pszSpanish             ; }
char const * SLPSMT_GetPszFrench              (void) { return _pszFrench              ; }
char const * SLPSMT_GetPszItalian             (void) { return _pszItalian             ; }
char const * SLPSMT_GetPszRussian             (void) { return _pszRussian             ; }
char const * SLPSMT_GetPszJapanese            (void) { return _pszJapanese            ; }
char const * SLPSMT_GetPszBRPortuguese            (void) { return _pszBRPortuguese            ; }
char const * SLPSMT_GetPszPTPortuguese            (void) { return _pszPTPortuguese            ; }
char const * SLPSMT_GetPszMXSpanish           (void) { return _pszMXSpanish            ; }

char const * SLPSMT_GetPszGreek(void) { return _pszGreek     ;}
char const * SLPSMT_GetPszTWChinese(void) { return _pszTWChinese ;}
char const * SLPSMT_GetPszNorwegian(void) { return _pszNorwegian ;}
char const * SLPSMT_GetPszHKChinese(void) { return _pszHKChinese ;}
char const * SLPSMT_GetPszPolish(void) { return _pszPolish    ;}
char const * SLPSMT_GetPszHungarian(void) { return _pszHungarian ;}
char const * SLPSMT_GetPszCzech(void) { return _pszCzech     ;}
char const * SLPSMT_GetPszFinnish(void) { return _pszFinnish   ;}
char const * SLPSMT_GetPszHindi(void) { return _pszHindi     ;}
char const * SLPSMT_GetPszINEnglish(void) { return _pszINEnglish ;}
char const * SLPSMT_GetPszSwedish(void) { return _pszSwedish   ;}
char const * SLPSMT_GetPszDanish(void) { return _pszDanish    ;}
char const * SLPSMT_GetPszTurkish(void) { return _pszTurkish   ;}
char const * SLPSMT_GetPszSlovak(void) { return _pszSlovak    ;}
char const * SLPSMT_GetPszDutch(void) { return _pszDutch    ;}


int          SLPSMT_GetWorkingThreadId(void) { return _g.ThreadId; }




int SLPSMT_StopSynthesis(void)
{
	SLOG(LOG_DEBUG, TAG_TTSP, ">> SLPSMT_StopSynthesis()");
	std::lock_guard<std::mutex> lock( _g.bStopMutex );
	_g.bStop         = true;
	_g.bSentenceDone = true;
	_CleanThreadData();
	return TTSP_ERROR_NONE;
}

int SLPSMT_SynthesizeText(int const iVoiceInfo, char const * pszTextUtf8, void * const pUserParam)
{
	if (! _g.pfnCallback)                { return TTSP_ERROR_INVALID_STATE; }
	if (! pszTextUtf8 || ! *pszTextUtf8) { return TTSP_ERROR_INVALID_PARAMETER; }

	SLOG(LOG_DEBUG, TAG_TTSP, ">> SLPSMT_SynthesizeText()");
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>> iVoiceInfo  : %d", iVoiceInfo );
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>> pszTextUtf8 : %s", pszTextUtf8);


	std::lock_guard<std::mutex> lock( _g.bStopMutex );
	_g.bStop = false;

	_PushThreadData(iVoiceInfo, pszTextUtf8, pUserParam);

	if (_g.ThreadId < 0)
	{
		_g.ThreadId = pthread_create(& _g.Thread, NULL, _Synthesize, pUserParam);
		if (_g.ThreadId < 0)
		{
			SLOG(LOG_ERROR, TAG_TTSP, ">>>  Fail to create thread");
			return TTSP_ERROR_OPERATION_FAILED;
		}
		pthread_detach(_g.Thread);
	}
	return TTSP_ERROR_NONE;
}

/*
int SLPSMT_GetiVoiceInfoEx(char const *pszLanguage, int const eVoiceType)
{
	int i;
	for (i=0 ; i<_nVoiceInfos ; i++)
	{
		if ( eVoiceType == _pVoiceInfos[i].eVoiceType  && ! strcmp(pszLanguage ,  _pVoiceInfos[i].pszLanguage)   )
		{
			int iResult = -1;
			_eTypeTTSMode TTSType =   eTTSMode_Normal;
			char Language[3] = {0,};
			char Contry[3] = {0,};
			char VoiceType[2] = {0,};

			VoiceType[0] = 'f';
			VoiceType[1] = 0;
			memcpy( Language , pszLanguage , sizeof(Language)-1);
			memcpy( Contry , pszLanguage+3 , sizeof(Contry)-1);

			if( eVoiceType == TTSP_VOICE_TYPE_MALE ) VoiceType[0] = 'm';

			iResult = SMTCheckVoiceAvailable( (_eTypeTTSMode)TTSType , DATA_DIR1 , '/' , Language , Contry , VoiceType , 0 );

			SLOG(LOG_DEBUG, TAG_TTSP, "%d)####### iResult [%d]",i , iResult);

			if( iResult == 0 ) return i ;
		}
	}
	return -1;
}
*/

int SLPSMT_SetVoiceList(ttspe_voice_list_s * p)
{
	if (! _gpVoiceInfos)
	{
		unsigned int i;

		_gpVoiceInfos = (ttspe_voice_info_s*) calloc(_nVoiceInfos, sizeof(ttspe_voice_info_s));
		if (! _gpVoiceInfos) { return TTSP_ERROR_OUT_OF_MEMORY; }

		for (i=0 ; i<_nVoiceInfos ; i++)
		{
			_gpVoiceInfos[i].lang   = strdup(_pVoiceInfos[i].pszLanguage);
			_gpVoiceInfos[i].vctype =        _pVoiceInfos[i].eVoiceType  ;
		}
	}
	p->voice_info = _gpVoiceInfos;
	p->size       = _nVoiceInfos;
	return TTSP_ERROR_NONE;
}

void SLPSMT_SetSpeechSpeed(int const eSpeechSpeed)
{
	int level = -1;

	if (eSpeechSpeed == 0) {
		//		level = TTSP_SPEED_NORMAL;
		level = 8;
	} else if (eSpeechSpeed >= 1 && eSpeechSpeed <= 3) {
		level = 2;
	} else if (eSpeechSpeed >= 4 && eSpeechSpeed <= 6) {
		level = 5;
	} else if (eSpeechSpeed >= 7 && eSpeechSpeed <= 9) {
		level = 8;
	} else if (eSpeechSpeed >= 10 && eSpeechSpeed <= 12) {
		level = 11;
	} else if (eSpeechSpeed >= 13 && eSpeechSpeed <= 15) {
		level = 14;
	} else {
		level = 8;
	}

	_g.eSpeechSpeed = level;
}

int SLPSMT_GetiVoiceInfo(char const *pszLanguage, int const eVoiceType)
{
	int i;
	for (i=0 ; i<_nVoiceInfos ; i++)
	{
		if (eVoiceType == _pVoiceInfos[i].eVoiceType
			&& ! strcmp(pszLanguage ,  _pVoiceInfos[i].pszLanguage)
			)
		{
			return i;
		}
	}
	return -1;
}

int SLPSMT_Initialize(ttspe_result_cb pfnCallBack)
{
	if (pfnCallBack)
	{
		_g.pfnCallback = pfnCallBack;
		return TTSP_ERROR_NONE;
	}
	return TTSP_ERROR_INVALID_STATE;
}

int SLPSMT_Finalize(void)
{
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>> SLPSMT_Finalize() called.");

	std::lock_guard<std::mutex> lock( _g.bStopMutex );
	_g.bStop = true;

	_CleanThreadData();

	pthread_mutex_lock  (& _g.MainThreadFinalizeLock);       // <---- lock
	SMTFinalize();
	pthread_mutex_unlock(& _g.MainThreadFinalizeLock);       // <---- unlock

	if (_gpVoiceInfos)
	{
		unsigned int i;
		for (i=0 ; i<_nVoiceInfos ; i++)
		{
			if (_gpVoiceInfos[i].lang) { free(_gpVoiceInfos[i].lang); }
		}
		free(_gpVoiceInfos);
	}

	_g.pfnCallback  = NULL;
	_g.eSpeechSpeed = 8;
	_g.iVoiceInfo   = -1;
	while(_g.ThreadId != -1){
		usleep(10000);
	}

	SLOG(LOG_DEBUG, TAG_TTSP, ">>>> SLPSMT_Finalize() returns.");
	return TTSP_ERROR_NONE;
}




static void _PushThreadData(int const iVoiceInfo, char const * pszTextUtf8, void* pUserParam)
// iVoiceInfo  should be correct.
// pszTextUtf8 should not be NULL.
{
	bool   b = false;
	char * pszDuplicatedTextUtf8;

	pthread_mutex_lock  (& _g.ThreadLock);       // <---- lock

	pszDuplicatedTextUtf8 = strdup(pszTextUtf8);
	if (pszDuplicatedTextUtf8)
	{
		TypeThreadQueueNode* p = (TypeThreadQueueNode*) calloc (1, sizeof(TypeThreadQueueNode));
		if (p)
		{
			p->iVoiceInfo  = iVoiceInfo;
			p->pszTextUtf8 = pszDuplicatedTextUtf8;
			p->pUserParam  = pUserParam;
			if (! _g.ThreadQueue_pHead) { _g.ThreadQueue_pHead        = p; }
			if (  _g.ThreadQueue_pTail) { _g.ThreadQueue_pTail->pNext = p; }
			_g.ThreadQueue_pTail = p;
			b = true;
		}

		if (! b) { free(pszDuplicatedTextUtf8); }
	}

	if (! b) { SLOG(LOG_ERROR, TAG_TTSP, ">>>__PushThreadData, out of memory"); }

	pthread_mutex_unlock(& _g.ThreadLock);       // <---- unlock
}

static TypeThreadQueueNode* _PopThreadData(void)
{
	TypeThreadQueueNode* p;

	pthread_mutex_lock  (& _g.ThreadLock);       // <---- lock

	p = _g.ThreadQueue_pHead;
	if (_g.ThreadQueue_pHead) { _g.ThreadQueue_pHead = _g.ThreadQueue_pHead->pNext; }
	if (p == _g.ThreadQueue_pTail) { _g.ThreadQueue_pTail = NULL; }

	pthread_mutex_unlock(& _g.ThreadLock);       // <---- unlock

	return p;
}

static bool _bEmptyThreadData(void)
{
	bool b = true;
	pthread_mutex_lock  (& _g.ThreadLock);       // <---- lock

	if (_g.ThreadQueue_pHead) { b = false; }

	pthread_mutex_unlock(& _g.ThreadLock);       // <---- unlock
	return b;
}


static void _CleanThreadData(void)
{
	pthread_mutex_lock  (& _g.ThreadLock);       // <---- lock

	while (_g.ThreadQueue_pHead)
	{
		TypeThreadQueueNode* const p = _g.ThreadQueue_pHead;
		_g.ThreadQueue_pHead = p->pNext;
		free(p->pszTextUtf8);
		free(p);
	}
	_g.ThreadQueue_pHead = NULL;
	_g.ThreadQueue_pTail = NULL;

	pthread_mutex_unlock(& _g.ThreadLock);       // <---- unlock
}

static void * _Synthesize(void* NotUsed)
{
	TypeThreadQueueNode* p = _PopThreadData();

	for ( ; p ; p=_PopThreadData())
	{
		int  const   iVoiceInfo  = p->iVoiceInfo;
		char const * pszTextUtf8 = p->pszTextUtf8;
		void       * pUserParam  = p->pUserParam;

		if (_g.pfnCallback)
		{
			int r = SMT_SUCCESS;

			SLOG(LOG_DEBUG, TAG_TTSP, ">>>> Thread, _Synthesize(), iVoiceInfo = %d", iVoiceInfo);
			SLOG(LOG_DEBUG, TAG_TTSP, ">>>>>> pszTextUtf8 = %s", pszTextUtf8);

			if (iVoiceInfo != _g.iVoiceInfo)
			{
				r = _ChangeVoice(iVoiceInfo);
				SLOG(LOG_DEBUG, TAG_TTSP, ">>>>>> iVoiceInfo was changed.");
			}

			if (r == SMT_SUCCESS)
			{
				_SetSpeechSpeed();

				SLOG(LOG_DEBUG, TAG_TTSP, ">>>>>> Set speech-speed");

				pthread_mutex_lock  (& _g.MainThreadFinalizeLock);       // <---- lock

				r = _Synthesize_SamsungTTS(pszTextUtf8, pUserParam, SMT_FRAME_PERIOD );

				pthread_mutex_unlock(& _g.MainThreadFinalizeLock);       // <---- unlock

			}
			SLOG(LOG_DEBUG, TAG_TTSP, ">>>Thread, _Synthesize() done");
		}

		free(p->pszTextUtf8);
		free(p);
	} // end of while loop

	_g.ThreadId = -1;
	return (void*) 1;
}

//
// END
//
// functions managing < Thread Queue >
//








//
// functions managing < Engine Switching >
//
// BEGIN
//

static int _ChangeVoice(int const iVoiceInfo)
{
	if (iVoiceInfo < 0 || iVoiceInfo >= VOICE_INDEX_MAX) {
		SLOG(LOG_ERROR, TAG_TTSP, "[ERROR] Invalid Voice");
		return -1;
	}
	
	int r;

	char const * pszLanguage = _pVoiceInfos[iVoiceInfo].pszLanguage;


	_eTypeTTSMode TTSType =   eTTSMode_Normal;
	char Language[3] = {0,};
	char Country[3] = {0,};
	char VoiceType[2] = {0,};
	int VoiceIndex = 0;

	VoiceType[0] = 'f';
	VoiceType[1] = 0;
	memcpy(Language, pszLanguage, sizeof(Language)-1);
	memcpy(Country, pszLanguage+3, sizeof(Country)-1);

	int const eVoiceType = _pVoiceInfos[iVoiceInfo].eVoiceType;

	if(eVoiceType == TTSP_VOICE_TYPE_MALE) VoiceType[0] = 'm';

	char filepath[512] = {'\0',};
	if (VOICE_INDEX_KOREAN_WOMAN == iVoiceInfo || VOICE_INDEX_USENGLISH_WOMAN == iVoiceInfo
		|| VOICE_INDEX_GERMAN_WOMAN == iVoiceInfo || VOICE_INDEX_SPANISH_WOMAN == iVoiceInfo
		|| VOICE_INDEX_FRENCH_WOMAN == iVoiceInfo || VOICE_INDEX_ITALIAN_WOMAN == iVoiceInfo) {
			snprintf(filepath, 512, "%s", DATA_DIR1);
	} else {
		snprintf(filepath, 512, "%s%s_%s/", DATA_DIR2, pszLanguage, (TTSP_VOICE_TYPE_MALE == eVoiceType) ? "Male" : "Female");
	}

	SLOG(LOG_DEBUG, TAG_TTSP, "data path (%s)", filepath);

	if (VOICE_INDEX_KOREAN_MAN == iVoiceInfo) {
		VoiceIndex = 1;
	}

	if (0 == SMTCheckVoiceAvailable(TTSType, filepath, '/', Language, Country, VoiceType, VoiceIndex)) {
		if (0 != SMTSetVoice(TTSType, filepath, '/', Language, Country, VoiceType, VoiceIndex)) {
			SLOG(LOG_ERROR, TAG_TTSP, "[ERROR] Fail to set voice");
			return -1;
		}
	} else {
		SLOG(LOG_ERROR, TAG_TTSP, "[ERROR] Fail to check voice available");
		return -1;
	}

	_g.iVoiceInfo = iVoiceInfo;

	SMTFinalize();
	r = SMTInitialize();

	if (r != SMT_SUCCESS) { SLOG(LOG_DEBUG, TAG_TTSP, ">>>  _ChangeVoice() returns %d.", r); }
	else
	{
		_g.iSamplingRate = SMTGetSamplingRate();
	}

	return r;
}

static void _SetSpeechSpeed(void)
{
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>> Set speech speed %d", _g.eSpeechSpeed);
	switch (_g.eSpeechSpeed)
	{
	case 14	: SMTSetSpeechSpeed(eSMTSpeechSpeed_VeryFast);  break;
	case 11	: SMTSetSpeechSpeed(eSMTSpeechSpeed_Fast    );  break;
	case 5	: SMTSetSpeechSpeed(eSMTSpeechSpeed_Slow    );  break;
	case 2	: SMTSetSpeechSpeed(eSMTSpeechSpeed_VerySlow);  break;
		//	case TTSP_SPEED_NORMAL    : SMTSetSpeechSpeed(eSMTSpeechSpeed_Normal  );  break;
	case 8  : SMTSetSpeechSpeed(eSMTSpeechSpeed_Normal  );  break;
	}
}

static void _CallBack(ttsp_result_event_e eEvent, unsigned int const nPCMs, void * pPCMs, void* pUserParam)
{
	unsigned int const n = nPCMs * sizeof(short);


	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  callback, pUserParam = 0x%x", (unsigned int)(reinterpret_cast<long>(pUserParam)));
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  callback, event=");

	switch (eEvent)
	{
	case TTSP_RESULT_EVENT_CONTINUE :
		if (_g.bSentenceDone)
		{
			eEvent = TTSP_RESULT_EVENT_START;
			_g.bSentenceDone = false;
		}
		break;

	case TTSP_RESULT_EVENT_FINISH :
		_g.bSentenceDone = true;
		break;
	}

#if 0
	if (nPCMs)
	{
		static int iWave = 0;
		static char pszWave[100];

		SLOG(LOG_DEBUG, TAG_TTSP, "@@@ saving wave file @@@");
		//sprintf(pszWave, "/mnt/nfs/tts/play%d.wav", iWave++);
		SMTSaveWave(pszWave, (short*) pPCMs, nPCMs * 2);
	}
#endif

	switch (eEvent)
	{
	case TTSP_RESULT_EVENT_START   : SLOG(LOG_DEBUG, TAG_TTSP, "TTSPE_CBEVENT_SYNTH_START");     break;
	case TTSP_RESULT_EVENT_CONTINUE: SLOG(LOG_DEBUG, TAG_TTSP, "TTSPE_CBEVENT_SYNTH_CONTINUE");  break;
	case TTSP_RESULT_EVENT_FINISH  : SLOG(LOG_DEBUG, TAG_TTSP, "TTSPE_CBEVENT_SYNTH_FINISH");    break;
	case TTSP_RESULT_EVENT_FAIL    : SLOG(LOG_DEBUG, TAG_TTSP, "TTSPE_CBEVENT_SYNTH_FAIL");      break;
	default                          : SLOG(LOG_ERROR, TAG_TTSP, "invalid");                       break;
	}

	if (eEvent==TTSP_RESULT_EVENT_FINISH && ! _bEmptyThreadData())
	{
		SLOG(LOG_DEBUG, TAG_TTSP, ">>> There is another input text.");
		SLOG(LOG_DEBUG, TAG_TTSP, ">>> TTSPE_CBEVENT_SYNTH_FINISH was chanage into TTSPE_CBEVENT_SYNTH_CONTINUE.");
		eEvent = TTSP_RESULT_EVENT_CONTINUE;
	}

	SLOG(LOG_DEBUG, TAG_TTSP, ">>> data size = %d", n);
	SLOG(LOG_DEBUG, TAG_TTSP, ">> >> Here we jump into the callback function.");

	int cbreturn = -1;
	if(_g.pfnCallback != NULL){
		cbreturn = _g.pfnCallback(eEvent, pPCMs, n, TTSP_AUDIO_TYPE_RAW_S16, _g.iSamplingRate, pUserParam);
	}
	SLOG(LOG_DEBUG, TAG_TTSP, ">> >> Here we return from the callback function.");
	SLOG(LOG_DEBUG, TAG_TTSP, ">> >> callback function return value = %d", cbreturn);
	if (-1 == cbreturn)
	{
		SLOG(LOG_DEBUG, TAG_TTSP, ">>> Callback function returns TTS_CALLBACK_HALT.");
		std::lock_guard<std::mutex> lock( _g.bStopMutex );
		_g.bStop         = true;
		_g.bSentenceDone = true;
	}
}

static int _Synthesize_SamsungTTS(char const * const pszTextUtf8, void* pUserParam, int const FramePeriod)
{
	static short pPcmBuffer[PCM_BUFFER_SIZE] = {0,};

	int          r = SMTInputText(pszTextUtf8);
	unsigned int i = 0;

	if (r != SMT_SUCCESS)
	{
		_CleanThreadData();
		SLOG(LOG_DEBUG, TAG_TTSP, ">>>  SMTInputText() returns %d.", r);
	}


	while (r == SMT_SUCCESS)
	{
		r = SMTSynthesize(& pPcmBuffer[i]);
		std::lock_guard<std::mutex> lock( _g.bStopMutex );
		if (_g.bStop)
		{
			_CleanThreadData();
			break;
		}
		else
		{
			switch (r)
			{
			case SMT_SYNTHESIS_FRAME_GENERATED:
				if (i + FramePeriod == PCM_BUFFER_SIZE)
				{
					_CallBack(TTSP_RESULT_EVENT_CONTINUE, PCM_BUFFER_SIZE, pPcmBuffer, pUserParam);
					i = 0;
				}
				else
				{
					i += FramePeriod;
				}
				break;

			case SMT_SYNTHESIS_PAUSE_DONE    :
			case SMT_SYNTHESIS_SENTENCE_DONE :
				r = SMT_SUCCESS;
				break;

			case SMT_SYNTHESIS_ALL_DONE :
				_CallBack(TTSP_RESULT_EVENT_FINISH, i, pPcmBuffer, pUserParam);
				break;

			default :
				_CleanThreadData();

				SLOG(LOG_DEBUG, TAG_TTSP, ">>>  SMTSynthesize() returns %d", r);

				_CallBack(TTSP_RESULT_EVENT_FAIL, i, pPcmBuffer, pUserParam);
				break;
			}
		}

	} // end of while loop

	return r;
}

//
// END
//
// functions managing < Engine Switching >
//
