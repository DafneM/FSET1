CC = gcc
LDFLAGS = -lwiringPi -lpthread
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/bin
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = $(OBJDIR)/distributed

all: clean $(EXE) 

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@
    
$(EXE): $(OBJ) 
	$(CC) $(OBJDIR)/*.o $(LDFLAGS) -o $@ 

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)