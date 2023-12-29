# some_script.py
import time

count = 0
while True:
    print(f"Tracked objects count: {count}")
    count += 1
    time.sleep(1)  # 每秒打印一次
