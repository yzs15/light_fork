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

import cv2
f_pid = os.getpid()
# print("parent mem",get_peak_physical_memory_usage(f_pid), read_memory_info(f_pid))

pid = os.fork()

if pid == 0:
    import time
    time.sleep(3)
   
    c_pid = os.getpid()
    cv2.setNumThreads(1)
    
    image_path = 'test.jpeg'
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)  
    blurred_image = cv2.GaussianBlur(image, (5, 5), 0)
    edges = cv2.Canny(blurred_image, 50, 150) 
    cv2.imwrite('edges_'+image_path, edges)
    util.log("cv2 real needs",get_peak_physical_memory_usage(c_pid))
else:
    # os.waitpid(pid, 0 )
    cv2.setNumThreads(1)
    image_path = 'test.jpeg'
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)  
    blurred_image = cv2.GaussianBlur(image, (5, 5), 0)
    edges = cv2.Canny(blurred_image, 50, 150) 
    cv2.imwrite('edges_'+image_path, edges)
    util.log("cv2 original",get_peak_physical_memory_usage(f_pid))