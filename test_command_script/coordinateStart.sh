hyperdex-coordinator --control-port 6970 --host-port 1234 --logging debug
cd ~/data 
hyperdex-daemon --host 127.0.0.1 --port 1234 --bind-to 127.0.0.2
