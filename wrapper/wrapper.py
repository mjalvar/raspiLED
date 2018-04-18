from ctypes import cdll
lib = cdll.LoadLibrary('./libspi.so')

class LedDisplay(object):
    def __init__(self):
        self.obj = lib.LedDisplay_new()

    def demo(self):
        lib.LedDisplay_demo(self.obj)