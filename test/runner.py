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
        return (False, "Compiler SEGFAULT")
    elif compiler.returncode == -13:
        print("Compiler terminated by SIGPIPE")
        return (False, "Compiler loop")
    elif compiler.returncode != 0:
        print("Compiler return code: ", compiler.returncode)
        return (False, "Compiler returned %d" % compiler.returncode)
        
    # check here for a file called 'core'

    args = ["/home/john/bin/asm6809", "--6809", "-H", "-o", hex, "-l", lst, asm]
    #print(" ".join(args))
    
    with Popen(args, stdout=PIPE) as assembler:
        msgs = assembler.stdout.read()
    
    msgs = msgs.decode("utf-8")
    
    if msgs != '':
        print("Assembler output: '" + msgs + "'")

    #print(assembler.returncode)
    if assembler.returncode != 0:
        return (False, "Assembler")

    args = ["/home/john/bin/sim6809", "-n", "-g", "-q", hex]
    #print(" ".join(args))

    with Popen(args, stdout=PIPE) as simulator:
        msgs = simulator.stdout.read()
    
    print("Simulator output: '" + msgs.decode("utf-8", errors="replace") + "'")

    #print(simulator.returncode)
    #if simulator.returncode < 0:
    #    return
    
    return (True, "")


with open("results.html", "w") as html:
    html.write("<HTML>\n")
    html.write("<HEAD>\n")
    html.write("<TITLE>Compiler Test Results</TITLE>\n")
    html.write("</HEAD>\n")
    html.write("<BODY>\n")
    html.write("<H1 ALIGN=CENTER>Compiler Test Results</H1>\n")
    html.write("<TABLE COLS=3 BORDER=2>\n")

    for d in os.listdir():
        if os.path.isfile(d):
#           print("FILE: ", d)
            pass
        else:
#           print("DIR : ", d)
            html.write("<TR ALIGN=CENTER><TD COLSPAN=3>%s</TD></TR>\n" % d)
            
#           for f in os.listdir(path=d):
#               print("SUB-FILE: ", f)

            for c in glob.glob(d + os.sep + "*.c"):
#               print("C FILE: ", c)
#               print("TEST  : ", os.path.splitext(c))
                name = os.path.splitext(os.path.split(c)[1])[0]
                (result, msg) = runtest(d, name, os.path.splitext(c)[0])
                html.write("<TR ALIGN=LEFT>\n")
                html.write("<TD>%s</TD>\n" % c)
                if result:
                    html.write("<TD COLOR=\"#00ff00\">PASS</TD>\n")
                else:
                    html.write("<TD COLOR=\"#ff0000\">FAIL</TD>\n")
                html.write("<TD>%s</TD>\n" % msg)
                html.write("</TR>\n")
                input("Press Enter to continue...")
            
    html.write("</TABLE>\n")
    html.write("</BODY>\n")
    html.write("</HTML>\n")
