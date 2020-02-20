#include "register.h"

#include "log.h"

reg_t R;

void printR()
{
    LogInfo("AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X PC=%04X FZ=%d FN=%d FH=%d FC=%d",
        R.AF, R.BC, R.DE, R.HL, R.SP, R.PC, R.FZ, R.FN, R.FH, R.FC);
}