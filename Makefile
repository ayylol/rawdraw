TARGET:=a.out

# Compiler Stuff
CC:=gcc
CFLAGS=-Wall $(LIBFLAGS)
DBGFLAGS:=-g -Wextra -Werror
RLSFLAGS:=-O2
LIBFLAGS:=

# Directories
SRCDIR:=src
OBJDIR:=build
SRCDIRS:=$(patsubst %/,%,$(dir $(wildcard $(SRCDIR)/*/ $(SRCDIR)/*/*/))) $(SRCDIR)

# Input/Output Files For Compiler
SRCS := $(wildcard *.c $(foreach fd, $(SRCDIRS), $(fd)/*.c))
INCS := $(wildcard *.h $(foreach fd, $(SRCDIRS), $(fd)/*.h))
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
INC_DIRS = $(addprefix -I , $(SRCDIRS))

.PHONY: all debug release echo clean dremake rremake
all: release

debug: CFLAGS+=$(DBGFLAGS)
debug: $(TARGET)

release: CFLAGS+=$(RLSFLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIRS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

rremake: clean release
dremake: clean debug

echo: 
	@echo "Source Dirs: $(SRCDIRS)"
	@echo "Includes: $(INCS)"
	@echo "Sources: $(SRCS)"
	@echo "Objects: $(OBJS)"

