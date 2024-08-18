#!/bin/bash

URL="http://localhost:8082"  # 요청을 보낼 URL
REQUESTS=100                  # 반복할 요청 수

# 전체 시작 시간을 기록 (초와 나노초를 따로 기록)
overall_start_sec=$(date +%s)
overall_start_nsec=$(date +%N)

for ((i=1; i<=REQUESTS; i++))
do
    echo "Request #$i"
    
    # 각 요청의 시작 시간을 기록 (초와 나노초를 따로 기록)
    start_sec=$(date +%s)
    start_nsec=$(date +%N)
    
    # curl로 요청 보내기 (응답 내용은 숨김)
    curl -s $URL > /dev/null
    
    # 각 요청의 종료 시간을 기록 (초와 나노초를 따로 기록)
    end_sec=$(date +%s)
    end_nsec=$(date +%N)
    
    # 응답 시간 계산 (밀리초)
    elapsed_time=$(( (end_sec - start_sec) * 1000 + (end_nsec - start_nsec) / 1000000 ))
    
    echo "Response time: ${elapsed_time} ms"
    
done

# 전체 종료 시간을 기록 (초와 나노초를 따로 기록)
overall_end_sec=$(date +%s)
overall_end_nsec=$(date +%N)

# 전체 소요 시간 계산 (밀리초)
overall_elapsed_time=$(( ((overall_end_sec - overall_start_sec) * 1000 + (overall_end_nsec - overall_start_nsec) / 1000000) / 1000 ))

echo "Total time for $REQUESTS requests: ${overall_elapsed_time} sec"

