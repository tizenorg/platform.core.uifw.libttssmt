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

#include "stdio.h"
#include "memory.h"

#include <ttsp.h>
#include "plugin_internal.h"

char * _psz             = NULL;
char * _pszEngineUUID;
char * _pszEngineName;
char * _pszEngineUgName;



int ttsp_load_engine(ttspd_funcs_s* pdfuncs, ttspe_funcs_s* pefuncs)
{
  if (! pdfuncs)
  {
    SLOG(LOG_ERROR, LOG_TAG, "parameter1, pdfuncs is NULL.");
    return TTSP_ERROR_INVALID_PARAMETER;
  }
  if (! pefuncs)
  {
    SLOG(LOG_ERROR, LOG_TAG, "parameter2, pefuncs is NULL.");
    return TTSP_ERROR_INVALID_PARAMETER;
  }

  pefuncs->size              = sizeof(ttspe_funcs_s);
  pefuncs->version           = 1;

  pefuncs->initialize        = plugin_Initialize;
  pefuncs->deinitialize      = plugin_Finalize;

  pefuncs->start_synth       = plugin_SynthesizeText;
  pefuncs->cancel_synth      = plugin_StopSynthesis;

  pefuncs->foreach_voices    = plugin_ForeachVoices;
  pefuncs->is_valid_voice    = plugin_IsValidVoice;

  pefuncs->load_voice	     = plugin_LoadVoice;
  pefuncs->unload_voice	     = plugin_UnloadVoice;

  plugin_SetDaemonAPIs(pdfuncs);

  SLOG(LOG_DEBUG, LOG_TAG, "[SMT] ttsp_load_engine() : version(%d), (6.20110527), size(%d)", pefuncs->version, pefuncs->size);

  return TTSP_ERROR_NONE;
}

void ttsp_unload_engine(void)
{
  SLOG(LOG_DEBUG, LOG_TAG, "[SMT] ttsp_unload_engine()");
}

int ttsp_get_engine_info(ttsp_engine_info_cb callback, void* user_data)
{

	callback(ENGINE_UUID, ENGINE_NAME, ENGINE_UG_NAME, false, user_data);

	return 0;
}
