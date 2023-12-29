source venv/bin/activate

export PYTHONMALLOC=malloc
ulimit -c unlimited
echo "type, peakMemory(Bytes)" > log.csv



LD_PRELOAD=./libipdos.so python3 memory_profiling_pytorch.py optimized
sleep 5
pkill -1 -f "python3 memory_profiling_pytorch.py"
python3 memory_profiling_pytorch.py forked
sleep 5
pkill -1 -f "python3 memory_profiling_pytorch.py"
# sleep 5
# python3 memory_profiling_pytorch.py forked
# sleep 5


# LD_PRELOAD=./libipdos.so python3 memory_profiling_numpy.py optimized
# sleep 5
# python3 memory_profiling_numpy.py forked
# sleep 5

# LD_PRELOAD=./libipdos.so python3 memory_profiling_requests.py optimized
# sleep 5
# python3 memory_profiling_requests.py forked
# sleep 5 

# LD_PRELOAD=./libipdos.so python3 memory_profiling_cv2.py optimized
# sleep 5
# python3 memory_profiling_cv2.py forked
# sleep 5