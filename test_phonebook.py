#!/usr/bin/python
import hyperclient
c = hyperclient.Client('127.0.0.1', 1234)
c.put('phonebook', 'jsmith1', {'first': 'John', 'last': 'Smith', 'phone':123456})
print c.get('phonebook', 'jsmith1')
