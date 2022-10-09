import argparse

class CmdParser:
    def __init__(self) -> None:
        parser = argparse.ArgumentParser()
        #subparser = parser.add_subparsers()
        parser.set_defaults(func=unknown_cmd)
        self.parser = parser

    @staticmethod
    def unknown_cmd(*args):
        pass

    def add
        