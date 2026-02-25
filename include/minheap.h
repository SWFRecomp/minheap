#pragma once

#include <stdint.h>

typedef struct
{
	uintptr_t value;
	
	char* restrict arena;
	size_t length;
	size_t arena_capacity;
} Minheap;

void minheap_init(Minheap* m);
void minheap_insert(Minheap* m, uintptr_t value);
uintptr_t minheap_peek(Minheap* m);
void minheap_pop(Minheap* m);