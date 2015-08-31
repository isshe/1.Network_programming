CFLAGS = -g -O2 -D_REENTRANT -Wall
PROGS = my_error.o mcast_join.o my_malloc.o my_socket.o send_recv.o \
		test_main.o udp_client.o my_sendrecv.o my_process.o \
		ga_aistruct.o ga_echeck.o ga_nsearch.o ga_serv.o ga_port.o\
		ga_clone.o	
test : 	
	gcc *.o -o test

ooo: $(PROGS)
%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o 
	rm -f test
