#pragma once

class StringMultiMapHandler : public IHandleTypeDispatch {
	public:
	void OnHandleDestroy(HandleType_t type, void* object);
};

extern HandleType_t g_StringMultiMapType;
extern HandleType_t g_StringMultiMapIteratorType;

cell_t sm_CreateStringMultiMap(IPluginContext *pContext, const cell_t *params);

cell_t sm_RemoveStringMultiMapEntry(IPluginContext *pContext, const cell_t *params);
cell_t sm_ClearStringMultiMap(IPluginContext *pContext, const cell_t *params);

cell_t sm_GetStringMultiMapSize(IPluginContext *pContext, const cell_t *params);

cell_t sm_GetStringMultiMapValue(IPluginContext *pContext, const cell_t *params);
cell_t sm_AddStringMultiMapValue(IPluginContext *pContext, const cell_t *params);

cell_t sm_GetStringMultiMapString(IPluginContext *pContext, const cell_t *params);
cell_t sm_AddStringMultiMapString(IPluginContext *pContext, const cell_t *params);

cell_t sm_GetStringMultiMapArray(IPluginContext *pContext, const cell_t *params);
cell_t sm_AddStringMultiMapArray(IPluginContext *pContext, const cell_t *params);

cell_t sm_BuildStringMultiMapIterator(IPluginContext *pContext, const cell_t *params);
cell_t sm_BuildStringMultiMapKeyIterator(IPluginContext *pContext, const cell_t *params);

cell_t sm_StringMultiMapIteratorNext(IPluginContext *pContext, const cell_t *params);
cell_t sm_GetStringMultiMapIteratorKey(IPluginContext *pContext, const cell_t *params);

cell_t sm_GetStringMultiMapIteratorString(IPluginContext *pContext, const cell_t *params);
cell_t sm_SetStringMultiMapIteratorString(IPluginContext *pContext, const cell_t *params);
cell_t sm_GetStringMultiMapIteratorValue(IPluginContext *pContext, const cell_t *params);
cell_t sm_SetStringMultiMapIteratorValue(IPluginContext *pContext, const cell_t *params);
cell_t sm_GetStringMultiMapIteratorArray(IPluginContext *pContext, const cell_t *params);
cell_t sm_SetStringMultiMapIteratorArray(IPluginContext *pContext, const cell_t *params);
cell_t sm_RemoveOnStringMultiMapIterator(IPluginContext *pContext, const cell_t *params);


const sp_nativeinfo_t g_StringMultiMapNatives[] = {
	{ "StringMultiMap.StringMultiMap", sm_CreateStringMultiMap },
	
	{ "StringMultiMap.Remove", sm_RemoveStringMultiMapEntry },
	{ "StringMultiMap.Clear", sm_ClearStringMultiMap },
	
	{ "StringMultiMap.Size.get", sm_GetStringMultiMapSize },
	
	{ "StringMultiMap.GetValue", sm_GetStringMultiMapValue },
	{ "StringMultiMap.AddValue", sm_AddStringMultiMapValue },
	
	{ "StringMultiMap.GetString", sm_GetStringMultiMapString },
	{ "StringMultiMap.AddString", sm_AddStringMultiMapString },
	
	{ "StringMultiMap.GetArray", sm_GetStringMultiMapArray },
	{ "StringMultiMap.AddArray", sm_AddStringMultiMapArray },
	
	{ "StringMultiMap.GetIterator", sm_BuildStringMultiMapIterator },
	{ "StringMultiMap.GetKeyIterator", sm_BuildStringMultiMapKeyIterator },
	
	{ "StringMultiMapIterator.Next", sm_StringMultiMapIteratorNext },
	{ "StringMultiMapIterator.GetKey", sm_GetStringMultiMapIteratorKey },
	{ "StringMultiMapIterator.GetString", sm_GetStringMultiMapIteratorString },
	{ "StringMultiMapIterator.SetString", sm_SetStringMultiMapIteratorString },
	{ "StringMultiMapIterator.GetValue", sm_GetStringMultiMapIteratorValue },
	{ "StringMultiMapIterator.SetValue", sm_SetStringMultiMapIteratorValue },
	{ "StringMultiMapIterator.GetArray", sm_GetStringMultiMapIteratorArray },
	{ "StringMultiMapIterator.SetArray", sm_SetStringMultiMapIteratorArray },
	
	{ "StringMultiMapIterator.Remove", sm_RemoveOnStringMultiMapIterator },
	
	{NULL, NULL},
};
