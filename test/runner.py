# runner --- run all the C compiler tests                      2022-06-21

import os
import glob

def runtest(testClass, name, path):
    print("Running '" + testClass + "', test '" + name + "' in '" + path + "'")
    cc = "../parser " + path + ".c"
    asm = "asm6809 --6309 " + path + ".asm"
    sim = "sim6809 -n -g -q " + path + ".hex"
    print(cc)
    print(asm)
    print(sim)

for d in os.listdir():
    if os.path.isfile(d):
#       print("FILE: ", d)
        pass
    else:
#       print("DIR : ", d)
        
#       for f in os.listdir(path=d):
#           print("SUB-FILE: ", f)

        for c in glob.glob(d + os.sep + "*.c"):
#           print("C FILE: ", c)
#           print("TEST  : ", os.path.splitext(c))
            name = os.path.splitext(os.path.split(c)[1])[0]
            runtest(d, name, os.path.splitext(c)[0])
        
