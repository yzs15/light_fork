#!/bin/bash

# 运行 Python 脚本并通过管道读取输出
/usr/bin/python3 -u some_script.py | while read line; do
    echo "Received: $line"
    if [[ "$line" == *"Tracked objects count"* ]]; then
        echo "Detected 'Tracked objects count' in the output."
        # 在这里可以添加进一步的处理逻辑
        # 如果需要终止 Python 脚本，可以使用 kill 命令
    fi
done
