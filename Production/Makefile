#
# Visiolock - Main Makefile.
# 
# @author Matthias Brun adapted by Matthieu Bonhomme
#

# Répertoire d'installation des outils de développement
# pour Raspberry Pi à modifier suivant votre propre chemin
export RASPBERRY_TOOLS = ~/Documents/ProSE/tools/


# Compilateur croisé à utiliser
export RASPBERRY_GCC_x86_64 = $(RASPBERRY_TOOLS)arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Adresse de la Raspberry :
RASPBERRY_ADDRESS = 10.42.0.1

# Login/mot de passe de connexion à la Raspberry :
RASPBERRY_LOGIN = facesecure
RASPBERRY_PASSWORD = pfe

# Terminal à utiliser pour la connexion ssh (nécessite sshpass et ssh)
TERM = gnome-terminal # gnome-terminal # xterm
TERMOPTIONS = -- # -- (pour gnome-terminal) # -e (pour xterm)

RASPBERRY_PROG_LOCATION = Visiolock

#
# Organisation des sources.
#

export SRCDIR = src 
export BINDIR = bin

export DOC = doc

SUBDIRS = $(SRCDIR)

# Sysroot à modifier selon le chemin des librairies de la Raspberry.
export RASPBERRY_SYSROOT = $(shell realpath ~/Documents/ProSE/rootfs_zero/)

# Définitions des outils.
#

# Compilateur.

ifeq ($(TARGET), raspberry)
# pour la cible Raspberry depuis une architecture x86.
export CC = $(RASPBERRY_GCC_x86_64)
export CCFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
export LDFLAGS += --sysroot=$(RASPBERRY_SYSROOT)

else
# pour le pc de développement.
export CC = gcc

endif

export CCFLAGS += -MMD -MP # gestion automatique des dépendances
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L
export CCFLAGS += -std=c99 -Wall -pedantic -lpthread -Wextra -Wno-unused-variable -Wno-unused-parameter

export LDFLAGS += -lrt -lm -lpthread -lsqlite3 -lgpiod

#
# Définitions des binaires à générer.
#

ifeq ($(TARGET), raspberry)
# pour la cible Raspberry.
export PROG_NAME = Visiolock.out

else 
# pour le pc de développement.
export PROG_NAME = Visiolock_local.out
endif

export PROG = $(BINDIR)/$(PROG_NAME)

#
# Règles du Makefile.
#

.PHONY: all clean documentation $(SUBDIRS)

# Compilation.
all: $(SUBDIRS)

# Nettoyage.
clean: $(SUBDIRS)
	@rm -f $(PROG) core* $(BINDIR)/core*

# Sous-répertoires.
$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@

.PHONY: upload

upload:
ifeq ($(TARGET), raspberry)
	@sshpass -p '$(RASPBERRY_PASSWORD)' scp $(PROG) $(RASPBERRY_LOGIN)@$(RASPBERRY_ADDRESS):$(RASPBERRY_PROG_LOCATION)/$(PROG_NAME)
endif

#
# Arrêt du programme par ctrl+c
# (utile pour utilisation dans certaines IDE)
#

.PHONY: kill

kill:
	killall -s INT "$(PROG)"