CC = g++
FLAGS = -Wextra -Wall -g -I/tmp/tmux-12016/newcode
LIBS = -lpthread
OBJS = functions.o main.o

.cpp.o:
	$(CC) $(FLAGS) -c $<

a.out: $(OBJS)
	$(CC) $(FLAGS) $(LIBS) -o $@ $(OBJS)

.PHNOY: clean
clean:
	rm *.o a.out