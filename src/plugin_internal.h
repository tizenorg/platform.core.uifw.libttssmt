/*
 * Samsung TTS
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the License);
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


#ifndef _PLUGIN_INTERNAL_H_
#define _PLUGIN_INTERNAL_H_

#include <ttsp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENGINE_UUID    "27F277E9-BBC4-4dca-B553-D9884A3CDAA0"
#define ENGINE_NAME    "Samsung TTS"
#define ENGINE_UG_NAME "tts_smt_setting_ug.so"

#define VTTS_VOICE_NAME_BUF_SIZE 8

typedef char*	ttspe_language;

typedef struct {
	ttspe_language    lang;    /**< Language */
	ttsp_voice_type_e vctype;  /**< Voice type */
} ttspe_voice_info_s;

typedef struct {
	ttspe_voice_info_s* voice_info;  /**< Voice info array */
	int                 size;        /**< size of array */
} ttspe_voice_list_s;

int plugin_Initialize(ttspe_result_cb const cb_func);

int plugin_Finalize(void);

int plugin_SynthesizeText
(
  char*    const   pszLanguage,
  ttsp_voice_type_e const   eVoiceType,
  char              const * pszTextUtf8,
  ttsp_speed_e     const   eSpeechSpeed,
  void                    * pUserParam
);

int plugin_StopSynthesis(void);

int plugin_GetAudioFormat(ttsp_audio_type_e* pType, int* pSamplingRate, int* pnChannels);

int plugin_ForeachVoices(ttspe_supported_voice_cb callback, void* user_data);

bool plugin_IsValidVoice(const char* language, ttsp_voice_type_e type);

int plugin_SetDefaultSpeechSpeed(ttsp_speed_e const  SpeechSpeed);

int plugin_SetSettingInfo(const char* key, const char* value);

int plugin_ForeachEngineSetting(ttspe_engine_setting_cb callback, void* user_data);

void plugin_SetDaemonAPIs (ttspd_funcs_s const * pAPIs);



#ifdef __cplusplus
}
#endif

#endif /* _PLUGIN_INTERNAL_H_ */
