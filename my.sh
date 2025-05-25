

#!/bin/bash
for ((i=1; i<=9; i++)); do
    nohup ./client 192.168.201.128 5085 >/dev/null 2>&1 &
done




