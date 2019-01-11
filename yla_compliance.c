#include "yla_compliance.h"

void compliance_table_init(compliance_table* array, size_t size)
{
	array->size = size;
	array->count = 0;
	array->high_byte = calloc(array->size, sizeof(yla_cop_type));
	array->low_byte = calloc(array->size, sizeof(yla_cop_type));
	array->mark = calloc(array->size, sizeof(yla_cop_type));
}

yla_int_type compliance_table_get_addr(compliance_table* array, yla_int_type mark)
{
	if (array->count == 0) {
		return 0;
	}

	yla_int_type i = 0;
	do
	{
		if (array->mark[i++] == mark) {
			return ((array->high_byte[i-1] << 8) + array->low_byte[i-1]);
		}
	} while(i != array->count);
	
	return 0;
}

void compliance_table_set_addr(compliance_table* array, yla_int_type mark, size_t addr)
{
	addr++;	// vm->pc has numeration from 0
	array->high_byte[array->count] = addr >> 8;
	array->low_byte[array->count] = addr - (array->high_byte[array->count] << 8);
	array->mark[array->count] = mark;
	array->count++;
}

void compliance_table_done(compliance_table* array)
{
	array->size = 0;
	array->count = 0;
	free(array->high_byte);
	free(array->low_byte);
	free(array->mark);
}
