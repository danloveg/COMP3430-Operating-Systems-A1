CC = gcc
CFLAGS = -g -Wall

PROG = shell
HEADERS = shell.h
SOURCES = shell.c

OBJDIR = object
OBJECTS = $(OBJDIR)/shell.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/shell.o: shell.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)


