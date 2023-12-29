import util
def get_peak_physical_memory_usage(pid):
    """
    获取指定PID进程的物理峰值内存使用量（VmHWM）
    """
    status_file = f'/proc/{pid}/status'
    try:
        with open(status_file, 'r') as file:
            for line in file:
                if 'VmHWM' in line:
                    return  int(line.split()[1])
    except IOError as e:
        raise e

def read_memory_info(pid):
    """
    读取指定PID进程的内存信息
    """
    mem_info = {}
    with open(f'/proc/{pid}/status', 'r') as file:
        for line in file:
            if 'VmRSS' in line or 'RssAnon' in line or 'RssFile' in line or 'RssShmem' in line:
                key, value = line.split(':')
                mem_info[key.strip()] = value.strip()
    return mem_info

import os
import time
import torch

f_pid = os.getpid()
# print("parent mem",get_peak_physical_memory_usage(f_pid), read_memory_info(f_pid))
# abs = "sdasdddddddda"
# print("torch version:",torch.__version__, hex(id(torch)), hex(id(abs)))
pid = os.fork()

if pid == 0:
    time.sleep(3)
   
    c_pid = os.getpid()
    # 创建两个128x128的随机矩阵
    matrix1 = torch.randn(128, 128)
    matrix2 = torch.randn(128, 128)

    # 执行矩阵乘法
    result = torch.matmul(matrix1, matrix2)
    util.log("pytorch real needs",get_peak_physical_memory_usage(c_pid))
    while True:
        time.sleep(1)
else:
    # exit(0)
    # os.waitpid(pid, 0)
    matrix1 = torch.randn(128, 128)
    matrix2 = torch.randn(128, 128)

    # 执行矩阵乘法
    result = torch.matmul(matrix1, matrix2)
    # util.log("pytorch original",get_peak_physical_memory_usage(f_pid))
    # time.sleep(10)
