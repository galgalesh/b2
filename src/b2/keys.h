#ifndef HEADER_F189E7E3EFB04F5490038430D6E02054// -*- mode:c++ -*-
#define HEADER_F189E7E3EFB04F5490038430D6E02054

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <beeb/keys.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <shared/enum_decl.h>
#include "keys.inl"
#include <shared/enum_end.h>

static const uint32_t PCKeyModifier_All=PCKeyModifier_Shift|PCKeyModifier_Ctrl|PCKeyModifier_Alt|PCKeyModifier_Gui|PCKeyModifier_AltGr|PCKeyModifier_NumLock;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Handles BeebKey and BeebSpecialKey. Returns nullptr if unknown.
const char *GetBeebKeyName(uint8_t beeb_key);
uint8_t GetBeebKeyByName(const char *name);

const char *GetBeebKeySymName(uint8_t beeb_keycap);
uint8_t GetBeebKeySymByName(const char *name);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool GetBeebKeyComboForKeySym(uint8_t *beeb_key,BeebShiftState *shift_state,uint8_t beeb_sym);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uint32_t GetPCKeyModifiersFromSDLKeymod(uint16_t mod);

// Get name of combined SDL_Keycode/PCKeyModifier value.
//
// If the SDL_Keycode part is 0, returns an empty string, ignoring any
// modifier flags.
std::string GetKeycodeName(uint32_t keycode);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif