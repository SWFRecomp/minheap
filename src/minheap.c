#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <minheap.h>

#define LIKELY(exp) __builtin_expect(exp, true)
#define UNLIKELY(exp) __builtin_expect(exp, false)

#define VAL(type, x) *((type*) x)
#define UP(x) VAL(uintptr_t, x)
#define UPADDR(x) ((uintptr_t*) &x)

#define ENSURE_SIZE(ptr, new_size, capac, elem_size) \
	if (UNLIKELY(new_size >= capac)) \
	{ \
		grow_ptr((char**) &ptr, &capac, elem_size); \
	}

void grow_ptr(char** ptr, size_t* capacity_ptr, size_t elem_size)
{
	char* data = *ptr;
	size_t capacity = *capacity_ptr;
	size_t old_data_size = capacity*elem_size;
	
	char* new_data = malloc(old_data_size << 1);
	
	memmove(new_data, data, old_data_size);
	
	free(data);
	
	*ptr = new_data;
	*capacity_ptr = capacity << 1;
}

void minheap_init(Minheap* m)
{
	m->length = 0;
	m->arena_capacity = 64*sizeof(uintptr_t);
	m->arena = malloc(m->arena_capacity*sizeof(uintptr_t));
}

void minheap_insert(Minheap* m, uintptr_t value)
{
	m->length += sizeof(uintptr_t);
	ENSURE_SIZE(m->arena, m->length, m->arena_capacity, 1);
	
	char* this = &m->arena[m->length - sizeof(uintptr_t)];
	UP(this) = value;
	
	char* first = (char*) &m->arena[0];
	
	while (this > first)
	{
		char* parent = (((this - first - sizeof(uintptr_t)))/2 & (~0b111)) + first;
		
		if (UP(parent) > value)
		{
			UP(parent) ^= value;
			UP(this) ^= UP(parent);
			UP(parent) ^= UP(this);
			this = parent;
		}
		
		else
		{
			break;
		}
	}
}

uintptr_t minheap_peek(Minheap* m)
{
	return UP(&m->arena[0]);
}

void minheap_pop(Minheap* m)
{
	m->length -= sizeof(uintptr_t);
	
	if (UNLIKELY(m->length == 0))
	{
		return;
	}
	
	char* this = &m->arena[m->length];
	UP(&m->arena[0]) = UP(this);
	this = (char*) &m->arena[0];
	
	uintptr_t value = UP(this);
	
	char* first = &m->arena[0];
	char* last = &m->arena[m->length - sizeof(uintptr_t)];
	
	while (this < last)
	{
		char* left = 2*((this - first)) + sizeof(uintptr_t) + first;
		
		if (UNLIKELY(left > last))
		{
			break;
		}
		
		char* right = 2*((this - first)) + 2*sizeof(uintptr_t) + first;
		
		char* min;
		
		if (UNLIKELY(right > last))
		{
			min = left;
		}
		
		else
		{
			min = UP(left) < UP(right) ? left : right;
		}
		
		if (UP(min) < value)
		{
			UP(min) ^= value;
			UP(this) ^= UP(min);
			UP(min) ^= UP(this);
			this = min;
		}
		
		else
		{
			break;
		}
	}
}

int main()
{
	Minheap m;
	
	minheap_init(&m);
	
	minheap_insert(&m, 4);
	minheap_insert(&m, 3);
	minheap_insert(&m, 6);
	minheap_insert(&m, 8);
	minheap_insert(&m, 2);
	minheap_insert(&m, 7);
	minheap_insert(&m, 5);
	minheap_insert(&m, 1);
	
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	fprintf(stderr, "got: 0x%08lX\n", minheap_peek(&m));
	minheap_pop(&m);
	
	return 0;
}