def log(type, info):
    f = open('log.csv', 'a')
    s = f'{get_args()} {type},{info}\n'
    f.write(s)
    print(s,end="")

def get_args():
    import sys
    return sys.argv[1]
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
def my_fork():
    import os
    pid = os.fork()
    if pid == 0:
        import time
        torch.randn(128,128)
        while True:
            time.sleep(1)
    else:
        return pid
if __name__ == "__main__":
    import torch
    import os
    import time
    start = time.time()
    pids = []
    for i in range(1):
        pid = my_fork()
        pids.append(pid)

    
    time.sleep(5)
    # kill all child process
    # for pid in pids:
    #     os.kill(pid, 9)

    # pid = os.fork()
    # if pid == 0:
    #     end = time.time()
    #     print("fork time:",end-start)
    #     c_pid = os.getpid()
    #     # torch.randn(128,128)
    #     print("torch real needs",read_memory_info(c_pid))
    # else:
    #     os.waitpid(pid, 0 )
    #     # torch.randn(128,128)
    #     print("torch original",read_memory_info(os.getpid()))
