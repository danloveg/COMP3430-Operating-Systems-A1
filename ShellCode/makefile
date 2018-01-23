CC = gcc
CFLAGS = -g -Wall

PROG = shell
HEADERS = shell.h shellvars.h shellstring.h shellfileio.h shellpipe.h
SOURCES = shell.c shellvars.c shellstring.c shellfileio.c shellpipe.c

OBJDIR = object
OBJECTS = $(OBJDIR)/shell.o $(OBJDIR)/shellvars.o $(OBJDIR)/shellstring.o $(OBJDIR)/shellfileio.o $(OBJDIR)/shellpipe.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/shell.o: shell.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellvars.o: shellvars.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellstring.o: shellstring.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellfileio.o: shellfileio.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shellpipe.o: shellpipe.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)


