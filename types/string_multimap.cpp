#include "extension.h"
#include "string_multimap.h"

#include <map>
#include <vector>
#include <cstring>

#include <mpark/variant.hpp>

#include "iterator_container.h"

HandleType_t g_StringMultiMapType = 0;
HandleType_t g_StringMultiMapIteratorType = 0;
 
using StringMultiMapValue = mpark::variant<cell_t, std::vector<cell_t>, std::string>;
using StringMultiMap = std::multimap<std::string, StringMultiMapValue>;

using StringMultiMapIterator = IteratorContainer<StringMultiMap>;

void StringMultiMapHandler::OnHandleDestroy(HandleType_t type, void* object) {
	if (type == g_StringMultiMapType) {
		delete (StringMultiMap*) object;
	}
	if (type == g_StringMultiMapIteratorType) {
		delete (StringMultiMapIterator*) object;
	}
}

HandleError ReadStringMultiMapHandle(Handle_t hndl, StringMultiMap **multiMap) {
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();
	
	return g_pHandleSys->ReadHandle(hndl, g_StringMultiMapType, &sec, (void **) multiMap);
}

HandleError ReadStringMultiMapIterHandle(Handle_t hndl, StringMultiMapIterator **multiMapIter) {
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();
	
	return g_pHandleSys->ReadHandle(hndl, g_StringMultiMapIteratorType, &sec, (void **) multiMapIter);
}

/* native bool StringMultiMap.StringMultiMap(); */
cell_t sm_CreateStringMultiMap(IPluginContext *pContext, const cell_t *params) {
	StringMultiMap *pMultiMap = new StringMultiMap;
	
	return g_pHandleSys->CreateHandle(g_StringMultiMapType, pMultiMap,
			pContext->GetIdentity(), myself->GetIdentity(), NULL);
}

cell_t sm_RemoveStringMultiMapEntry(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	pMultiMap->erase(key);
	return false;
}

/* native bool StringMultiMap.GetValue(const char[] key, any &value); */
cell_t sm_GetStringMultiMapValue(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	auto result = pMultiMap->find(std::string(key));
	if (result != pMultiMap->end()) {
		if (auto pval = mpark::get_if<cell_t>(&result->second)) {
			cell_t* val;
			pContext->LocalToPhysAddr(params[3], &val);
			
			*val = *pval;
			return true;
		}
	}
	return false;
}

/* native void StringMultiMap.AddValue(const char[] key, any value); */
cell_t sm_AddStringMultiMapValue(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	cell_t val = params[3];
	
	pMultiMap->insert(StringMultiMap::value_type(std::string(key), val));
	return 0;
}

/* native bool StringMultiMap.GetString(const char[] key, char[] value, int maxlen, int &written); */
cell_t sm_GetStringMultiMapString(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	auto result = pMultiMap->find(std::string(key));
	if (result != pMultiMap->end()) {
		if (auto pval = mpark::get_if<std::string>(&result->second)) {
			cell_t *pWritten;
			pContext->LocalToPhysAddr(params[5], &pWritten);
			
			*pWritten = pContext->StringToLocal(params[3], params[4], pval->c_str());
			
			return true;
		}
	}
	return false;
}

/* native void StringMultiMap.AddString(const char[] key, const char[] value); */
cell_t sm_AddStringMultiMapString(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	char* val;
	pContext->LocalToString(params[3], &val);
	
	pMultiMap->insert(StringMultiMap::value_type(std::string(key), val));
	return 0;
}

/* native bool StringMultiMap.GetArray(const char[] key, any[] value, int max_size, int &size); */
cell_t sm_GetStringMultiMapArray(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	auto result = pMultiMap->find(std::string(key));
	if (result != pMultiMap->end()) {
		if (auto pval = mpark::get_if<std::vector<cell_t>>(&result->second)) {
			cell_t* array;
			pContext->LocalToPhysAddr(params[3], &array);
			
			cell_t *pSize;
			pContext->LocalToPhysAddr(params[5], &pSize);
			
			if (pval->size() > size_t(params[4])) {
				*pSize = params[4];
			} else {
				*pSize = pval->size();
			}
			
			memcpy(array, pval->data(),  sizeof(cell_t) * pSize[0]);
			
			return true;
		}
	}
	return false;
}

/* native void StringMultiMap.AddArray(const char[] key, const any[] array, int num_items); */
cell_t sm_AddStringMultiMapArray(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	cell_t* array;
	pContext->LocalToPhysAddr(params[3], &array);
	
	size_t array_len = params[4];
	
	std::vector<cell_t> val;
	for (cell_t i = 0; i < array_len; i++) {
		val.push_back(array[i]);
	}
	
	pMultiMap->insert(StringMultiMap::value_type(std::string(key), val));
	return 0;
}

/* native StringMultiMapIterator StringMultiMap.GetIterator(); */
cell_t sm_BuildStringMultiMapIterator(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	StringMultiMapIterator *pIter = new StringMultiMapIterator(pMultiMap, pMultiMap->begin(),
			pMultiMap->end());
	
	return g_pHandleSys->CreateHandle(g_StringMultiMapIteratorType, pIter,
			pContext->GetIdentity(), myself->GetIdentity(), NULL);
}

/* native StringMultiMapIterator StringMultiMap.GetKeyIterator(const char[] key); */
cell_t sm_BuildStringMultiMapKeyIterator(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMap *pMultiMap;
	HandleError err;
	if ((err = ReadStringMultiMapHandle(hndl, &pMultiMap)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMap handle %x (error %d)", hndl, err);
	}
	
	char *key;
	pContext->LocalToString(params[2], &key);
	
	auto range = pMultiMap->equal_range(key);
	StringMultiMapIterator *pIter = new StringMultiMapIterator(pMultiMap, range.first, range.second);
	
	return g_pHandleSys->CreateHandle(g_StringMultiMapIteratorType, pIter,
			pContext->GetIdentity(), myself->GetIdentity(), NULL);
}

/* native bool StringMultiMapIterator.Next(); */
cell_t sm_StringMultiMapIteratorNext(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	return pMultiMapIter->Next();
}

/* native void StringMultiMapIterator.GetKey(char[] buffer, int maxlen); */
cell_t sm_GetStringMultiMapIteratorKey(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	pContext->StringToLocal(params[2], params[3], pMultiMapIter->Current()->first.c_str());
	
	return 0;
}

/* native bool StringMultiMapIterator.GetString(char[] buffer, int maxlen); */
cell_t sm_GetStringMultiMapIteratorString(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	if (auto pval = mpark::get_if<std::string>(&pMultiMapIter->Current()->second)) {
		pContext->StringToLocal(params[2], params[3], pval->c_str());
		return true;
	}
	
	return false;
}

/* native void StringMultiMapIterator.SetString(const char[] value); */
cell_t sm_SetStringMultiMapIteratorString(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	char* val;
	pContext->LocalToString(params[2], &val);
	
	pMultiMapIter->Current()->second = val;
	
	return 0;
}

/* native bool StringMultiMapIterator.GetValue(any &value); */
cell_t sm_GetStringMultiMapIteratorValue(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	if (auto pval = mpark::get_if<cell_t>(&pMultiMapIter->Current()->second)) {
		cell_t* val;
		pContext->LocalToPhysAddr(params[2], &val);
		
		*val = *pval;
		return true;
	}
	
	return false;
}

/* native void StringMultiMapIterator.SetValue(any value); */
cell_t sm_SetStringMultiMapIteratorValue(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	cell_t val = params[2];
	
	pMultiMapIter->Current()->second = val;
	
	return 0;
}

/* native bool StringMultiMapIterator.GetArray(any[] array, int max_size, int& size = 0); */
cell_t sm_GetStringMultiMapIteratorArray(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	if (auto pval = mpark::get_if<std::vector<cell_t>>(&pMultiMapIter->Current()->second)) {
		cell_t* array;
		pContext->LocalToPhysAddr(params[2], &array);
		
		cell_t *pSize;
		pContext->LocalToPhysAddr(params[4], &pSize);
		
		if (pval->size() > size_t(params[3])) {
			*pSize = params[3];
		} else {
			*pSize = pval->size();
		}
		
		memcpy(array, pval->data(),  sizeof(cell_t) * pSize[0]);
		
		return true;
	}
	
	return false;
}

/* native void StringMultiMapIterator.SetArray(const any[] array, int num_items); */
cell_t sm_SetStringMultiMapIteratorArray(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	cell_t* array;
	pContext->LocalToPhysAddr(params[2], &array);
	
	size_t array_len = params[3];
	
	std::vector<cell_t> val;
	for (cell_t i = 0; i < array_len; i++) {
		val.push_back(array[i]);
	}
	
	pMultiMapIter->Current()->second = val;
	
	return 0;
}

/* native void StringMultiMapIterator.Erase(); */
cell_t sm_RemoveOnStringMultiMapIterator(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	StringMultiMapIterator *pMultiMapIter;
	HandleError err;
	if ((err = ReadStringMultiMapIterHandle(hndl, &pMultiMapIter)) != HandleError_None) {
		return pContext->ThrowNativeError("Invalid StringMultiMapIterator handle %x (error %d)", hndl, err);
	}
	
	pMultiMapIter->MarkRemoved();
	
	return 0;
}
