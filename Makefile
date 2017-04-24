# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = smash.o jobs.o commands.o signals.o
OBJS-a = prog-a.o
OBJS-b = prog-b.o
OBJS-c = prog-c.o
OBJS-d = prog-d.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
prog-a: $(OBJS-a)
	$(CCLINK) -o prog-a $(OBJS-a)
prog-b: $(OBJS-b)
	$(CCLINK) -o prog-b $(OBJS-b)
prog-c: $(OBJS-c)
	$(CCLINK) -o prog-c $(OBJS-c)
prog-d: $(OBJS-d)
	$(CCLINK) -o prog-d $(OBJS-d)
# Creating the object files
jobs.o: jobs.c jobs.h
commands.o: commands.c commands.h
smash.o: smash.c commands.h
signals.o: signals.c signals.h
prog-a.o: prog-a.c
prog-b.o: prog-b.c
prog-c.o: prog-c.c
prog-d.o: prog-d.c
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

