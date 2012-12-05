g++ photo.cc base64.cc -lMagick++ -lglog -lhyperclient -fPIC -shared -o photo.so
g++ dummy.cc -lglog -lhyperclient -fPIC -shared -o dummy.so
g++ dummyS.cc -lglog -lhyperclient -fPIC -shared -o dummyS.so
g++ getpid.cc -lglog -lhyperclient -fPIC -shared -o getpid.so
g++ null.cc -lglog -lhyperclient -fPIC -shared -o null.so
