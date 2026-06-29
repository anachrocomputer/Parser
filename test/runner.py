# runner --- run all the C compiler tests                      2026-06-21

import os
from subprocess import Popen, PIPE
import glob

def runtest(testClass, name, path):
    print("Running '" + testClass + "', test '" + name + "' in '" + path + "'")
    src = path + ".c"
    asm = path + ".asm"
    lst = path + ".lst"
    hex = path + ".hex"

    args = ["../parser", src]
    #print(" ".join(args))

    lNum = 1
    with Popen(args, stderr=PIPE) as compiler:
        for line in compiler.stderr:
            print(lNum, line.decode("utf-8").rstrip())
            lNum += 1
            
            if lNum > 20:
                print("Compiler output grows without bound")
                break
    
    if compiler.returncode == -11:
        print("Compiler SEGFAULT")
    elif compiler.returncode == -13:
        print("Compiler terminated by SIGPIPE")
    elif compiler.returncode != 0:
        print("Compiler return code: ", compiler.returncode)
        
    # check here for a file called 'core'

    if compiler.returncode != 0:
        return

    args = ["/home/john/bin/asm6809", "--6809", "-H", "-o", hex, "-l", lst, asm]
    #print(" ".join(args))
    
    with Popen(args, stdout=PIPE) as assembler:
        msgs = assembler.stdout.read()
    
    msgs = msgs.decode("utf-8")
    
    if msgs != '':
        print("Assembler output: '" + msgs + "'")

    #print(assembler.returncode)
    if assembler.returncode != 0:
        return

    args = ["/home/john/bin/sim6809", "-n", "-g", "-q", hex]
    #print(" ".join(args))

    with Popen(args, stdout=PIPE) as simulator:
        msgs = simulator.stdout.read()
    
    print("Simulator output: '" + msgs.decode("utf-8") + "'")

    #print(simulator.returncode)
    #if simulator.returncode < 0:
    #    return


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
            input("Press Enter to continue...")
        
