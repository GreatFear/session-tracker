all: session frontend

frontend: frontend.c
	gcc -g pkg-config --cflags gtk+-3.0 frontend.c -o timer pkg-config --libs gtk+-3.0

session: session.c
	gcc session.c -o test -I$(pg_config --includedir) -L$(pg_config --libdir) -lpq
