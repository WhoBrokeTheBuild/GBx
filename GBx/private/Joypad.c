#include <GBx/Joypad.h>

#include <GBx/Log.h>

bool RightPressed;
bool LeftPressed;
bool UpPressed;
bool DownPressed;
bool APressed;
bool BPressed;
bool SelectPressed;
bool StartPressed;

joypad_state_t JOYP;

void ResetJoypad()
{
    JOYP.raw = 0x00;
    UpdateJoypad();
}

void UpdateJoypad()
{
    JOYP.InputStates = 0xF;

    if (JOYP.SelectButtons) {
        LogVerbose(2, "Updating Buttons");

        JOYP.A &= !APressed;
        JOYP.B &= !BPressed;
        JOYP.Select &= !SelectPressed;
        JOYP.Start &= !StartPressed;
    }

    if (JOYP.SelectDPad) {
        LogVerbose(2, "Updating DPad");

        JOYP.Right &= !RightPressed;
        JOYP.Left &= !LeftPressed;
        JOYP.Up &= !UpPressed;
        JOYP.Down &= !DownPressed;
    }
}

void PrintJoypad()
{
    LogInfo("JOYP DPad=%d Buttons=%d U=%d D=%d L=%d R=%d A=%d B=%d START=%d "
            "SELECT=%d",
        JOYP.SelectDPad,
        JOYP.SelectButtons,
        JOYP.Up,
        JOYP.Down,
        JOYP.Left,
        JOYP.Right,
        JOYP.A,
        JOYP.B,
        JOYP.Start,
        JOYP.Select);
}