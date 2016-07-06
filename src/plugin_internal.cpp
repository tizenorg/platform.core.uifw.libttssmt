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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plugin_internal.h"
#include "_SlpSmtApis.h"
#include <string>
#include <libxml/parser.h>
#include <glib.h>
#include <unistd.h>
#include <tzplatform_config.h>

#define TTSSMT_INFO_XML_PATH tzplatform_mkpath(TZ_USER_HOME, "/share/.voice/tts/engine-info/ttssmt-info.xml")

#define TTSSMT_INFO_TAG_TTS_ENGINE	"tts-engine"
#define TTSSMT_INFO_TAG_NAME		"name"
#define TTSSMT_INFO_TAG_ID		"id"
#define TTSSMT_INFO_TAG_SETTING		"setting"
#define TTSSMT_INFO_TAG_VOICES		"voices"
#define TTSSMT_INFO_TAG_VOICE		"voice"
#define TTSSMT_INFO_TAG_VOICE_TYPE	"type"
#define TTSSMT_INFO_TAG_VOICE_TYPE_FEMALE	"female"
#define TTSSMT_INFO_TAG_VOICE_TYPE_MALE		"male"
#define TTSSMT_INFO_TAG_VOICE_TYPE_CHILD	"child"

using namespace std;

// Once eigine was initialize, that means gpDaemonAPIs and its members are not NULL.
// So we do not need to check whether this is NULL, nor its members are NULL.
static ttspd_funcs_s const * gpDaemonAPIs = NULL;

static std::string g_current_lang;

GSList* g_ttsinfo_list = NULL;

static int _plugin_get_language_info(void);
static int _plugin_free_language_info(void);
static void _plugin_parse_tts_config(void);

int plugin_Initialize(ttspe_result_cb const pfnCallBack)
{
	int r = SLPSMT_Initialize(pfnCallBack);
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>plugin Initialize() returns %d.", r);
	_plugin_get_language_info();
	return r;
}

int plugin_Finalize(void)
{
	int r = SLPSMT_Finalize();
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>plugin Initialize() returns %d.", r);
	_plugin_free_language_info();
	return r;
}

int plugin_SynthesizeText(char const *pszLanguage, int eVoiceType_0,  char const *pszTextUtf8,
			int eSpeechSpeed_0, const char* credential, void* pUserParam)
{
	int iVoiceInfo   = -1;
	int eVoiceType   = eVoiceType_0;
	int eSpeechSpeed = eSpeechSpeed_0;

	int ThreadId = SLPSMT_GetWorkingThreadId();
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>plugin SynthesizeText()");
	if (ThreadId != -1)
	{
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  A new text is comming into the TTS engine, while Synthesizing.");
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  The text would be pushed into the thread queue.");
	}
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  ThreadId = %d", ThreadId);
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  pUserParam = 0x%x", (unsigned int)(reinterpret_cast<long>(pUserParam)));

	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  eSpeechSpeed = %d", eSpeechSpeed);
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  pszLanguage  = %s", pszLanguage);
	SLOG(LOG_DEBUG, TAG_TTSP, ">>>  eVoiceType   = %s", //eVoiceType==TTSPE_VCTYPE_DEFAULT ? "TTSPE_VCTYPE_DEFAULT" :
				     eVoiceType==TTSP_VOICE_TYPE_MALE    ? "TTSP_VOICE_TYPE_MALE"    :
				     eVoiceType==TTSP_VOICE_TYPE_FEMALE  ? "TTSP_VOICE_TYPE_FEMALE"  :
				     eVoiceType==TTSP_VOICE_TYPE_CHILD   ? "TTSP_VOICE_TYPE_CHILD"   : ""
	);

	SLPSMT_SetSpeechSpeed(eSpeechSpeed);

	// set voice
	iVoiceInfo = SLPSMT_GetiVoiceInfo(pszLanguage, eVoiceType);
	if (iVoiceInfo < 0)
	{
		SLOG(LOG_ERROR, TAG_TTSP, "iVoiceInfo < 0");
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


	if(!g_ttsinfo_list){
		return -1;
	}

	GSList* iter;

	for (iter = g_ttsinfo_list; iter != NULL; iter = g_slist_next(iter)) {
		ttspe_voice_info_s *lang_info = (ttspe_voice_info_s*)iter->data;
                if (NULL == lang_info)
                        continue;
                if (NULL == lang_info->lang)
                        continue;

		char* lang = strdup(lang_info->lang);
		int vctype = lang_info->vctype;

		SLOG(LOG_DEBUG, TAG_TTSP, "[ %s, %d ]", lang, vctype);

		if (false == callback(lang, vctype, user_data))
		{
			SLOG(LOG_ERROR, TAG_TTSP, ">> plugin_ForeachVoices(), callback fail!!");
                        if (lang)
                                free(lang);
			break;
		}

		if(lang)
			free(lang);
	}

	return TTSP_ERROR_NONE;
}

bool plugin_IsValidVoice(const char* language, int type)
{
	if (NULL == language)
		return false;

	if(!g_ttsinfo_list){
		SLOG(LOG_ERROR, TAG_TTSP, "ttsinfo is NULL");
		return false;
	}


	GSList* iter;

	for (iter = g_ttsinfo_list; iter != NULL; iter = g_slist_next(iter)) {
		ttspe_voice_info_s *lang_info = (ttspe_voice_info_s*)iter->data;
                if (NULL == lang_info)
                        continue;
                if (NULL == lang_info->lang)
                        continue;

		if(!strcmp(lang_info->lang, language) && type == lang_info->vctype){
			SLOG(LOG_DEBUG, TAG_TTSP, "[ %s, %d ] IS VALID", lang_info->lang, lang_info->vctype);
			return true;
		}
	}

	SLOG(LOG_DEBUG, TAG_TTSP, "[ %s, %d ] NOT EXIST", language, type);
	return false;
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

int plugin_SetPrivateData(const char* key, const char* data)
{
	return 0;
}

int plugin_GetPrivateData(const char* key, char** data)
{
	if (NULL != key) {
		if (!strcmp(key, "version")) {
			*data = strdup("1.0");
		}
	}

	return 0;
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
static void _plugin_parse_tts_config(void)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	xmlChar *key;

	SLOG(LOG_DEBUG, TAG_TTSP, "tts xml path (%s)", TTSSMT_INFO_XML_PATH);

	doc = xmlParseFile(TTSSMT_INFO_XML_PATH);
	if (doc == NULL) {
		SLOG(LOG_ERROR, TAG_TTSP, " Fail to parse file error : %s", TTSSMT_INFO_XML_PATH);
		return ;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		SLOG(LOG_ERROR, TAG_TTSP, "Empty document");
		xmlFreeDoc(doc);
		return ;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) TTSSMT_INFO_TAG_TTS_ENGINE)) {
		SLOG(LOG_ERROR, TAG_TTSP, "The wrong type, root node is NOT %s", TTSSMT_INFO_TAG_TTS_ENGINE);
		xmlFreeDoc(doc);
		return ;
	}

	cur = cur->xmlChildrenNode;
	if (cur == NULL) {
		SLOG(LOG_ERROR, TAG_TTSP, "<tts-engine> child NULL");
		xmlFreeDoc(doc);
		return ;
	}

	while (cur != NULL) {
		if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTSSMT_INFO_TAG_NAME)) {
			key = xmlNodeGetContent(cur);
			if (NULL != key) {
				SLOG(LOG_DEBUG, TAG_TTSP, "<name>%s</name>", (char *)key);
				xmlFree(key);
			} else {
				SLOG(LOG_ERROR, TAG_TTSP, "setting path is NULL");
			}
		} else if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTSSMT_INFO_TAG_ID)) {
			key = xmlNodeGetContent(cur);
			if (NULL != key) {
				SLOG(LOG_DEBUG, TAG_TTSP, "<id>%s<id>", (char *)key);
				xmlFree(key);
			} else {
				SLOG(LOG_ERROR, TAG_TTSP, "setting path is NULL");
			}
		} else if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTSSMT_INFO_TAG_SETTING)) {
			key = xmlNodeGetContent(cur);
			if (NULL != key) {
				SLOG(LOG_DEBUG, TAG_TTSP, "<setting>%s</setting>", (char *)key);
				xmlFree(key);
			} else {
				SLOG(LOG_ERROR, TAG_TTSP, "setting path is NULL");
			}
		} else if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTSSMT_INFO_TAG_VOICES)) {

			SLOG(LOG_DEBUG, TAG_TTSP, "<voices>");
			xmlNodePtr voice_node = NULL;
			voice_node = cur->xmlChildrenNode;

			xmlChar *attr = NULL;

			while (NULL != voice_node) {
				if (0 == xmlStrcmp(voice_node->name, (const xmlChar *)TTSSMT_INFO_TAG_VOICE)) {

					char current_language[16] = {0, };
					int current_voicetype = -1;

					attr = xmlGetProp(voice_node, (const xmlChar*)TTSSMT_INFO_TAG_VOICE_TYPE);
					if (NULL != attr) {
						if (0 == xmlStrcmp(attr, (const xmlChar *)TTSSMT_INFO_TAG_VOICE_TYPE_FEMALE)) {
							key = xmlNodeGetContent(voice_node);
							if (NULL != key) {
								SLOG(LOG_DEBUG, TAG_TTSP, "<voice type=%s>",  key);
							}

							current_voicetype = TTSP_VOICE_TYPE_FEMALE;
						} else if (0 == xmlStrcmp(attr, (const xmlChar *)TTSSMT_INFO_TAG_VOICE_TYPE_MALE)) {
							key = xmlNodeGetContent(voice_node);
							if (NULL != key) {
								SLOG(LOG_DEBUG, TAG_TTSP, "<voice type=%s>",  key);
							}
							current_voicetype = TTSP_VOICE_TYPE_MALE;
						} else if (0 == xmlStrcmp(attr, (const xmlChar *)TTSSMT_INFO_TAG_VOICE_TYPE_CHILD)) {
							key = xmlNodeGetContent(voice_node);
							if (NULL != key) {
								SLOG(LOG_DEBUG, TAG_TTSP, "<voice type=%s>",  key);
							}
							current_voicetype = TTSP_VOICE_TYPE_CHILD;
						} else {
							SLOG(LOG_DEBUG, TAG_TTSP, "user defined voice type");
							// fallback
							current_voicetype = TTSP_VOICE_TYPE_FEMALE;
						}

						xmlFree(attr);
					} else {
						SLOG(LOG_DEBUG, TAG_TTSP, "<%s> has no content", TTSSMT_INFO_TAG_VOICE_TYPE);
					}

					key = xmlNodeGetContent(voice_node);
					if (NULL != key) {
						strncpy(current_language, (char*)key, strlen((char*)key));
						xmlFree(key);
					} else {
						SLOG(LOG_DEBUG, TAG_TTSP, "<%s> has no content", TTSSMT_INFO_TAG_VOICE);
					}

					ttspe_voice_info_s *lang_info = (ttspe_voice_info_s*)malloc(sizeof(ttspe_voice_info_s));
                                        if (NULL == lang_info) {
                                                SLOG(LOG_ERROR, TAG_TTSP, "[ERROR] Fail to memory allocation");
                                                xmlFreeDoc(doc);
                                                return;
                                        }
					if(!strcmp(current_language, "no-NO")){
						lang_info->lang = strdup("nb-NO");
					} else if(!strcmp(current_language, "no_NO")){
						lang_info->lang = strdup("nb_NO");
					} else {
						lang_info->lang = strdup(current_language);
					}
					lang_info->vctype = current_voicetype;

					SLOG(LOG_DEBUG, TAG_TTSP, "Language (%s) Voicetype (%d)", lang_info->lang, lang_info->vctype);

					g_ttsinfo_list = g_slist_append(g_ttsinfo_list, lang_info);
				}

				voice_node = voice_node->next;
			}

		} else {
			SLOG(LOG_DEBUG, TAG_TTSP, "[WARNING] unknown tag (%s)", (const char*)cur->name );
		}

		cur = cur->next;
	}

	xmlFreeDoc(doc);
}


static int _plugin_get_language_info(void)
{
	_plugin_parse_tts_config();
	return 0;
}

static int _plugin_free_language_info()
{
	if(!g_ttsinfo_list){
		return -1;
	}

	GSList* iter;

	for (iter = g_ttsinfo_list; iter != NULL; iter = g_slist_next(iter)) {
		ttspe_voice_info_s *lang_info = (ttspe_voice_info_s*)iter->data;
		if(lang_info) {
			if(lang_info->lang)
				free(lang_info->lang);

			free(lang_info);
		}
	}

	g_slist_free(g_ttsinfo_list);
	g_ttsinfo_list = NULL;

	return 0;
}

