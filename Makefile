CC := g++
SRCDIR := src
INC := -I include
LIBS := -lpthread -lws2_32
CFLAGS := -g
BUILDDIR := build
TARGETDIR := bin
TARGET := libsimpleserver.a
SRCEXT := cpp
SOURCES=$(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

$(TARGETDIR)/$(TARGET): $(OBJECTS)
	ar rvs $@ $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) $(LIBS) -c -o $@ $<

clean:
	-del /s $(TARGETDIR) $(BUILDDIR)

.PHONY: clean