/*
 * Samsung TTS
 * Copyright 2012  Samsung Electronics Co., Ltd
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
	printf(">>>plugin Initialize() returns %d.\n", r);
	return r;
}

int plugin_Finalize(void)
{
	int r = SLPSMT_Finalize();
	printf(">>>plugin Initialize() returns %d.\n", r);
	return r;
}

int plugin_SynthesizeText(char* const   pszLanguage, ttsp_voice_type_e eVoiceType_0,  char const * pszTextUtf8,
			ttsp_speed_e eSpeechSpeed_0, void* pUserParam)
{
	int               iVoiceInfo   = -1;
	ttsp_voice_type_e eVoiceType   = eVoiceType_0;
	ttsp_speed_e     eSpeechSpeed = eSpeechSpeed_0;

	int ThreadId = SLPSMT_GetWorkingThreadId();
	printf(">>>plugin SynthesizeText()\n");
	if (ThreadId != -1)
	{
	printf(">>>  A new text is comming into the TTS engine, while Synthesizing.\n");
	printf(">>>  The text would be pushed into the thread queue.\n");
	}
	printf(">>>  ThreadId = %d\n", ThreadId);
	printf(">>>  pUserParam = 0x%x\n", (unsigned int) pUserParam);

	printf(">>>  eSpeechSpeed = %s\n", //eSpeechSpeed==TTSPE_SPEED_DEFAULT   ? "TTSPE_SPEED_DEFAULT"   :
				     eSpeechSpeed==TTSP_SPEED_VERY_FAST ? "TTSP_SPEED_VERY_FAST" :
				     eSpeechSpeed==TTSP_SPEED_FAST      ? "TTSP_SPEED_FAST"      :
				     eSpeechSpeed==TTSP_SPEED_NORMAL    ? "TTSP_SPEED_NORMAL"    :
				     eSpeechSpeed==TTSP_SPEED_SLOW      ? "TTSP_SPEED_SLOW"      :
				     eSpeechSpeed==TTSP_SPEED_VERY_SLOW ? "TTSP_SPEED_VERY_SLOW" : ""
	);
	printf(">>>  pszLanguage  = %s\n", pszLanguage);
	printf(">>>  eVoiceType   = %s\n", //eVoiceType==TTSPE_VCTYPE_DEFAULT ? "TTSPE_VCTYPE_DEFAULT" :
				     eVoiceType==TTSP_VOICE_TYPE_MALE    ? "TTSP_VOICE_TYPE_MALE"    :
				     eVoiceType==TTSP_VOICE_TYPE_FEMALE  ? "TTSP_VOICE_TYPE_FEMALE"  :
				     eVoiceType==TTSP_VOICE_TYPE_CHILD   ? "TTSP_VOICE_TYPE_CHILD"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER1   ? "TTSP_VOICE_TYPE_USER1"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER2   ? "TTSP_VOICE_TYPE_USER2"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER3   ? "TTSP_VOICE_TYPE_USER3"   : ""
	);

	SLPSMT_SetSpeechSpeed(eSpeechSpeed);

	printf(">>>  \n");
	printf(">>>  eSpeechSpeed = %s\n", //eSpeechSpeed==TTSP_SPEED_DEFAULT   ? "TTSPE_SPEED_DEFAULT"   :
				     eSpeechSpeed==TTSP_SPEED_VERY_FAST ? "TTSPE_SPEED_VERY_FAST" :
				     eSpeechSpeed==TTSP_SPEED_FAST      ? "TTSPE_SPEED_FAST"      :
				     eSpeechSpeed==TTSP_SPEED_NORMAL    ? "TTSPE_SPEED_NORMAL"    :
				     eSpeechSpeed==TTSP_SPEED_SLOW      ? "TTSPE_SPEED_SLOW"      :
				     eSpeechSpeed==TTSP_SPEED_VERY_SLOW ? "TTSPE_SPEED_VERY_SLOW" : ""
	);
	printf(">>>  pszLanguage  = %s\n", pszLanguage);
	printf(">>>  eVoiceType   = %s\n", //eVoiceType==TTSPE_VCTYPE_DEFAULT ? "TTSPE_VCTYPE_DEFAULT" :
				     eVoiceType==TTSP_VOICE_TYPE_MALE    ? "TTSPE_VCTYPE_MALE"    :
				     eVoiceType==TTSP_VOICE_TYPE_FEMALE  ? "TTSPE_VCTYPE_FEMALE"  :
				     eVoiceType==TTSP_VOICE_TYPE_CHILD   ? "TTSPE_VCTYPE_CHILD"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER1   ? "TTSPE_VCTYPE_USER1"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER2   ? "TTSPE_VCTYPE_USER2"   :
				     eVoiceType==TTSP_VOICE_TYPE_USER3   ? "TTSPE_VCTYPE_USER3"   : ""
	);

	// set voice
	iVoiceInfo = SLPSMT_GetiVoiceInfoEx(pszLanguage, eVoiceType);
	if (iVoiceInfo < 0)
	{
		printf("iVoiceInfo < 0\n");
		return TTSP_ERROR_INVALID_VOICE;
	}

	return SLPSMT_SynthesizeText(iVoiceInfo, pszTextUtf8, pUserParam);
}

int plugin_StopSynthesis(void)
{
	return SLPSMT_StopSynthesis();
}

int plugin_GetAudioFormat(ttsp_audio_type_e* pType, int* pSamplingRate, int* pnChannels)
{
	if (pType && pSamplingRate && pnChannels)
	{
		*pSamplingRate  = 16000;
		*pnChannels     = 1;
		*pType          = TTSP_AUDIO_TYPE_RAW;
		return TTSP_ERROR_NONE;
	}
	return TTSP_ERROR_INVALID_PARAMETER;
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
			printf(">> plugin_ForeachVoices(), callback fail!!\n");
			break;
		}
	}

	return TTSP_ERROR_NONE;
}

bool plugin_IsValidVoice(const char* language, ttsp_voice_type_e type)
{
	if (NULL == language)
		return TTSP_ERROR_INVALID_PARAMETER;

	int iVoiceInfo = SLPSMT_GetiVoiceInfoEx((char* )language, type);

	if (iVoiceInfo < 0)
	{
		printf("iVoiceInfo < 0\n");
		return false;
	}

	return true;
}

int plugin_SetSettingInfo(const char* key, const char* value)
{
	if (NULL == key || NULL == value) {
		return TTSP_ERROR_INVALID_PARAMETER;
	}

	printf("Set engine setting : key(%s) value(%s)\n", key, value);

	return TTSP_ERROR_NONE;
}

int plugin_ForeachEngineSetting(ttspe_engine_setting_cb callback, void* user_data)
{
	if (false == callback("Sample_key1", "Sample_value1", user_data))
		return TTSP_ERROR_NONE;

	if (false == callback("Sample_key2", "Sample_value2", user_data))
		return TTSP_ERROR_NONE;

	return TTSP_ERROR_NONE;
}

void plugin_SetDaemonAPIs(ttspd_funcs_s const * pAPIs)
{
	gpDaemonAPIs = pAPIs;
}
