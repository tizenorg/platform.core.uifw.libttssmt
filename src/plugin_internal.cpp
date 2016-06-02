/*
 * Samsung TTS
 * Copyright 2012-2014  Samsung Electronics Co., Ltd
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plugin_internal.h"
#include "_SlpSmtApis.h"


// Once eigine was initialize, that means gpDaemonAPIs and its members are not NULL.
// So we do not need to check whether this is NULL, nor its members are NULL.
static ttspd_funcs_s const * gpDaemonAPIs = NULL;


int plugin_Initialize(ttspe_result_cb const pfnCallBack)
{
	int r = SLPSMT_Initialize(pfnCallBack);
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>plugin Initialize() returns %d.", r);
	return r;
}

int plugin_Finalize(void)
{
	int r = SLPSMT_Finalize();
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>plugin Initialize() returns %d.", r);
	return r;
}

int plugin_SynthesizeText(char const *pszLanguage, int eVoiceType_0,  char const *pszTextUtf8,
			int eSpeechSpeed_0, const char* credential, void* pUserParam)
{
	int iVoiceInfo   = -1;
	int eVoiceType   = eVoiceType_0;
	int eSpeechSpeed = eSpeechSpeed_0;

	int ThreadId = SLPSMT_GetWorkingThreadId();
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>plugin SynthesizeText()");
	if (ThreadId != -1)
	{
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  A new text is comming into the TTS engine, while Synthesizing.");
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  The text would be pushed into the thread queue.");
	}
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  ThreadId = %d", ThreadId);
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  pUserParam = 0x%x", (unsigned int)(reinterpret_cast<long>(pUserParam)));

	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  eSpeechSpeed = %d", eSpeechSpeed);
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  pszLanguage  = %s", pszLanguage);
	SLOG(LOG_DEBUG, TTSP_TAG, ">>>  eVoiceType   = %s", //eVoiceType==TTSPE_VCTYPE_DEFAULT ? "TTSPE_VCTYPE_DEFAULT" :
				     eVoiceType==TTSP_VOICE_TYPE_MALE    ? "TTSP_VOICE_TYPE_MALE"    :
				     eVoiceType==TTSP_VOICE_TYPE_FEMALE  ? "TTSP_VOICE_TYPE_FEMALE"  :
				     eVoiceType==TTSP_VOICE_TYPE_CHILD   ? "TTSP_VOICE_TYPE_CHILD"   : ""
	);

	SLPSMT_SetSpeechSpeed(eSpeechSpeed);

	// set voice
	iVoiceInfo = SLPSMT_GetiVoiceInfoEx(pszLanguage, eVoiceType);
	if (iVoiceInfo < 0)
	{
		SLOG(LOG_ERROR, TTSP_TAG, "iVoiceInfo < 0");
		return TTSP_ERROR_INVALID_VOICE;
	}

	return SLPSMT_SynthesizeText(iVoiceInfo, pszTextUtf8, pUserParam);
}

int plugin_StopSynthesis(void)
{
	return SLPSMT_StopSynthesis();
}

int plugin_ForeachVoices(ttspe_supported_voice_cb callback, void* user_data)
{
	if(NULL == callback)
		return TTSP_ERROR_INVALID_PARAMETER;

	ttspe_voice_list_s pVoiceList;
	SLPSMT_SetVoiceList(&pVoiceList);

	int i = 0;

	ttspe_voice_info_s* temp = pVoiceList.voice_info;

	for(i=0 ; i < pVoiceList.size; i++)
	{
		if (false == callback(temp[i].lang, temp[i].vctype, user_data))
		{
			SLOG(LOG_ERROR, TTSP_TAG, ">> plugin_ForeachVoices(), callback fail!!");
			break;
		}
	}

	return TTSP_ERROR_NONE;
}

bool plugin_IsValidVoice(const char* language, int type)
{
	if (NULL == language)
		return TTSP_ERROR_INVALID_PARAMETER;

	int iVoiceInfo = SLPSMT_GetiVoiceInfoEx((char* )language, type);

	if (iVoiceInfo < 0)
	{
		SLOG(LOG_ERROR, TTSP_TAG, "iVoiceInfo < 0");
		return false;
	}

	return true;
}

int plugin_LoadVoice(const char* language, int type)
{
	return 0;
}

int plugin_UnloadVoice(const char* language, int type)
{
	return 0;
}

bool plugin_NeedAppCredential(void)
{
	return false;
}

void plugin_SetDaemonAPIs(ttspd_funcs_s const * pAPIs)
{
	gpDaemonAPIs = pAPIs;

	int min, normal, max;

	gpDaemonAPIs->get_speed_range(&min, &normal, &max);

	printf("[SMT] Speed range : min(%d) normal(%d) max(%d)\n", min, normal, max);

	gpDaemonAPIs->get_pitch_range(&min, &normal, &max);

	printf("[SMT] Pitch range : min(%d) normal(%d) max(%d)\n", min, normal, max);

}
