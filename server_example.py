import flask
from flask import request
import numpy
import linpack
app = flask.Flask(__name__)
import torch
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)

@app.route('/',methods=['POST'])
def face():
    data_json = request.get_json()
    face = data_json['face']
    torch 

@app.route('/', methods=['POST'])
def hello_world():
    data_json = request.get_json()
    num = data_json['num']
    a = numpy.random.randn(num, num)
    b = numpy.random.randn(num, num)
    c = numpy.matmul(a, b)
    return 'Hello, World!'

@app.route('/linpack', methods=['POST'])
def linpack():
    data_json = request.get_json()
    num = data_json['num']
    a = numpy.random.randn(num, num)
    b = numpy.random.randn(num, num)
    c = numpy.matmul(a, b)
    return 'Hello, World!'

if __name__ == '__main__':
    import os
    pid = os.fork() 
    if pid == 0:
        set_mem_file_name("numpy.mem")
        app.run(port=5000)
    else:
        pass