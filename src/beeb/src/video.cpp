#include <shared/system.h>
#include <beeb/video.h>

#include <shared/enum_def.h>
#include <beeb/video.inl>
#include <shared/enum_end.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static_assert((BeebControlPixel_Nothing^1)==BeebControlPixel_Cursor,"");