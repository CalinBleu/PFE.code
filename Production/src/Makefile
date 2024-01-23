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

# Point d'entrée du programme.

MAIN = starter.c

# Gestion automatique des dépendances.

DEP = $(MAIN:.c=.d)

# Executable à générer.

EXEC = ../$(PROG)

# Inclusion depuis le niveau du package.

CCFLAGS += -I.


#
# Règles du Makefile.
#

.PHONY: all clean $(PACKAGES)

# Compilation.

all: $(PACKAGES)
	$(MAKE) $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

# Nettoyage.

clean: $(PACKAGES)
	@rm -f $(DEP)

# Packages

$(PACKAGES):
	$(MAKE) $(MAKECMDGOALS) -C $@

-include $(DEP)