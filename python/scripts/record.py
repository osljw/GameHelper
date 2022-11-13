import time
import datetime
from .record_mouse import MouseRecord, mouse_data
from .record_keyboard import KeyboardRecord, keyboard_data

from flask import Flask, jsonify

# import grpc
# from concurrent import futures
# from proto import record_pb2_grpc, record_pb2

# class RecordServicer(record_pb2_grpc.RecordServicer):
#     def Start(self, request, context):
#         """Sends a greeting
#         """
#         print("Start called")
#         self.mr = MouseRecord()
#         self.kr = KeyboardRecord()

#         self.mr.start()
#         self.kr.start()
#         return record_pb2.RecordReply(message="Start")

#     def Stop(self, request, context):
#         print("Stop Called")
#         self.kr.stop()
#         self.mr.stop()

#         self.mr.join()
#         self.kr.join()
#         return record_pb2.RecordReply(message="Stop")

app = Flask(__name__)

mr = MouseRecord()
kr = KeyboardRecord()
start_time = datetime.datetime.now()

@app.route('/start_record', methods=['GET'])
def start_record():
    global mr, kr, start_time

    mouse_data.clear()
    keyboard_data.clear()

    if mr.listener.is_alive():
        mr.stop()
    if kr.listener.is_alive():
        kr.stop()

    mr = MouseRecord()
    kr = KeyboardRecord()

    mr.start()
    kr.start()

    return jsonify({'result': 'start success'})

def save_data(mouse_data, keyboard_data):
    pass

@app.route('/stop_record', methods=['GET'])
def stop_record():
    global mr, kr

    mr.stop()
    kr.stop()

    return jsonify({'result': 'stop success', 'data': keyboard_data})



def record(args):
    print("record args:", args)
    start_time = datetime.datetime.now()
    # mr = MouseRecord()
    # kr = KeyboardRecord()

    # mr.start()
    # kr.start()

    # time.sleep(3)
    # kr.stop()
    # mr.stop()

    # mr.join()
    # kr.join()

    # server = grpc.server(futures.ThreadPoolExecutor(max_workers=2))
    # record_pb2_grpc.add_RecordServicer_to_server(RecordServicer(), server)
    # server.add_insecure_port("localhost:50051")
    # server.start()
    # print("grpc server start...")
    # server.wait_for_termination()
 
    app.run(host="0.0.0.0", port=8383, debug=False)


class RecordArgs:
    def __init__(self, subparser, command, description) -> None:
        parser = subparser.add_parser(command,
                                      help=description,
                                      description=description,
                                      epilog="record mouse and keyboard",
                                      #formatter_class=SmartFormatter
                                      )
        parser.set_defaults(func=record)
        parser.add_argument('--output', type=str, help='output file')

if __name__ == '__main__':
    record()