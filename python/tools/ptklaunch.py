import ptk.common as ptk_common
import argparse
import ptk
import time

import sys

def main():
    parser = argparse.ArgumentParser(description='Launcher to run programs.')
    parser.add_argument("-p", "--path", type=str, help="Path to executable")
    parser.add_argument('argv', nargs='*')
    args = parser.parse_args()


    if not args.path:
        print("Path to executable not provided")
        sys.exit(0)

    program = ptk_common.Process(args.path, args.argv)
    program.start()

    # popen = subprocess.Popen(cmd, stdout=subprocess.PIPE) # Call subprocess
    # poll_result = popen.poll()

    # if poll_result is None or poll_result == 0:
    #     print("Started process {}".format(popen.pid))
    # else:
    #     print("Process failed to start")

    while not ptk.is_shutdown() and program.isAlive():
        time.sleep(1)
        pass

    # while okay():
    #     time.sleep(1)
    #     poll_result = popen.poll()
    #     print(_signal_name_from_exitcode(poll_result))
    #     output, error  = popen.communicate()
    #     print(output.decode("utf-8"))
    #     # print(error)

    # poll_result = popen.poll()
    # print(_signal_name_from_exitcode(poll_result)) 

    print(program.getProcessStats())
    program.shutdown()

if __name__ == "__main__":
    sys.exit(main())