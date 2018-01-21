CC = gcc
CFLAGS = -g -Wall

PROG = shell
HEADERS = shell.h shellvars.h shellstring.h
SOURCES = shell.c shellvars.c shellstring.c

OBJDIR = object
OBJECTS = $(OBJDIR)/shell.o $(OBJDIR)/shellvars.o $(OBJDIR)/shellstring.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/shell.o: shell.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellvars.o: shellvars.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellstring.o: shellstring.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)


