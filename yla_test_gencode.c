/*
    Generate code for test Virtual Machine

    This file is part of YLA VM (Yet another Language for Academic purpose: Virtual Machine).

    YLA VM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "yla_test_gencode.h"
#include "yla_type.h"
#include "yla_vm.h"

void put_commd(yla_cop_type** prg, yla_cop_type value)
{
	**prg = value;
	(*prg)++;
}

void put_value(yla_cop_type** prg, yla_int_type value)
{
    // NOTE: it's work OK only if yla_cop_type is 1 byte
    int i;
    unsigned int mask = 0xff;
    unsigned int int_value = value;

    mask <<= (8*(sizeof(unsigned int)-1));
    int_value <<= (8*(sizeof(unsigned int) - sizeof(yla_int_type)));

    for (i=0; i<sizeof(yla_int_type); ++i) {
        unsigned int only_value = (int_value & mask);
        only_value >>= (8*(sizeof(unsigned int)-1));
        put_commd(prg, only_value);
        int_value <<= 8;
    }
}

void put_header(yla_cop_type** prg, size_t stack_size, size_t vartable_size, size_t code_size)
{
    put_value(prg, MAGIC_CODE1);
    put_value(prg, MAGIC_CODE2);
    put_value(prg, MAGIC_CODE3);
    put_value(prg, stack_size);
    put_value(prg, vartable_size);
    put_value(prg, code_size);
}

int link_program(yla_cop_type *prog_ptr, yla_int_type prog_counter, compliance_table *link_table)
{
	for (int i = 0; i < 12; i++) {
		yla_cop_type emptiness = (prog_ptr + i);	// skip header
		*(prog_ptr)++;
	}
	for (int i = 12; i < prog_counter + 12; i++) {
		if (i != (prog_counter + 12 - 1)) { 
			yla_cop_type value = *(prog_ptr);
			if ((value == CALO)		||
					(value == CJMP)	||
					(value == CJZ)		||
					(value == CJNZ)	||
					(value == CJE)		||
					(value == CJNE)
			) {
				yla_int_type op1 = *(prog_ptr + 1);
				yla_int_type op2 = *(prog_ptr + 2);
				
				yla_int_type mark = ((yla_int_type)op1 << 8) + (yla_int_type)op2;
				
				yla_int_type addr = compliance_table_get_addr(link_table, mark);
				
				yla_cop_type high_byte = addr >> 8;
				yla_cop_type low_byte = addr - (high_byte << 8);
					
				(prog_ptr) - 2;
				*(prog_ptr + 1) = high_byte;
				*(prog_ptr + 2) = low_byte;
				(prog_ptr) + 2;
			}
			
			*(prog_ptr)++;
		} else { 
			break;	
		}
	}
	
	return 1;
}
