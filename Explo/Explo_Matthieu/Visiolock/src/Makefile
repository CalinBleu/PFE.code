#
# Organisation des sources.
#

# Packages.

PACKAGES = archivist \
			ai \
			brain \
			doorman \
			guard \
			manager \
			rfid \
			gui \

# Deux niveaux de packages sont accessibles.

SRC  = $(wildcard */*.c)		
SRC += $(wildcard */*/*.c)
OBJ = $(SRC:.c=.o)


# Gestion automatique des dépendances.

DEP = $(MAIN:.c=.d)

# Executable à générer.

EXEC = ../$(PROG)


export CC = /usr/bin/aarch64-linux-gnu-gcc


#
# Règles du Makefile.
#

.PHONY: all clean $(PACKAGES)

# Compilation.

all: $(PACKAGES)
	$(MAKE) $(EXEC)

$(EXEC): $(OBJ) 
	$(CC) -c $(CCFLAGS) $(OBJ) -MF $(DEP) -o $(EXEC)

# Nettoyage.

clean: $(PACKAGES)
	@rm -f $(DEP)

# Packages

$(PACKAGES):
	$(MAKE) $(MAKECMDGOALS) -C $@

-include $(DEP)
