#! python2.7 or python3.x

# ThrowSwitch.py
# Michael Adams, unquietwiki.com
# Current version: Nov 7, 2016

# Copyright (c) 2016 Michael Adams & contributors
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# ====================
# NOTE: please consult http://pythonhosted.org/psutil/ on ideas for expanding / modifying this. This program relies on how that library works, but I got nothing to do with them.
# TODO: add option to guarantee alternate program stays running, if it is closed early.
# ====================

# Requires "pip install psutil", or "easy_install psutil" before use.
import argparse, os, subprocess, sys, time, psutil

# Scan for a process, and indicate its existence
def procScan(procName,kill=False):
    for pid in psutil.pids():
        if pid != os.getpid():
            p = psutil.Process(pid)
            try:
                if procName in p.cmdline():
                    if kill is True:
                        p.kill()
                    return True
            except Exception as e:
                print(e.message)
                pass
    return False

# Process handler: if mainProc is detected, kill altProc; otherwise start altProc
def procHandler(mainProc,altProc,seconds):
    runServer = True
    startedAlt = False
    while runServer is True:
        try:
            print("Waiting " + str(seconds) + " seconds")
            time.sleep(float(seconds))
            # Main program is running
            if procScan(mainProc) is True:
                # Alt program is running: stop it
                if startedAlt is True:
                    print("Stopping " + altProc)
                    procScan(altProc,True)
                    startedAlt = False
                else:
                    print("Main program should be running")
            # Main program isn't running
            else:
                if startedAlt is False:
                    print("Starting " + altProc)
                    psutil.Popen(altProc)
                    startedAlt = True
                else:
                    print("Alternate should be running")
        except KeyboardInterrupt:
            print("Exiting program")
            runServer = False

# Main program
def main():
    # Define command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("processMain",help="Full path of process to keep running")
    parser.add_argument("processAlt",help="Full path of alternate process to run")
    parser.add_argument("seconds",help="Seconds to wait between checks")
    args = parser.parse_args()
    oops = False
    # Ensure it has complete set of arguments
    if args.processMain is not None:
        if args.processAlt is not None:
            if args.seconds is not None:
                if args.processMain != args.processAlt:
                    procHandler(args.processMain,args.processAlt,args.seconds)
                else:
                    print("Running against itself???")
                    oops = True
            else:
                oops = True
        else:
            oops = True
    else:
        oops = True
    if oops is True:
        print("Please specify, in order, the Path+EXE to check for, the Path+EXE to run as an alternate, and the seconds to wait between checks. Thanks!")

    # Exit
    sys.exit(0)

# Launch program
if __name__ == "__main__":
    main()
