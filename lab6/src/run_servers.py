import subprocess

with open("servers.txt", "r") as f:
    for ip, port in map(lambda x: x.split(), f.readlines()):
        # print(f"Start server {ip}:{port}", subprocess.run(["./server", "--port", port, "--tnum", "2", "&"])); 
        print(f"Start server {ip}:{port}", subprocess.run([f"./server --port {port} --tnum 2 &"], shell=True)); 
