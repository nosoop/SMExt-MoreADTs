/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"

#include "types/string_multimap.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

MoreADTExt g_Extension;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Extension);

StringMultiMapHandler g_StringMultiMapHandler;

bool MoreADTExt::SDK_OnLoad(char* error, size_t maxlength, bool late) {
	sharesys->AddNatives(myself, g_StringMultiMapNatives);
	g_StringMultiMapType = g_pHandleSys->CreateType("StringMultiMap",
			&g_StringMultiMapHandler, 0, NULL, NULL, myself->GetIdentity(), NULL);
	g_StringMultiMapIteratorType = g_pHandleSys->CreateType("StringMultiMapIterator",
			&g_StringMultiMapHandler, 0, NULL, NULL, myself->GetIdentity(), NULL);
	
	return true;
}

void MoreADTExt::SDK_OnUnload() {
	g_pHandleSys->RemoveType(g_StringMultiMapType, myself->GetIdentity());
	g_pHandleSys->RemoveType(g_StringMultiMapIteratorType, myself->GetIdentity());
}
