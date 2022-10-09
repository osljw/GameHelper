#! /usr/bin/env python
# coding=utf8

import grpc

from proto import record_pb2_grpc, record_pb2

def run():
    '''
    模拟请求服务方法信息
    :return:
    '''
    conn = grpc.insecure_channel('localhost:50051')
    client = record_pb2_grpc.RecordStub(channel=conn)
    request = record_pb2.RecordRequest(name="test")
    response = client.Start(request)
    print("received:", response)

if __name__ == '__main__':
    run()
