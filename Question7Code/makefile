CC = gcc
CFLAGS = -g -lpthread -Wall

PROG = minelement
SOURCES = minelement.c

OBJDIR = object
OBJECTS = $(OBJDIR)/minelement.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/minelement.o: minelement.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)

