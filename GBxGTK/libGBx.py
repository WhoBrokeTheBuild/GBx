
from enum import Enum
from ctypes import *

c_uint8_p = POINTER(c_uint8)

# TODO: Windows
libGBx = CDLL('libGBx.so')
libGBx.GBx_GetVersionString.restype = c_char_p
libGBx.GBx_Init.restype = c_bool
libGBx.GBx_GetClockSpeed.restype = c_uint
libGBx.GBx_Step.restype = c_uint
libGBx.GBx_PPU_GetPixelData.restype = c_uint8_p

class clog:

    CALLBACK_FUNC = CFUNCTYPE(None, py_object, c_int, c_char_p)

    init = libGBx.clog_init
    term = libGBx.clog_term

    def add_callback(func, userData):
        return libGBx.clog_add_callback(func, py_object(userData))

    def remove_callback(func, userData):
        return libGBx.clog_remove_callback(func, py_object(userData))

class GBx:

    SERIAL_READ_FUNC = CFUNCTYPE(None, py_object, c_uint8)
    SERIAL_WRITE_FUNC = CFUNCTYPE(c_uint8, py_object)

    class Model(Enum):
        DMG = 0
        MGB = 1
        SGB = 2
        CGB = 3

    SCREEN_WIDTH      = 160
    SCREEN_HEIGHT     = 144
    SCREEN_COMPONENTS = 3

    def __init__(self):
        self.ctx = c_void_p(0)

    @staticmethod
    def GetVersionString():
        return libGBx.GBx_GetVersionString().decode()

    @staticmethod
    def GetVersion():
        major = c_int(0)
        minor = c_int(0)
        patch = c_int(0)

        libGBx.GBx_GetVersion(
            pointer(major),
            pointer(minor),
            pointer(patch)
        )

        return (major, minor, patch)

    def Init(self):
        return libGBx.GBx_Init(byref(self.ctx))

    def Term(self):
        return libGBx.GBx_Term(byref(self.ctx))

    def Reset(self):
        return libGBx.GBx_Reset(self.ctx)

    def GetClockSpeed(self):
        return libGBx.GBx_GetClockSpeed(self.ctx)

    def Step(self):
        return libGBx.GBx_Step(self.ctx)

    def Frame(self):
        return libGBx.GBx_Frame(self.ctx)

    def Cartridge_Load(self, filename):
        return libGBx.GBx_Cartridge_Load(self.ctx, c_char_p(filename))

    def PPU_GetPixelData(self):
        return libGBx.GBx_PPU_GetPixelData(self.ctx)

    def Serial_SetCallbacks(self, readFunc, writeFunc, userData):
        return libGBx.GBx_Serial_SetCallbacks(
            self.ctx,
            readFunc,
            writeFunc,
            py_object(userData)
        )
