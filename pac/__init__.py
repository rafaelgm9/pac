import numpy as np
import ctypes
import os
import glob


def pair_count(npart, d1, boxsize, nside, minsep, maxsep, nbins):
    lib_path = glob.glob(os.path.join(os.path.dirname(__file__), '../pac*.so'))[0]
    pac_lib = ctypes.cdll.LoadLibrary(lib_path)

    dptr = ctypes.POINTER(ctypes.c_double)
    longlongptr = ctypes.POINTER(ctypes.c_longlong)

    pair_count_run = pac_lib.DD
    pair_count_run.argtypes = [ctypes.c_longlong, dptr, ctypes.c_double, ctypes.c_int, ctypes.c_double, ctypes.c_double,
                               ctypes.c_int]
    pair_count_run.restype = longlongptr

    d1 = np.ascontiguousarray(d1)

    d1_ptr = d1.ctypes.data_as(dptr)

    print("running pair count")
    dd_ptr = pair_count_run(npart, d1_ptr, boxsize, nside, minsep, maxsep, nbins)
    dd = np.ctypeslib.as_array(dd_ptr, shape=(nbins,))

    return dd
