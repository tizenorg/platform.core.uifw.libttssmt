/*
*  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved 
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*  http://www.apache.org/licenses/LICENSE-2.0
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ttssmt_setting_main.h"

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *naviframe;
	Evas_Object *genlist;

	Evas_Object *progress_popup;
	Evas_Object *delete_popup;
	Elm_Object_Item *selected_lang;
} appdata_s;

typedef enum {
	DOWNLOAD_TITLE = 0,
	CS_CZ_FEMALE,
	DA_DK_FEMALE,
	DE_DE_FEMALE,
	DE_DE_MALE,
	EL_GR_MALE,
	EN_GB_FEMALE,
	EN_GB_MALE,
	EN_IN_FEMALE,
	EN_US_FEMALE,
	EN_US_MALE,
	ES_ES_FEMALE,
	ES_ES_MALE,
	ES_MX_FEMALE,
	ES_MX_MALE,
	FI_FI_FEMALE,
	FR_FR_FEMALE,
	FR_FR_MALE,
	HI_IN_FEMALE,
	HU_HU_FEMALE,
	IT_IT_FEMALE,
	IT_IT_MALE,
	JA_JP_FEMALE,
	JA_JP_MALE,
	KO_KR_FEMALE,
	KO_KR_MALE,
	NB_NO_FEMALE,
	NL_NL_FEMALE,
	PL_PL_FEMALE,
	PT_BR_FEMALE,
	PT_BR_MALE,
	PT_PT_FEMALE,
	PT_PT_MALE,
	RU_RU_FEMALE,
	SK_SK_FEMALE,
	SV_SE_FEMALE,
	TR_TR_FEMALE,
	ZH_CN_FEMALE,
	ZH_CN_MALE,
	ZH_HK_FEMALE,
	ZH_HK_MALE,
	ZH_TW_FEMALE,
	END_INDEX
} item_index_e;

/* Todo - update language name */
static char* item_data[END_INDEX][5] = {
	{"Download (via network)", "", "", "", ""},
	{"\xC4\x8C\x65\xC5\xA1\x74\x69\x6e\x61 - \xC5\xBD\x65\x6e\x61", "cs_CZ_Female", "libttssmt-cs_CZ_Female", "cs_CZ", "Female"},
	{"Dansk - Kvinde", "da_DK_Female", "libttssmt-da_DK_Female", "da_DK", "Famale"},
	{"\x44\x65\x75\x74\x73\x63\x68 - Weiblich", "de_DE_Female", "libttssmt-de_DE_Female", "de_DE", "Female"},
	{"\x44\x65\x75\x74\x73\x63\x68 - Mannlich", "de_DE_Male", "libttssmt-de_DE_Male", "de_DE", "Male"},
	{"\xCE\x95\xCE\xBB\xCE\xBB\xCE\xB7\xCE\xBD\xCE\xB9\xCE\xBA\xCE\xAC - \xCE\x98\xCE\xB7\xCE\xBB\xCF\x85\xCE\xBA\xCF\x8C", "el_GR_Female", "libttssmt-el_GR_Female", "el_GR", "Female"},
	{"English (United Kingdom) - Female", "en_GB_Female", "libttssmt-en_GB_Female", "en_GB", "Female"},
	{"English (United Kingdom) - Male", "en_GB_Male", "libttssmt-en_GB_Male", "en_GB", "Male"},
	{"English (India) - Female", "en_IN_Female", "libttssmt-en_IN_Female", "en_IN", "Famale"},
	{"\x45\x6E\x67\x6C\x69\x73\x68 - Female", "en_US_Female", "libttssmt-en_US_Female", "en_US", "Female"},
	{"\x45\x6E\x67\x6C\x69\x73\x68 - Male", "en_US_Male", "libttssmt-en_US_Male", "en_US", "Male"},
	{"\x45\x73\x70\x61\xC3\xB1\x6F\x6C - Mujer", "es_ES_Female", "libttssmt-es_ES_Female", "es_ES", "Female"},
	{"\x45\x73\x70\x61\xC3\xB1\x6F\x6C - Hombre", "es_ES_Male", "libttssmt-es_ES_Male", "es_ES", "Male"},
	{"\x45\x73\x70\x61\xC3\xB1\x6f\x6c\x20\x28\x45\x73\x74\x61\x64\x6f\x73\x20\x55\x6e\x69\x64\x6f\x73\x29 - Mujer", "es_MX_Female", "libttssmt-es_MX_Female", "es_MX", "Female"},
	{"\x45\x73\x70\x61\xC3\xB1\x6f\x6c\x20\x28\x45\x73\x74\x61\x64\x6f\x73\x20\x55\x6e\x69\x64\x6f\x73\x29 - Hombre", "es_MX_Male", "libttssmt-es_MX_Male", "es_MX", "Male"},
	{"Suomi - Nainen ", "fi_FI_Female", "libttssmt-fi_FI_Female", "fi_FI", "Female"},
	{"\x46\x72\x61\x6E\xC3\xA7\x61\x69\x73 - Femme", "fr_FR_Female", "libttssmt-fr_FR_Female", "fr_FR", "Female"},
	{"\x46\x72\x61\x6E\xC3\xA7\x61\x69\x73 - Homme", "fr_FR_Male", "libttssmt-fr_FR_Male", "fr_FR", "Male"},
	{"\xE0\xA4\xB9\xE0\xA4\xBF\xE0\xA4\xA8\xE0\xA5\x8D\xE0\xA4\xA6\xE0\xA5\x80 - \xE0\xA4\xB8\xE0\xA5\x8D\xE0\xA4\xA4\xE0\xA5\x8D\xE0\xA4\xB0\xE0\xA5\x80", "hi_IN_Female", "libttssmt-hi_IN_Female", "hi_IN", "Female"},
	{"Magyar - \x4e\xC5\x91", "hu_HU_Female", "libttssmt-hu_HU_Female", "hu_HU", "Female"},
	{"\x49\x74\x61\x6C\x69\x61\x6E\x6F - Femmina", "it_IT_Female", "libttssmt-it_IT_Female", "it_IT", "Female"},
	{"\x49\x74\x61\x6C\x69\x61\x6E\x6F - Maschio", "it_IT_Male", "libttssmt-it_IT_Male", "it_IT", "Male"},
	{"\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E - \xE5\xA5\xB3\xE6\x80\xA7", "ja_JP_Female", "libttssmt-ja_JP_Female", "ja_JP", "Female"},
	{"\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E - \xE7\x94\xB7\xE6\x80\xA7", "ja_JP_Male", "libttssmt-ja_JP_Male", "ja_JP", "Male"},
	{"\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4 - \xEC\x97\xAC", "ko_KR_Female", "libttssmt-ko_KR_Female", "ko_KR", "Female"},
	{"\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4 - \xEB\x82\xA8", "ko_KR_Male", "libttssmt-ko_KR_Male", "ko_KR", "Male"},
	{"Norsk - Kvinne", "nb_NO_Female", "libttssmt-nb_NO_Female", "nb_NO", "Female"},
	{"Nederlands - Vrouw", "nl_NL_Female", "libttssmt-nl_NL_Female", "nl_NL", "Female"},
	{"Polski - Kobieta", "pl_PL_Female", "libttssmt-pl_PL_Female", "pl_PL", "Female"},
	{"\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x42\x72\x61\x73\x69\x6c\x29 - Feminino", "pt_BR_Female", "libttssmt-pt_BR_Female", "pt_BR", "Female"},
	{"\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x42\x72\x61\x73\x69\x6c\x29 - Masculino", "pt_BR_Male", "libttssmt-pt_BR_Male", "pt_BR", "Male"},
	{"\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x50\x6f\x72\x74\x75\x67\x61\x6c\x29 - Feminino", "pt_PT_Female", "libttssmt-pt_PT_Female", "pt_PT", "Female"},
	{"\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x50\x6f\x72\x74\x75\x67\x61\x6c\x29 - Masculino", "pt_PT_Male", "libttssmt-pt_PT_Male", "pt_PT", "Male"},
	{"\x50\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9 - \xD0\x96\xD0\xB5\xD0\xBD\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9", "ru_RU_Female", "libttssmt-ru_RU_Female", "ru_RU", "Female"},
	{"\x53\x6c\x6f\x76\x65\x6e\xC4\x8D\x69\x6e\x61 - \xC5\xBD\x65\x6e\x61", "sk_SK_Female", "libttssmt-sk_SK_Female", "sk_SK", "Female"},
	{"Svenska - Kvinna", "sv_SE_Female", "libttssmt-sv_SE_Female", "sv_SE", "Female"},
	{"\x54\xC3\xBC\x72\x6b\xC3\xA7\x65 - \x4b\x61\x64\xC4\xB1\x6e", "tr_TR_Female", "libttssmt-tr_TR_Female", "tr_TR", "Female"},
	{"\xE7\xAE\x80\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87 - \xE5\xA5\xB3", "zh_CN_Female", "libttssmt-zh_CN_Female", "zh_CN", "Female"},
	{"\xE7\xAE\x80\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87 - \xE7\x94\xB7", "zh_CN_Male", "libttssmt-zh_CN_Male", "zh_CN", "Male"},
	{"\xE7\xB9\x81\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87\x28\xE9\xA6\x99\xE6\xB8\xAF\x29 - \xE5\xA5\xB3", "zh_HK_Female", "libttssmt-zh_HK_Female", "zh_HK", "Female"},
	{"\xE7\xB9\x81\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87\x28\xE9\xA6\x99\xE6\xB8\xAF\x29 - \xE7\x94\xB7", "zh_HK_Male", "libttssmt-zh_HK_Male", "zh_HK", "Male"},
	{"\xE7\xB9\x81\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87\x28\xE5\x8F\xB0\xE6\xB9\xBE\x29 - \xE5\xA5\xB3", "zh_TW_Female", "libttssmt-zh_TW_Female", "zh_TW", "Female"}
};

#define TTS_TAG_ENGINE_BASE_TAG		"tts-engine"
#define TTS_TAG_ENGINE_VOICE_SET	"voices"
#define TTS_TAG_ENGINE_VOICE		"voice"
#define TTS_TAG_ENGINE_VOICE_TYPE	"type"

#define TTS_TAG_VOICE_TYPE_FEMALE	"female"
#define TTS_TAG_VOICE_TYPE_MALE		"male"
#define TTS_TAG_VOICE_TYPE_CHILD	"child"

#define TTS_ENGINE_INFO	tzplatform_mkpath(TZ_USER_HOME, "/share/.voice/tts/engine-info/ttssmt-info.xml")

#define TTS_ENGINE_DATA_PATH tzplatform_mkpath(TZ_USER_HOME, "/share/.voice/tts/engine-info/")

static Elm_Genlist_Item_Class *g_itc_group_title = NULL;
static Elm_Genlist_Item_Class *g_itc_button_1line = NULL;

static appdata_s *g_ad;

static int g_download_id;

typedef struct {
	char*	language;
	int	type;
} tts_config_voice_s; 

static int __tts_engine_info_add_voice(const char* path, tts_config_voice_s* voice)
{
	if (NULL == path || NULL == voice) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Input parameter is NULL");
		return -1;
	}

	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;

	doc = xmlParseFile(path);
	if (doc == NULL) {
		return -1;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Empty document");
		xmlFreeDoc(doc);
		return -1;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *)TTS_TAG_ENGINE_BASE_TAG)) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] The wrong type, root node is NOT 'tts-engine'");
		xmlFreeDoc(doc);
		return -1;
	}

	cur = cur->xmlChildrenNode;
	if (cur == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Empty document");
		xmlFreeDoc(doc);
		return -1;
	}

	while (cur != NULL) {
		if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTS_TAG_ENGINE_VOICE_SET)) {
			xmlNodePtr voice_node = NULL;
			voice_node = xmlNewNode(NULL, (const xmlChar *)TTS_TAG_ENGINE_VOICE);
			if (2 == voice->type) {
				xmlSetProp(voice_node, (const xmlChar *)TTS_TAG_ENGINE_VOICE_TYPE, (const xmlChar*)TTS_TAG_VOICE_TYPE_FEMALE);
			} else {
				xmlSetProp(voice_node, (const xmlChar *)TTS_TAG_ENGINE_VOICE_TYPE, (const xmlChar*)TTS_TAG_VOICE_TYPE_MALE);
			}

			xmlNodeSetContent(voice_node, (const xmlChar *)voice->language);
			xmlAddChild(cur, voice_node);
		}
		cur = cur->next;
	}

	int ret = xmlSaveFormatFile(path, doc, 1);
	if (0 > ret) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Save result : %d", ret);
	}

	xmlFreeDoc(doc);

	return 0;
}

static int __tts_engine_info_remove_voice(const char* path, tts_config_voice_s* voice)
{
	if (NULL == path || NULL == voice) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Input parameter is NULL");
		return -1;
	}

	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	xmlChar *key = NULL;
	xmlChar *attr = NULL;

	doc = xmlParseFile(path);
	if (doc == NULL) {
		return -1;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Empty document");
		xmlFreeDoc(doc);
		return -1;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *)TTS_TAG_ENGINE_BASE_TAG)) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] The wrong type, root node is NOT 'tts-engine'");
		xmlFreeDoc(doc);
		return -1;
	}

	cur = cur->xmlChildrenNode;
	if (cur == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Empty document");
		xmlFreeDoc(doc);
		return -1;
	}

	while (cur != NULL) {
		if (0 == xmlStrcmp(cur->name, (const xmlChar *)TTS_TAG_ENGINE_VOICE_SET)) {
			xmlNodePtr voice_node = NULL;
			voice_node = cur->xmlChildrenNode;

			while (NULL != voice_node) {
				if (0 == xmlStrcmp(voice_node->name, (const xmlChar *)TTS_TAG_ENGINE_VOICE)) {
					key = xmlNodeGetContent(voice_node);
					if (NULL != key) {
						if (!strcmp((char*)key, voice->language)) {
							int temp_type = -1;
							attr = xmlGetProp(voice_node, (const xmlChar*)TTS_TAG_ENGINE_VOICE_TYPE);
							if (NULL != attr) {
								if (0 == xmlStrcmp(attr, (const xmlChar *)TTS_TAG_VOICE_TYPE_FEMALE)) {
									temp_type = (int)2;
								} else if (0 == xmlStrcmp(attr, (const xmlChar *)TTS_TAG_VOICE_TYPE_MALE)) {
									temp_type = (int)1;
								}
								xmlFree(attr);
							}
							if (temp_type == voice->type) {
								/* remove */
								xmlUnlinkNode(voice_node);
								xmlFreeNode(voice_node);
							}
						}
						xmlFree(key);
					} else {
						dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] <%s> has no content", TTS_TAG_ENGINE_VOICE_TYPE);
					}
				}
				voice_node = voice_node->next;
			}
		}
		cur = cur->next;
	}

	int ret = xmlSaveFormatFile(path, doc, 1);
	if (0 > ret) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Save result : %d", ret);
	}

	xmlFreeDoc(doc);

	return 0;
}

static char * __genlist_text_get(void *data, Evas_Object *obj, const char *part)
{
	item_index_e idx = (item_index_e)data;

	if (!strcmp("elm.text", part)) {
		return strdup(item_data[idx][0]);
	}

	return NULL;
}

static bool __installed_lang(int idx)
{
	char filepath[1024] = {'\0',};
	const char* data_path = TTS_ENGINE_DATA_PATH;
	snprintf(filepath, 1024, "%s/%s", data_path, item_data[idx][1]);

	if (EN_US_FEMALE == idx || KO_KR_FEMALE == idx || DE_DE_FEMALE == idx || FR_FR_FEMALE == idx || IT_IT_FEMALE == idx || ES_ES_FEMALE == idx) {
		return true;
	} else {
		if (0 != access(filepath, F_OK)) {
			return false;
		} else {
			return true;
		}
	}
}

static void __check_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_genlist_item_update(g_ad->selected_lang);
}

static Evas_Object * __genlist_content_get(void *data, Evas_Object *obj, const char *part)
{
	item_index_e idx = (item_index_e)data;

	if (!strcmp("elm.swallow.icon.1", part)) {
		Evas_Object *check = elm_check_add(obj);
		elm_object_style_set(check, "default");
		elm_check_state_set(check, __installed_lang(idx));
		if (EN_US_FEMALE == idx || KO_KR_FEMALE == idx || DE_DE_FEMALE == idx || FR_FR_FEMALE == idx || IT_IT_FEMALE == idx || ES_ES_FEMALE == idx) {
			elm_object_disabled_set(check, EINA_TRUE);
		}

		evas_object_propagate_events_set(check, EINA_TRUE);
		evas_object_smart_callback_add(check, "changed", __check_changed_cb, data);
		evas_object_show(check);
		return check;
	}

	return NULL;
}

static void __hide_progress_popup()
{
	if (NULL != g_ad->progress_popup) {
		evas_object_del(g_ad->progress_popup);
		g_ad->progress_popup = NULL;
	}
}

static void __progress_popup_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	if (NULL != g_ad->progress_popup) {
		evas_object_del(g_ad->progress_popup);
		g_ad->progress_popup = NULL;
	}
	return;
}

static void __progress_popup_back_cb(void *data, Evas_Object *obj, void *event_info)
{

	return;
}

static void __show_progress_popup(bool failed, const char* msg)
{
	if (NULL != g_ad->progress_popup) {
		evas_object_del(g_ad->progress_popup);
		g_ad->progress_popup = NULL;
	}

	Evas_Object *popup = elm_popup_add(g_ad->win);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_text_set(popup, "title,text", "Downloading");

	if (failed) {
		if (NULL != msg) {
			elm_object_part_text_set(popup, "default", msg);
		} else {
			elm_object_part_text_set(popup, "default", "Fail to download language pack.");
		}
		elm_popup_timeout_set(popup, 2.0);

	} else {
		Evas_Object *progressbar = elm_progressbar_add(popup);
		elm_object_style_set(progressbar, "process_large");
		evas_object_size_hint_align_set(progressbar, EVAS_HINT_FILL, 0.5);
		evas_object_size_hint_weight_set(progressbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_progressbar_pulse(progressbar, EINA_TRUE);

		elm_object_content_set(popup, progressbar);
		evas_object_show(progressbar);
	}

	evas_object_smart_callback_add(popup, "block,clicked", NULL, NULL);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, __progress_popup_del_cb, NULL);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, __progress_popup_back_cb, NULL);

	g_ad->progress_popup = popup;

	evas_object_show(popup);
}

static void __delete_popup_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	if (NULL != g_ad->delete_popup) {
		evas_object_del(g_ad->delete_popup);
		g_ad->delete_popup = NULL;
	}
	return;
}

static void __delete_popup_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	return;
}

static void __show_delete_popup()
{
	if (NULL != g_ad->delete_popup) {
		evas_object_del(g_ad->delete_popup);
		g_ad->delete_popup = NULL;
	}

	Evas_Object *popup = elm_popup_add(g_ad->win);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_text_set(popup, "title,text", "Deleting");
	elm_popup_timeout_set(popup, 1.0);

	Evas_Object *progressbar = elm_progressbar_add(popup);
	elm_object_style_set(progressbar, "process_large");
	evas_object_size_hint_align_set(progressbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(progressbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse(progressbar, EINA_TRUE);

	elm_object_content_set(popup, progressbar);
	evas_object_show(progressbar);

	evas_object_smart_callback_add(popup, "block,clicked", NULL, NULL);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, __delete_popup_del_cb, NULL);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, __delete_popup_back_cb, NULL);

	g_ad->delete_popup = popup;

	evas_object_show(popup);
}

static void __remove_temp_file(void* data)
{
	intptr_t pidx = (intptr_t)user_data;
	int idx = (int)pidx;

	char cmd[1024] = {'\0',};
	//char* data_path = app_get_data_path();
	const char* data_path = TTS_ENGINE_DATA_PATH;
	snprintf(cmd, 1024, "tar zxvf %s/%s.tar.gz -C %s", data_path, item_data[idx][2], data_path);
	int ret = system(cmd);
	dlog_print(DLOG_DEBUG, LOG_TAG, "system return(%d)", ret);

	char filepath[1024] = {'\0',};
	snprintf(filepath, 1024, "%s/%s.tar.gz", data_path, item_data[idx][2]);
	remove(filepath);
}

static void* __download_completed_cb(void *user_data)
{
	__remove_temp_file(user_data);

	/* TODO */
	tts_config_voice_s *voice = (tts_config_voice_s *)calloc(1, sizeof(tts_config_voice_s));
	voice->language = strdup(item_data[idx][3]);
	if (!strcmp(item_data[idx][4], "Male")) {
		voice->type = 1;
	} else {
		voice->type = 2;
	}
	__tts_engine_info_add_voice(TTS_ENGINE_INFO, voice);

	free(voice->language);
	free(voice);

	__hide_progress_popup();
	elm_genlist_item_update(g_ad->selected_lang);

	return NULL;
}

static void* __download_failed_cb(void *user_data)
{
	__remove_temp_file(user_data);

	__hide_progress_popup();
	__show_progress_popup(true, NULL);
	elm_genlist_item_update(g_ad->selected_lang);
	return NULL;
}

static void *__download_canceled_cb(void *user_data)
{
	__remove_temp_file(user_data);
}

static void __download_state_changed_cb(int download_id, download_state_e state, void *user_data)
{
	if (DOWNLOAD_STATE_COMPLETED == state) {
		dlog_print(DLOG_INFO, LOG_TAG, "===== Download Completed");
		ecore_main_loop_thread_safe_call_sync(__download_completed_cb, user_data);
	} else if (DOWNLOAD_STATE_FAILED == state) {
		dlog_print(DLOG_INFO, LOG_TAG, "===== Download Failed");
		ecore_main_loop_thread_safe_call_sync(__download_failed_cb, user_data);
	} else if (DOWNLOAD_STATE_CANCELED == state) {
		dlog_print(DLOG_INFO, LOG_TAG, "===== Download Canceled");
		ecore_main_loop_thread_safe_call_sync(__download_canceled_cb, user_data);
	}

	download_destroy(download_id);
	wifi_deinitialize();
	dlog_print(DLOG_INFO, LOG_TAG, "=====");
}

static void __wifi_conn_changed_cb(wifi_connection_state_e state, wifi_ap_h ap, void* user_data)
{
	if (WIFI_CONNECTION_STATE_CONNECTED != state) {
		dlog_print(DLOG_INFO, LOG_TAG, "Wifi disconnected");
		int ret;
		ret = download_cancel(g_download_id);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to download cancel");
		}
		__show_progress_popup(true, "Network ERROR - Check wifi connection");
	}
}

static void __lang_item_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);
	g_ad->selected_lang = item;

	intptr_t pidx = (intptr_t)data;
	int idx = (int)pidx;
	dlog_print(DLOG_INFO, LOG_TAG, "[%d] item clicked", idx);

	/* download */
	if (false == __installed_lang(idx)) {
		dlog_print(DLOG_INFO, LOG_TAG, ">> Download <<");
		int ret;

		ret = wifi_initialize();
		if (WIFI_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to wifi init");
			__show_progress_popup(true, "Network ERROR - Check wifi connection");
			return;
		}

		wifi_connection_state_e state;
		ret = wifi_get_connection_state(&state);
		if (WIFI_ERROR_NONE != ret) {
			__show_progress_popup(true, "Network ERROR - Check wifi connection");
			wifi_deinitialize();
			return;
		}

		if (WIFI_CONNECTION_STATE_CONNECTED != state) {
			dlog_print(DLOG_ERROR, LOG_TAG, "Wifi Disconnected");
			__show_progress_popup(true, "Network ERROR - Check wifi connection");
			wifi_deinitialize();
			return;
		}

		ret = wifi_set_connection_state_changed_cb(__wifi_conn_changed_cb, (void *)pidx);
		if (WIFI_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to set wifi state changed cb");
			__show_progress_popup(true, "Network ERROR - Check wifi connection");
			wifi_deinitialize();
			return;
		}

		int download_id;
		ret = download_create(&download_id);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to download create(%d)", ret);
			__show_progress_popup(true, NULL);
			wifi_deinitialize();
			return;
		}

		ret = download_set_network_type(download_id, DOWNLOAD_NETWORK_WIFI);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to set network type(%d)", ret);
			__show_progress_popup(true, "Network ERROR - Check wifi connection");
			download_destroy(download_id);
			wifi_deinitialize();
			return;
		}

		ret = download_set_state_changed_cb(download_id, __download_state_changed_cb, (void *)pidx);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to set state cb(%d)", ret);
			__show_progress_popup(true, NULL);
			download_destroy(download_id);
			wifi_deinitialize();
			return;
		}

		char url[1024] = {'\0',};
		snprintf(url, 1024, "http://download.tizen.org/releases/resources/voice/tts/smt-language-pack/%s.tar.gz", item_data[idx][2]);
		ret = download_set_url(download_id, url);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to set url(%d)", ret);
			__show_progress_popup(true, NULL);
			download_destroy(download_id);
			wifi_deinitialize();
			return;
		}

		//char *data_path = app_get_data_path();
		const char *data_path = TTS_ENGINE_DATA_PATH;
		ret = download_set_destination(download_id, data_path);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to set destination(%d)", ret);
			__show_progress_popup(true, NULL);
			download_destroy(download_id);
			wifi_deinitialize();
			return;
		}
		//free(data_path);

		ret = download_start(download_id);
		if (DOWNLOAD_ERROR_NONE != ret) {
			dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] Fail to start(%d)", ret);
			__show_progress_popup(true, NULL);
			download_destroy(download_id);
			wifi_deinitialize();
			return;
		}

		g_download_id = download_id;
		__show_progress_popup(false, NULL);
		
		dlog_print(DLOG_INFO, LOG_TAG, "<< End >>");
	} else {
		dlog_print(DLOG_INFO, LOG_TAG, ">> Delete <<");
		char cmd[1024] = {'\0',};
		//char* data_path = app_get_data_path();
		const char* data_path = TTS_ENGINE_DATA_PATH;
		snprintf(cmd, 1024, "rm -rf %s/*%s*", data_path, item_data[idx][1]);
		int ret = system(cmd);
		dlog_print(DLOG_DEBUG, LOG_TAG, "system return (%d)", ret);
		elm_genlist_item_update(g_ad->selected_lang);

		__show_delete_popup();

		tts_config_voice_s *voice = (tts_config_voice_s *)calloc(1, sizeof(tts_config_voice_s));
		voice->language = strdup(item_data[idx][3]);
		if (!strcmp(item_data[idx][4], "Male")) {
			voice->type = 1;
		} else {
			voice->type = 2;
		}
		__tts_engine_info_remove_voice(TTS_ENGINE_INFO, voice);

		free(voice->language);
		free(voice);

		dlog_print(DLOG_INFO, LOG_TAG, "<< End >>");
	}
}

static void create_contents(appdata_s *ad)
{
	/* Genlist */
	ad->genlist = elm_genlist_add(ad->naviframe);
	elm_genlist_mode_set(ad->genlist, ELM_LIST_COMPRESS);
	elm_genlist_homogeneous_set(ad->genlist, EINA_TRUE);

	g_itc_group_title = elm_genlist_item_class_new();
	if (NULL == g_itc_group_title) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Fail to item class new");
		return;
	}
	g_itc_group_title->item_style = "group_index";
	g_itc_group_title->func.text_get = __genlist_text_get;
	g_itc_group_title->func.content_get = NULL;
	g_itc_group_title->func.state_get = NULL;
	g_itc_group_title->func.del = NULL;

	g_itc_button_1line = elm_genlist_item_class_new();
	if (NULL == g_itc_button_1line) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Fail to item class new");
		return;
	}
	g_itc_button_1line->item_style = "type1";
	g_itc_button_1line->func.text_get = __genlist_text_get;
	g_itc_button_1line->func.content_get = __genlist_content_get;
	g_itc_button_1line->func.state_get = NULL;
	g_itc_button_1line->func.del = NULL;

	/* Download - Title */
	Elm_Object_Item *it;
	intptr_t pi = (intptr_t)DOWNLOAD_TITLE;
	it = elm_genlist_item_append(ad->genlist, g_itc_group_title, (void *)pi, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(it, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	int i;
	for (i = DOWNLOAD_TITLE + 1; i < END_INDEX; i++) {
		pi = (intptr_t)i;
		it = elm_genlist_item_append(ad->genlist, g_itc_button_1line, (void *)pi, NULL, ELM_GENLIST_ITEM_NONE, __lang_item_clicked_cb, (void *)pi);
		if (EN_US_FEMALE == i || KO_KR_FEMALE == i || DE_DE_FEMALE == i || FR_FR_FEMALE == i || IT_IT_FEMALE == i || ES_ES_FEMALE == i) {
			elm_genlist_item_select_mode_set(it, ELM_OBJECT_SELECT_MODE_NONE);
		}
	}

	evas_object_show(ad->genlist);
}

static Eina_Bool __naviframe_item_pop_cb(void *data, Elm_Object_Item *it)
{
	dlog_print(DLOG_DEBUG, LOG_TAG, "");
	ui_app_exit();
	return EINA_TRUE;
}

static void create_base_gui(appdata_s *ad)
{
	g_ad = ad;
	/* Window */
	ad->win = elm_win_util_standard_add("org.tizen.ttssmt-setting", "org.tizen.ttssmt-setting");
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Naviframe */
	ad->naviframe = elm_naviframe_add(ad->conform);
	eext_object_event_callback_add(ad->naviframe, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	evas_object_size_hint_weight_set(ad->naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->naviframe, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(ad->conform, "elm.swallow.content", ad->naviframe);
	evas_object_show(ad->naviframe);

	/* Contents */
	create_contents(ad);
	Elm_Object_Item *main_item = elm_naviframe_item_push(ad->naviframe, "Engine setting", NULL, NULL, ad->genlist, NULL);
	elm_naviframe_item_title_enabled_set(main_item, EINA_TRUE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(main_item, __naviframe_item_pop_cb, ad);
	elm_object_content_set(ad->conform, ad->naviframe);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}


static bool app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	Initialize UI resources and application's data
	If this function returns true, the main loop of application starts
	If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void app_terminate(void *data)
{
	/* Release all resources. */
}

static void ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}


/*
vi:ts=4:ai:nowrap:expandtab
*/
