#pragma once

#ifdef _DEBUG
#pragma comment(lib, "cpu-core-debug.lib")
#pragma comment(lib, "cpu-render-debug.lib")
#pragma comment(lib, "cpu-engine-debug.lib")
#else
#pragma comment(lib, "cpu-core.lib")
#pragma comment(lib, "cpu-render.lib")
#pragma comment(lib, "cpu-engine.lib")
#endif

#include <cpu-engine/cpu-engine.h>

#include "App.h"