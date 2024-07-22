

CC=gcc
CCOPTS= --std=gnu99 -Wall -Wextra
AR=ar  #archiviatore 
RM=rm #-f  #rimuovere file 

# File oggetto
OBJS=bit_map.o buddy_allocator.o pseudo_malloc.o pseudo_malloc_test.o

# File header
HEADERS=bit_map.h buddy_allocator.h pseudo_malloc.h

# Nome della libreria e dell'eseguibile
LIBS=libbuddy.a
BINS=pseudo_malloc_test

# Regole di compilazione
.PHONY: clean all

all: $(BINS)

# Regola per creare l'eseguibile
$(BINS): $(OBJS) $(LIBS)
	$(CC) $(CCOPTS) -o $@ $(OBJS) -L. -lbuddy -lm

# Regola per creare la libreria statica
$(LIBS): bit_map.o buddy_allocator.o
	$(AR) -rcs $@ $^

# Regola generica per compilare i file .c in .o
%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c $< -o $@

# Pulizia dei file generati
clean:
	$(RM) $(OBJS) $(LIBS) $(BINS)