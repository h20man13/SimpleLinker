ROOTDIR:=./
MAKEDIR:=$(ROOTDIR)
SRCDIR:=$(ROOTDIR)./Src
INCLUDEDIR:=$(ROOTDIR)./Include
OBJDIR:=$(ROOTDIR)./Obj
BINDIR:=$(ROOTDIR)./Bin

SRCFILES:=$(wildcard $(SRCDIR)/*.c)
OBJFILES:=$(SRCFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HEADERFILES:=$(wildcard $(INCLUDEDIR)/*.h)

OUTPUTEXE:=$(BINDIR)/Linker.exe

all: $(OBJDIR) $(OBJFILES) $(BINDIR) $(OUTPUTEXE)

$(OUTPUTEXE): $(OBJFILES)
	clang -I$(INCLUDEDIR) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	clang -c $< -I$(INCLUDEDIR) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

clean: 
	rm -rf $(OBJFILES) $(OUTPUTEXE)
	rm -rf $(OBJDIR) $(BINDIR)




