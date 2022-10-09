

def extract(args):
    print("extract args:", args)

class ExtractArgs:
    def __init__(self, subparser, command, description) -> None:
        parser = subparser.add_parser(command,
                                      help=description,
                                      description=description,
                                      epilog="Questions and feedback: https://faceswap.dev/forum",
                                      #formatter_class=SmartFormatter
                                      )
        parser.set_defaults(func=extract)
        parser.add_argument('--test', type=int, help='test value')

