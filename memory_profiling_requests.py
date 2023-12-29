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

import requests
f_pid = os.getpid()
print("parent mem",get_peak_physical_memory_usage(f_pid), read_memory_info(f_pid))

pid = os.fork()

if pid == 0:
    import time
    time.sleep(5)
    # os.waitpid(f_pid, 0 )
    c_pid = os.getpid()
    requests.get("http://www.baidu.com")
    util.log("requests real needs",get_peak_physical_memory_usage(c_pid))
else:
    # os.waitpid(pid, 0 )
    requests.get("http://www.baidu.com")
    util.log("requests original",get_peak_physical_memory_usage(f_pid))
   
