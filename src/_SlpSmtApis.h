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

#ifndef   __SLP_SMT_APIS_H__
#define   __SLP_SMT_APIS_H__

#include <ttsp.h>
#include "plugin_internal.h"

int  SLPSMT_Initialize    (ttspe_result_cb const pfnCallBack);
int  SLPSMT_Finalize      (void);

int  SLPSMT_SetVoiceList  (ttspe_voice_list_s *);
void SLPSMT_SetSpeechSpeed(ttsp_speed_e const eSpeechSpeed);
int  SLPSMT_GetiVoiceInfo (char* const pszLanguage, ttsp_voice_type_e const eVoiceType);
int  SLPSMT_GetiVoiceInfoEx(char* const pszLanguage, ttsp_voice_type_e const eVoiceType);
int  SLPSMT_SynthesizeText(int const iVoiceInfo, char const * pszTextUtf8, void* const pUserParam);
int  SLPSMT_StopSynthesis (void);

char const * SLPSMT_GetPszKorean              (void);
char const * SLPSMT_GetPszUSEnglish           (void);
char const * SLPSMT_GetPszGerman              (void);
char const * SLPSMT_GetPszSpanish             (void);
char const * SLPSMT_GetPszFrench              (void);
char const * SLPSMT_GetPszItalian             (void);
int          SLPSMT_GetWorkingThreadId        (void);


#endif // __SLP_SMT_APIS_H__
