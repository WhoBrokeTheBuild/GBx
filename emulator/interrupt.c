#include "interrupt.h"
#include "log.h"
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "video.h"

bool IME = true;
IE_t IE;
IE_t IF;
