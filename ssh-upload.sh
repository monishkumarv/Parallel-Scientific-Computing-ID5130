#!/bin/bash

# clear
# scp -r openacc/ ce18b118@aqua.iitm.ac.in:scratch/

# Upload remote
sshpass -p "<password>" scp -o StrictHostKeyChecking=no -r course-project/ ce18b118@aqua.iitm.ac.in:scratch/
echo "File upload successful!"
