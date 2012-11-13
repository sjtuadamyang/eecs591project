hyperdex-coordinator-control --host 127.0.0.1 --port 6970 add-space << EOF
space phonebook
dimensions username, first, last, phone
key username auto 1 3
subspace first, last, phone auto 2 3
EOF
