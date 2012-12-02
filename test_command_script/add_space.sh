hyperdex-coordinator-control --host 127.0.0.1 --port 6970 add-space << EOF
space user
dimensions username, password
key username auto 0 3
EOF

hyperdex-coordinator-control --host 127.0.0.1 --port 6970 add-space << EOF
space userphoto
dimensions username, 

