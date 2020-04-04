#ifndef SM83_H
#define SM83_H

#include <SM83/Bus.h>
#include <SM83/CPU.h>
#include <SM83/Context.h>
#include <SM83/Debug.h>
#include <SM83/Disassembler.h>
#include <SM83/Interrupts.h>
#include <SM83/Types.h>

void SM83_Tick(sm83_t * cpu, unsigned mcycles);

void SM83_Init(sm83_t * cpu);

void SM83_SetMode(sm83_t * cpu, sm83_mode_t mode);

#endif // SM83_H