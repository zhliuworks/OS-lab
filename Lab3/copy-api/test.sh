#!/bin/bash

name=(copy fcopy fcopyf fcopyc fcopys)
times=1
totalcost=0

make
echo "------------------------------------"
echo "test.in [10.00 MB]     [$times] iters"
echo "------------------------------------"
echo "Program    Passed?    Time(ms)"
echo "------------------------------------"
for (( i=0;i<5;i++ )) do
    # test for correctness
    ./copyfile $i
    diff test/test.in test/test.${name[i]}.out > /dev/null
    if [ $? -eq 0 ]; then
        res=passed
    else
        res=NOTpassed
    fi
    # test for time cost
    totalcost=0
    for (( j=0;j<$times;j++ )) do
        start=$(date +%s%N)
        ./copyfile $i
        end=$(date +%s%N)    
        cost=`echo "scale=2; ($end-$start)/1000000" | bc`
        totalcost=`echo "$cost+$totalcost" | bc`
    done;
    totalcost=`echo "scale=2; $totalcost/$times" | bc`
    printf "%-10s %-10s %.2f\n" ${name[i]} $res $totalcost
done;
echo "------------------------------------"