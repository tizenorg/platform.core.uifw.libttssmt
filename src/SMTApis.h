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

#ifndef   _SMTAPIS_H_
#define   _SMTAPIS_H_


#ifdef __cplusplus
extern "C" {
#endif

#define UINT unsigned int

typedef enum _eTypeLanguage
{
  eLANGUAGE_BEGIN       = 0x00000000,

  eKOREAN               = eLANGUAGE_BEGIN, // 한국어

  eUSENGLISH            = 0x00000100,      // 미국영어
  eUSENGLISH_OLD        = 0x0000010F,
  eGBENGLISH            = 0x00000110,      // 영국영어
  eGBENGLISH_OLD        = 0x0000011F,
  eINENGLISH            = 0x00000120,      // 인도영어

  eCHINESE              = 0x00000200,      // 중국어(북경어)
  eTAIWANESE            = 0x00000210,      // 대만어
  eCANTONESE            = 0x00000220,      // 광동어(홍콩어)

  eGERMAN               = 0x00000300,      // 독일어
  eGERMAN_OLD           = 0x0000030F,
  eSPANISH              = 0x00000400,      // 스페인어
  eSPANISH_OLD          = 0x0000040F,
  eFRENCH               = 0x00000500,      // 프랑스어
  eFRENCH_OLD           = 0x0000050F,
  eITALIAN              = 0x00000600,      // 이탈리아어
  eITALIAN_OLD          = 0x0000060F,
  eDUTCH                = 0x00000700,      // 네덜란드어
  ePORTUGUESE           = 0x00000800,      // 포르투칼어
  eGREEK                = 0x00000900,      // 그리스어
  eTURKISH              = 0x00000A00,      // 터키어

  eRUSSIAN              = 0x00000B00,      // 러시아어

  eJAPANESE             = 0x00000C00,      // 일본어
  eJAPANESE_OLD         = 0x00000C0F,

  ///////////////////////////////////////////
  eDICTIONARY_USENGLISH = 0x10000100,      // 영어사전

  eLANGUAGE_END,
  eLANGUAGE_INVALID     = eLANGUAGE_END,
} eTypeLanguage;


#define SMT_SUCCESS                    0
#define SMT_SYNTHESIS_FRAME_GENERATED  0
#define SMT_SYNTHESIS_ALL_DONE         1
#define SMT_SYNTHESIS_SENTENCE_DONE    2
#define SMT_SYNTHESIS_PAUSE_DONE       3

typedef enum _eTypeTTSMode
{
  eTTSMode_Normal,
  eTTSMode_Dictionary,
  
  ///////////////////////////////////////////
  
  eTTSMode_INVALID,
} eTypeTTSMode;

typedef enum
{
  eSMTSpeechSpeed_VerySlow,
  eSMTSpeechSpeed_Slow,
  eSMTSpeechSpeed_Normal,
  eSMTSpeechSpeed_Fast,
  eSMTSpeechSpeed_VeryFast,
} etypeSpeechSpeed;

int SMTSetVoice           (eTypeTTSMode const eTTSMode, char const * const pszFilePath, char const DirDelimiter, char const * const pszLanguage, char const * const pszContry, char const * const pszVoiceType, UINT const VoiceIndex);
int SMTCheckVoiceAvailable(eTypeTTSMode const eTTSMode, char const * const pszFilePath, char const DirDelimiter, char const * const pszLanguage, char const * const pszContry, char const * const pszVoiceType, UINT const VoiceIndex);


int SMTInitialize(void);
int SMTFinalize(void);
int SMTSet_Language(eTypeLanguage eLanguage, int VoiceType, int ModelFileLocation);
int SMTInputText(char const * const pszText_UTF8);
int SMTSynthesize(short * const pPcmBuffer);

int  SMTGetFramePeriod (void);

void SMTSetSpeechSpeed(etypeSpeechSpeed const speed);

int SMTSaveWave(char* pszFileName, short* pOutData, int const nDataSize);

#ifdef __cplusplus
}
#endif

#endif //_SMTAPIS_H_


