import argparse
from scripts.extract import ExtractArgs
from scripts.record import RecordArgs

def unknown_cmd(*args):
    print("unknown cmd: ", args)

def main():
    parser = argparse.ArgumentParser()
    
    subparser = parser.add_subparsers()
    ExtractArgs(subparser, "extract", "extract cmd")
    RecordArgs(subparser, "record", "record cmd")

    parser.set_defaults(func=unknown_cmd)
    arguments = parser.parse_args()
    arguments.func(arguments)

if __name__ == '__main__':
    main()