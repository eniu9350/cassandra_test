CC = gcc

all:
#	gcc `pkg-config --cflags dbus-glib-1` \
			`pkg-config --cflags dbus-1` \
			`pkg-config --cflags glib-2.0` \
			-I../../../install/apache-cassandra-1.0.7/interface/gen-c_glib/ \
			-I/home/realjin/install/thrift-0.8.0/lib/c_glib/src/ \
			test.c \
			`pkg-config --libs dbus-glib-1` \
			`pkg-config --libs dbus-1` \
			`pkg-config --libs glib-2.0` \
			-lthrift

	gcc -c `pkg-config --cflags thrift_c_glib` -I../../../install/apache-cassandra-1.0.7/interface/gen-c_glib/ test.c -o test.o 
	gcc -c `pkg-config --cflags thrift_c_glib`\
	../../../install/apache-cassandra-1.0.7/interface/gen-c_glib/cassandra.c -o cassandra.o
	gcc -c `pkg-config --cflags thrift_c_glib`\
	../../../install/apache-cassandra-1.0.7/interface/gen-c_glib/cassandra_types.c -o cassandra_types.o
	libtool --tag=CC --mode=link gcc `pkg-config --libs thrift_c_glib` -o test test.o cassandra.o cassandra_types.o
#	gcc -I/home/realjin/install/thrift-0.8.0/lib/c_glib/src/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/usr/include/glib-2.0/ -I../../../install/apache-cassandra-1.0.7/interface/gen-c_glib/ test.c -lglib-2.0
