G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

all: Reader Admin

Reader.o: Reader.cpp
	$(G) -c $(CFLAGS) Reader.cpp

Reader: Reader.o
	$(G) $(LFLAGS) -o Reader Reader.o 
	
Admin.o: Admin.cpp
	$(G) -c $(CFLAGS) Admin.cpp

Admin: Admin.o
	$(G) $(LFLAGS) -o Admin Admin.o 

clean:
	rm -f *.o Reader Admin
