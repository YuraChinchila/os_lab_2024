import subprocess

subprocess.run(["kill -9 $(ps ax | grep tcpserver | awk '{ print $1; }')"], shell=True) 
subprocess.run(["kill -9 $(ps ax | grep udpserver | awk '{ print $1; }')"], shell=True) 
