/*
	Virtual Machine code compliance table header
*/

#ifndef yla_compliance_h
#define yla_compliance_h

#include <stdlib.h>
#include "yla_cop.h"
#include "yla_type.h"

typedef struct {
	yla_int_type *mark;			
	yla_cop_type *high_byte;	
	yla_cop_type *low_byte;	
	yla_int_type size;
	yla_int_type count;
} compliance_table;

void compliance_table_init(compliance_table* array, size_t size);
void compliance_table_done(compliance_table* array);

yla_int_type compliance_table_get_addr(compliance_table* array, yla_int_type mark);
void compliance_table_set_addr(compliance_table* array, yla_int_type mark, size_t addr);

#endif
