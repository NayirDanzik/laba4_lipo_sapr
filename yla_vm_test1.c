/*
    Virtual machine Test #1

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
#include "yla_vm.h"
#include "yla_cop.h"
#include "yla_test.h"
#include "yla_test_gencode.h"
#include "yla_word.h"

static int test_gencode()
{
    yla_cop_type buf[100];
    yla_cop_type *ptr = buf;
    
    put_commd(&ptr, 0x12);
    put_value(&ptr, 0x3456);
    put_commd(&ptr, 0x78);
    put_commd(&ptr, 0x9a);

    YLATEST_ASSERT_TRUE(buf[0] == 0x12, "put_commd");
    YLATEST_ASSERT_TRUE(buf[1] == 0x34, "put_value");
    YLATEST_ASSERT_TRUE(buf[2] == 0x56, "put_value");
    YLATEST_ASSERT_TRUE(buf[3] == 0x78, "put_value");
    YLATEST_ASSERT_TRUE(buf[4] == 0x9a, "put_value");
    
    return 0;
}

static int test_init_null()
{
    yla_vm vm;
    yla_cop_type buf[100];

    YLATEST_ASSERT_FALSE(yla_vm_init(&vm, NULL, 100), "init NULL");
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm)==YLA_VM_ERROR_NO_PROGRAM_CODE, "init NULL");
    
    return 0;
}

static int test_init_0()
{
    yla_cop_type code[1];
    yla_vm vm;

    YLATEST_ASSERT_FALSE(yla_vm_init(&vm, code, 0), "init 0 size code");
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm)==YLA_VM_ERROR_NO_PROGRAM_CODE, "init 0 size code");

    return 0;
}

static int test_init_simple()
{
    yla_cop_type prg[HEADER_SIZE + 1];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 1);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 1), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == -1, "halt expected")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_init_simple2()
{
    yla_cop_type prg[HEADER_SIZE + 2];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 2);
    put_commd(&ptr, CNOP);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 2), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == 1, "OK expected")
    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == -1, "halt expected")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_init_simple_run()
{
    yla_cop_type prg[HEADER_SIZE + 2];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 2);
    put_commd(&ptr, CNOP);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 2), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_push()
{
    yla_cop_type prg[HEADER_SIZE + 4];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 1, 0, 4);
    put_commd(&ptr, CPUSH);
    put_value(&ptr, 0x1234);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 4), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_get_stack_full()
{
    yla_cop_type prg[HEADER_SIZE + 4];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 4);
    put_commd(&ptr, CPUSH);
    put_value(&ptr, 0x1234);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 4), "normal");
    YLATEST_ASSERT_FALSE(yla_vm_run(&vm), "normal")
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm) == YLA_VM_ERROR_STACK_FULL, "incorrect error code");
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_code_limit()
{
    yla_cop_type prg[HEADER_SIZE + 1];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 1);
    put_commd(&ptr, CNOP);

    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 1), "normal");
    YLATEST_ASSERT_FALSE(yla_vm_run(&vm), "normal")
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm) == YLA_VM_ERROR_CODE_SEG_EXCEED, "incorrect error code");
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_save_load()
{
	 yla_int_type stack_size = 1;
	 yla_int_type vartable_size = 261;
	 yla_int_type prg_size = (4 + 3 * 2);
    yla_cop_type prg[HEADER_SIZE + prg_size];
    yla_cop_type *ptr = prg;

    put_header(&ptr, stack_size, vartable_size, prg_size);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x9696);
	 put_commd(&ptr, CSAVE);
	 put_value(&ptr, 0x0104);
	 put_commd(&ptr, CLOAD);
	 put_value(&ptr, 0x0104);
	 put_commd(&ptr, CHALT);
    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + prg_size), "normal");
	 for (int i = 0; i < 3; i++)
	 {
    	YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == 1, "OK expected")
	 }
    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == -1, "halt expected")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_dup_instruction()
{
	 yla_int_type stack_size = 4;
	 yla_int_type vartable_size = 0;
	 yla_int_type prg_size = (6 + 4 * 2);
    yla_cop_type prg[HEADER_SIZE + prg_size];
    yla_cop_type *ptr = prg;

    put_header(&ptr, stack_size, vartable_size, prg_size);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000d);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000e);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000f);
	 put_commd(&ptr, CDUP);
	 put_value(&ptr, 0x0000);
	 put_commd(&ptr, CTEST);
	 put_commd(&ptr, CHALT);
    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + prg_size), "normal");
	 for (int i = 0; i < 4; i++)
	 {
    	YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == 1, "OK expected")
	 }
	 yla_vm_do_command(&vm);
    YLATEST_ASSERT_TRUE(vm.stack.ptr[3] == 15, "DUP 0 must save variable with offset 0")
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_stack_instructions()
{
	 yla_int_type stack_size = 5;
	 yla_int_type vartable_size = 0;
	 yla_int_type prg_size = (9 + 8 * 2);
    yla_cop_type prg[HEADER_SIZE + prg_size];
    yla_cop_type *ptr = prg;

    put_header(&ptr, stack_size, vartable_size, prg_size);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000d);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000e);
    put_commd(&ptr, CPUSH);
	 put_value(&ptr, 0x000f);
	 put_commd(&ptr, CDUP);
	 put_value(&ptr, 0x0002);
	 put_commd(&ptr, CDUP);
	 put_value(&ptr, 0x0000);
	 put_commd(&ptr, CDEEP);
	 put_value(&ptr, 0x0002);
	 put_commd(&ptr, CSTK);
	 put_value(&ptr, 0x0002);
	 put_commd(&ptr, CHALT);
    yla_vm vm;

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + prg_size), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal");
	 YLATEST_ASSERT_TRUE(vm.stack.ptr[0] == 13, "last variable must be equal to first variable after execution");
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static int test_count_word_length()
{
	yla_int_type stack_size = 10;
	yla_int_type vartable_size = 10;
	yla_int_type prg_size = 512;
	yla_cop_type prg[HEADER_SIZE + prg_size];
	yla_cop_type *prog_ptr = prg;
	
	yla_int_type prog_counter = 0;
	yla_int_type *prog_counter_ptr = prog_counter;
	
	compliance_table link_table;
	compliance_table_init(&link_table, 10);
	
	yla_int_type global_var[5];
	
	global_var[0] = 0x0000;		
	global_var[4] = 0x0004; 
	
	put_header(&prog_ptr, stack_size, vartable_size, prg_size);
	
	char word1[5 + 1];
	strcpy(word1, "Hello");
	
	yla_int_type i;
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(word1); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(word1[i]));	prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CALO);					prog_counter++;
	put_value(&prog_ptr, 0x0100);				prog_counter += 2;	
	
	put_commd(&prog_ptr, CLOAD);				prog_counter++;
	put_value(&prog_ptr, global_var[0]);		prog_counter += 2;
	
	put_commd(&prog_ptr, CHALT);				prog_counter++;
	
	put_count_word_length(&prog_ptr, 0x0100, &link_table, &prog_counter, global_var);
	
	yla_vm vm;
	
	yla_cop_type *prog_linker_ptr = prg;
	int link_state = link_program(prog_linker_ptr, prog_counter, &link_table);
	YLATEST_ASSERT_TRUE(link_state == 1, "link state OK expected");
	
	YLATEST_ASSERT_TRUE(yla_vm_init(&vm, &prg, (prg_size + HEADER_SIZE)), "vm normal init expected");
	
	if (link_state == 1) {
		YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal");
	} else if (link_state == -8) {
		vm.last_error = YLA_VM_LINKER_ERROR_TABLE_EMPTY;
	} else if (link_state == -9) {
		vm.last_error = YLA_VM_LINKER_ERROR_ADDRESS_NOT_FOUND;
	}
	YLATEST_ASSERT_TRUE(vm.last_error == YLA_VM_ERROR_OK, "vm error OK expected");
	
	YLATEST_ASSERT_TRUE(vm.vartable[global_var[0]] == 5, "expected word length == 5");
	
	YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");
	
	return 0;
}

static int test_string_concat()
{
	yla_int_type stack_size = 20;
	yla_int_type vartable_size = 10;
	yla_int_type prg_size = 512;
	yla_cop_type prg[HEADER_SIZE + prg_size];
	yla_cop_type *prog_ptr = prg;
	
	yla_int_type prog_counter = 0;
	yla_int_type *prog_counter_ptr = prog_counter;
	
	compliance_table link_table;
	compliance_table_init(&link_table, 20);
	
	yla_int_type global_var[5];
	
	global_var[0] = 0x0000;		
	global_var[1] = 0x0001;	
	global_var[2] = 0x0002;
	global_var[3] = 0x0003;		 
	global_var[4] = 0x0004;	 
	
	put_header(&prog_ptr, stack_size, vartable_size, prg_size);
	
	char word1[5 + 1];
	char word2[5 + 1];
	strcpy(word1, "Hello");
	strcpy(word2, "World");
	
	yla_int_type i;
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(word1); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(word1[i]));	prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(word2); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(word2[i]));	prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CALO);					prog_counter++;
	put_value(&prog_ptr, 0x0120);				prog_counter += 2;	
	
	put_commd(&prog_ptr, CHALT);				prog_counter++;
	
	put_count_word_length(&prog_ptr, 0x0100, &link_table, &prog_counter, global_var);
	put_string_concat(&prog_ptr, 0x0120, &link_table, &prog_counter, global_var, 0x0100);
	
	yla_vm vm;
	
	yla_cop_type *prog_linker_ptr = prg;
	int link_state = link_program(prog_linker_ptr, prog_counter, &link_table);
	YLATEST_ASSERT_TRUE(link_state == 1, "link state OK expected");
	
	YLATEST_ASSERT_TRUE(yla_vm_init(&vm, &prg, (prg_size + HEADER_SIZE)), "vm normal init expected");
	
	if (link_state == 1) {
		YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal");
	} else if (link_state == -8) {
		vm.last_error = YLA_VM_LINKER_ERROR_TABLE_EMPTY;
	} else if (link_state == -9) {
		vm.last_error = YLA_VM_LINKER_ERROR_ADDRESS_NOT_FOUND;
	}
	YLATEST_ASSERT_TRUE(vm.last_error == YLA_VM_ERROR_OK, "vm error OK expected");
	
	YLATEST_ASSERT_TRUE(vm.vartable[global_var[0]] == 10, "expected concatenated word length == 10");
	
	YLATEST_ASSERT_FALSE(vm.stack.ptr[6] == YLA_WORD_NULL_TERMINATOR, "expected NOT null terminator on the end of first word");
	YLATEST_ASSERT_TRUE(vm.stack.ptr[6] == 'W' && vm.stack.ptr[7] == 'o' && vm.stack.ptr[8] == 'r' && vm.stack.ptr[9] == 'l' && vm.stack.ptr[10] == 'd', "expected 'World' on the end of stack");
	
	YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");
	
	return 0;
}

static int test_string_repeat()
{
	yla_int_type stack_size = 20;
	yla_int_type vartable_size = 10;
	yla_int_type prg_size = 512;
	yla_cop_type prg[HEADER_SIZE + prg_size];
	yla_cop_type *prog_ptr = prg;
	
	yla_int_type prog_counter = 0;
	yla_int_type *prog_counter_ptr = prog_counter;
	
	compliance_table link_table;
	compliance_table_init(&link_table, 20);
	
	yla_int_type global_var[5];
	
	global_var[0] = 0x0000;		
	global_var[1] = 0x0001;		
	global_var[2] = 0x0002;
	global_var[3] = 0x0003; 
	global_var[4] = 0x0004; 
	
	put_header(&prog_ptr, stack_size, vartable_size, prg_size);
	
	char word1[5 + 1];
	strcpy(word1, "World");
	
	yla_int_type i;
	
	put_commd(&prog_ptr, CPUSH);						prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);		prog_counter += 2;
	
	for (i = 0; i < strlen(word1); i++) {
		put_commd(&prog_ptr, CPUSH);					prog_counter++;
		put_value(&prog_ptr, char_to_int(word1[i]));	prog_counter += 2;
	}
	
	put_commd(&prog_ptr, CPUSH);				prog_counter++;
	put_value(&prog_ptr, 0x0002);				prog_counter += 2;
	
	put_commd(&prog_ptr, CALO);					prog_counter++;
	put_value(&prog_ptr, 0x0110);				prog_counter += 2;	
	
	put_commd(&prog_ptr, CHALT);				prog_counter++;
	
	put_count_word_length(&prog_ptr, 0x0100, &link_table, &prog_counter, global_var);
	put_string_repeat(&prog_ptr, 0x0110, &link_table, &prog_counter, global_var, 0x0100);
	
	yla_vm vm;
	
	yla_cop_type *prog_linker_ptr = prg;
	int link_state = link_program(prog_linker_ptr, prog_counter, &link_table);
	YLATEST_ASSERT_TRUE(link_state == 1, "link state OK expected");
	
	YLATEST_ASSERT_TRUE(yla_vm_init(&vm, &prg, (prg_size + HEADER_SIZE)), "vm normal init expected");
	
	if (link_state == 1) {
		YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal");
	} else if (link_state == -8) {
		vm.last_error = YLA_VM_LINKER_ERROR_TABLE_EMPTY;
	} else if (link_state == -9) {
		vm.last_error = YLA_VM_LINKER_ERROR_ADDRESS_NOT_FOUND;
	}
	YLATEST_ASSERT_TRUE(vm.last_error == YLA_VM_ERROR_OK, "vm error OK expected");
	
	YLATEST_ASSERT_TRUE(vm.vartable[global_var[0]] == 15, "expected repeated word length == 15");
	
	YLATEST_ASSERT_FALSE(vm.stack.ptr[6] == YLA_WORD_NULL_TERMINATOR, "expected NOT null terminator on the end of first word");
	YLATEST_ASSERT_TRUE(vm.stack.ptr[6] == 'W' && vm.stack.ptr[7] == 'o' && vm.stack.ptr[8] == 'r' && vm.stack.ptr[9] == 'l' && vm.stack.ptr[10] == 'd', "expected 'World' on the end of stack");
	
	YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");
	
	return 0;
}

static int test_string_add_num()
{
	yla_int_type stack_size = 30;
	yla_int_type vartable_size = 10;
	yla_int_type prg_size = 1024;
	yla_cop_type prg[HEADER_SIZE + prg_size];
	yla_cop_type *prog_ptr = prg;
	
	yla_int_type prog_counter = 0;
	yla_int_type *prog_counter_ptr = prog_counter;
	
	compliance_table link_table;
	compliance_table_init(&link_table, 20);
	
	yla_int_type global_var[5];
	
	put_header(&prog_ptr, stack_size, vartable_size, prg_size);
	
	put_commd(&prog_ptr, CPUSH);					prog_counter++;
	put_value(&prog_ptr, YLA_WORD_NULL_TERMINATOR);	prog_counter += 2;
	
	put_commd(&prog_ptr, CPUSH);					prog_counter++;
	put_value(&prog_ptr, 0x00fe);					prog_counter += 2;
	put_commd(&prog_ptr, CITOA);					prog_counter++;
	
	put_commd(&prog_ptr, CHALT);					prog_counter++;
	
	yla_vm vm;
	
	yla_cop_type *prog_linker_ptr = prg;
	int link_state = link_program(prog_linker_ptr, prog_counter, &link_table);
	YLATEST_ASSERT_TRUE(link_state == 1, "link state OK expected");
	
	YLATEST_ASSERT_TRUE(yla_vm_init(&vm, &prg, (prg_size + HEADER_SIZE)), "vm normal init expected");
	
	if (link_state == 1) {
		YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal");
	} else if (link_state == -8) {
		vm.last_error = YLA_VM_LINKER_ERROR_TABLE_EMPTY;
	} else if (link_state == -9) {
		vm.last_error = YLA_VM_LINKER_ERROR_ADDRESS_NOT_FOUND;
	}
	YLATEST_ASSERT_TRUE(vm.last_error == YLA_VM_ERROR_OK, "vm error OK expected");
	
	YLATEST_ASSERT_TRUE(vm.stack.ptr[1] == '2' && vm.stack.ptr[2] == '5' && vm.stack.ptr[3] == '4', "expected '254' converted to char format");
	
	YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");
	
	return 0;
}

YLATEST_BEGIN(yla_vm_test1)
  YLATEST_ADD_TEST_CASE(test_gencode)
  YLATEST_ADD_TEST_CASE(test_init_null)
  YLATEST_ADD_TEST_CASE(test_init_0)
  YLATEST_ADD_TEST_CASE(test_init_simple)
  YLATEST_ADD_TEST_CASE(test_init_simple2)
  YLATEST_ADD_TEST_CASE(test_init_simple_run)
  YLATEST_ADD_TEST_CASE(test_push)
  YLATEST_ADD_TEST_CASE(test_get_stack_full)
  YLATEST_ADD_TEST_CASE(test_code_limit)
  YLATEST_ADD_TEST_CASE(test_save_load)
  YLATEST_ADD_TEST_CASE(test_dup_instruction)
  YLATEST_ADD_TEST_CASE(test_stack_instructions)
  YLATEST_ADD_TEST_CASE(test_count_word_length)
  YLATEST_ADD_TEST_CASE(test_string_concat)
  YLATEST_ADD_TEST_CASE(test_string_repeat)
  YLATEST_ADD_TEST_CASE(test_string_add_num)
YLATEST_END
