#ifndef HEADER_9DAE5793C00D47FC8B45FBE4B517D13E// -*- mode:c++ -*-
#define HEADER_9DAE5793C00D47FC8B45FBE4B517D13E
#if SYSTEM_OSX

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <memory>

class VBlankMonitor;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::unique_ptr<VBlankMonitor> CreateVBlankMonitorOSX();

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
#endif