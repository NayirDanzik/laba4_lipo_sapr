/*
    Virtual Machine main

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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "yla_vm.h"
#include "yla_cop.h"
#include "yla_type.h"
#include "yla_test.h"
#include "yla_test_gencode.c"
#include "yla_compliance.h"
#include "yla_word.h"

int char_to_int(char letter);

int main(int argc, char argv[])
{
	yla_int_type stack_size = 100;
	yla_int_type vartable_size = 1440;
	yla_int_type prg_size = 2048;
	yla_cop_type prg[HEADER_SIZE + prg_size];
	yla_cop_type *prog_ptr = prg;
	
	yla_int_type prog_counter = 0;
	yla_int_type *prog_counter_ptr = prog_counter;
	
	compliance_table link_table;
	compliance_table_init(&link_table, 40);
	
	yla_int_type global_var[8];
	
	global_var[0] = 0x0011;	
	global_var[1] = 0x0012;		
	global_var[2] = 0x0013;
	global_var[3] = 0x0014;		
	global_var[4] = 0x0015;	 
	
	put_header(&prog_ptr, stack_size, vartable_size, prg_size);
	
	char in[2 + 1];
	char out[3 + 1];
	
	strcpy(in, "in");
	strcpy(out, "out");
	
	yla_int_type i;
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(in); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(in[i]));		prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(out); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(out[i]));		prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, 0x0002);						prog_counter += 2;
	
	put_commd(&prog_ptr, CALO);							prog_counter++;
	put_value(&prog_ptr, 0x0250);						prog_counter += 2;	
	
	for (i = 0; i < strlen(out) * 3 + 1; i++) {
		put_commd(&prog_ptr, COUT);						prog_counter++;
		put_value(&prog_ptr, 0x0001);					prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CHALT);						prog_counter++;
	
	put_count_word_length(&prog_ptr, 0x0200, &link_table, &prog_counter, global_var);
	put_string_repeat(&prog_ptr, 0x0250, &link_table, &prog_counter, global_var, 0x0200);
	
	yla_vm vm;
	
	yla_cop_type *prog_linker_ptr = prg;
	int link_state = link_program(prog_linker_ptr, prog_counter, &link_table);
	
	yla_vm_init(&vm, &prg, (prg_size + HEADER_SIZE));
	
	if (link_state == 1) {
		yla_vm_run(&vm);
	} else if (link_state == -8) {
		vm.last_error = YLA_VM_LINKER_ERROR_TABLE_EMPTY;
	} else if (link_state == -9) {
		vm.last_error = YLA_VM_LINKER_ERROR_ADDRESS_NOT_FOUND;
	}
	
	yla_vm_done(&vm);
	
	return 0;
}