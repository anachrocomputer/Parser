# runner --- run all the C compiler tests                      2026-06-21

import os
import subprocess
import glob

def runtest(testClass, name, path):
    print("Running '" + testClass + "', test '" + name + "' in '" + path + "'")
    src = path + ".c"
    asm = path + ".asm"
    lst = path + ".lst"
    hex = path + ".hex"

    args = ["../parser", src]
    print(" ".join(args))

    with subprocess.Popen(args, stdout=subprocess.PIPE) as compiler:
       msgs = compiler.stdout.read()
    
    print("Compiler output: '" + msgs.decode("utf-8") + "'")
    
    # check here for a file called 'core'

    args = ["/home/john/bin/asm6809", "--6809", "-H", "-o", hex, "-l", lst, asm]
    print(" ".join(args))
    
    with subprocess.Popen(args, stdout=subprocess.PIPE) as assembler:
       msgs = assembler.stdout.read()
    
    print("Assembler output: '" + msgs.decode("utf-8") + "'")

    args = ["/home/john/bin/sim6809", "-n", "-g", "-q", hex]
    print(" ".join(args))

    with subprocess.Popen(args, stdout=subprocess.PIPE) as simulator:
       msgs = simulator.stdout.read()
    
    print("Simulator output: '" + msgs.decode("utf-8") + "'")

    input("Press Enter to continue...")


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
        
