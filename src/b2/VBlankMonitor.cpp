#include <shared/system.h>
#include "VBlankMonitor.h"
#include "VBlankMonitorWindows.h"
#include "VBlankMonitorOSX.h"
#include "VBlankMonitorDefault.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VBlankMonitor::Handler::Handler() {
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VBlankMonitor::Handler::~Handler() {
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VBlankMonitor::VBlankMonitor() {
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VBlankMonitor::~VBlankMonitor() {
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::unique_ptr<VBlankMonitor> CreateVBlankMonitor(VBlankMonitor::Handler *handler,Messages *messages) {
    std::unique_ptr<VBlankMonitor> v;

#if SYSTEM_WINDOWS
    v=CreateVBlankMonitorWindows();
#elif SYSTEM_OSX
    v=CreateVBlankMonitorOSX();
#else
    v=CreateVBlankMonitorDefault(std::chrono::microseconds(20000));
#endif

    if(!v) {
        return nullptr;
    }

    if(!v->Init(handler,messages)) {
        return nullptr;
    }

    return v;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////