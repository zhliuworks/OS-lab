#!/bin/bash
output=test.upipe.out
if [ -e $output ]; then
    rm $output
fi

make
./copyfile
diff test.in test.upipe.out > /dev/null
if [ $? -eq 0 ]; then
    echo passed
else
    echo NOTpassed
fi