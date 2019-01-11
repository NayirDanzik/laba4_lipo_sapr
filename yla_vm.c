/*
    Virtual Machine implementation

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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "yla_cop.h"
#include "yla_vm.h"
#include "yla_type.h"

int yla_vm_get_value(yla_vm *vm, yla_int_type *value);

int yla_vm_set_var(yla_vm *vm, size_t index, yla_int_type value);
int yla_vm_get_var(yla_vm *vm, size_t index, yla_int_type *value);

yla_int_type yla_vm_get_value_internal(yla_cop_type *start);

int yla_vm_read_header(yla_vm *vm, yla_cop_type *program, size_t program_size);
int yla_vm_check_magic(yla_cop_type** program);
int yla_vm_read_sizes(yla_vm *vm, yla_cop_type **program);

int yla_vm_do_command_internal(yla_vm *vm, yla_cop_type cop);

char *yla_vm_error_message(int error_code);

/*
Public functions
*/

int yla_vm_init(yla_vm *vm, yla_cop_type *program, size_t program_size)
{
	if (!vm) {
		return 0;
	}

	if (!yla_vm_read_header(vm, program, program_size)) {
		vm->last_error = YLA_VM_ERROR_NO_PROGRAM_CODE;
		return 0;
	}
	
	yla_stack_init(&vm->stack, vm->stack_size);
	vm->vartable = calloc(vm->vartable_size, sizeof(yla_int_type));
	vm->code = malloc(vm->code_size);
	vm->pc = 0;
	if (program_size < HEADER_SIZE + vm->code_size) {
		vm->last_error = YLA_VM_ERROR_NO_PROGRAM_CODE;
		return 0;
	}
	
	memcpy(vm->code, program + HEADER_SIZE, vm->code_size);
	
	vm->last_error = YLA_VM_ERROR_OK;
	
	return 1;
}

int yla_vm_done(yla_vm *vm)
{
	if (!vm) {
		return 0;
	}

	if (vm->code) {
		free(vm->code);
	}

	if (vm->vartable) {
		free(vm->vartable);
	}

	yla_stack_done(&vm->stack);
	
	return 1;
}

int yla_vm_run(yla_vm *vm)
{
	int cmd_result;

	if (!vm) {
		return 0;
	}

	if (!vm->code) {
		return 0;
	}

	while(1) {
		if (vm->pc + 1 > vm->code_size) {
			vm->last_error = YLA_VM_ERROR_CODE_SEG_EXCEED;
			return 0;
		}
		yla_cop_type cop = vm->code[vm->pc++];

		cmd_result = yla_vm_do_command_internal(vm, cop);
		
		if (cmd_result == -1) {
			return 1;
		}
		if (cmd_result == 0) {
			return 0;
		}
	}
}

int yla_vm_do_command(yla_vm *vm)
{
	if (!vm) {
		return 0;
	}

	if (!vm->code) {
		return 0;
	}

	if (vm->pc + 1 > vm->code_size) {
		vm->last_error = YLA_VM_ERROR_CODE_SEG_EXCEED;
		return 0;
	}
	yla_cop_type cop = vm->code[vm->pc++];

	return yla_vm_do_command_internal(vm, cop);
}

int yla_vm_last_error(yla_vm *vm)
{
	if (!vm) {
		return 0;
	}
	return vm->last_error;
}

int yla_vm_error_text(yla_vm *vm, int error_code, char *buf, int buf_len)
{
	char *error_message;
	int message_len;

	if (!vm) {
		return 0;
	}

	error_message = yla_vm_error_message(error_code);
	message_len = strlen(error_message) + 1;
	if (buf == NULL || buf_len <= message_len) {
		return message_len;
	}

	strcpy(buf, error_message);

	return 0;
}

/*
Private functions
*/

/*
Get values
*/
int yla_vm_get_value(yla_vm *vm, yla_int_type *value)
{
	if (vm->pc + sizeof(yla_int_type) > vm->code_size) {
		vm->last_error = YLA_VM_ERROR_CODE_SEG_EXCEED;
		return 0;
	}
	
	yla_cop_type prog_var[2];
	yla_cop_type *prog_var_ptr = prog_var;
	
	prog_var[0] = vm->code[vm->pc];
	prog_var[1] = vm->code[vm->pc + 1];
	
	*value = yla_vm_get_value_internal(prog_var_ptr);
	
	vm->pc += sizeof(yla_int_type);

	return 1;
}

yla_int_type yla_vm_get_value_internal(yla_cop_type *start)
{
	unsigned int full_value = 0;
	size_t i=0;
	for (i=0; i<sizeof(yla_int_type); ++i) {
		full_value <<= 8;
		unsigned char byte = *start++;
		full_value |= byte;
	}
	return full_value;
}

/*
Reading program
*/
int yla_vm_read_header(yla_vm *vm, yla_cop_type *program, size_t program_size)
{
	if (program == NULL) {
		return 0;
	}
	if (program_size < HEADER_SIZE) {
		return 0;
	}
	if (!yla_vm_check_magic(&program)) {
		return 0;
	}
	if (!yla_vm_read_sizes(vm, &program)) {
		return 0;
	}
	
	return 1;
}

int yla_vm_check_magic(yla_cop_type** program)
{
	yla_int_type magic_value;
	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE1) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE2) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE3) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	return 1;
}

int yla_vm_read_sizes(yla_vm *vm, yla_cop_type **program)
{
	vm->stack_size = (size_t)yla_vm_get_value_internal(*program);
	if (vm->stack_size > MAX_STACK_SIZE) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	vm->vartable_size = (size_t)yla_vm_get_value_internal(*program);
	if (vm->vartable_size > MAX_VARTABLE_SIZE) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	vm->code_size = (size_t)yla_vm_get_value_internal(*program);
	if (vm->code_size > MAX_CODE_SIZE) {
		return 0;
	}
	*program += sizeof(yla_int_type);

	return 1;
}

/*
Vartable
*/
int yla_vm_get_var(yla_vm *vm, size_t index, yla_int_type *value)
{
	if (!vm) {
		return 0;
	}
	if (index >= vm->vartable_size) {
		return 0;
	}

	*value = vm->vartable[index];

	return 1;
}

int yla_vm_set_var(yla_vm *vm, size_t index, yla_int_type value)
{
	if (!vm) {
		return 0;
	}
	if (index >= vm->vartable_size) {
		return 0;
	}

	vm->vartable[index] = value;
}

/*
Stack
*/

int yla_vm_stack_pull(yla_vm *vm, yla_int_type *value)
{
	if (!yla_stack_pull(&vm->stack, value)) {
		vm->last_error = YLA_VM_ERROR_STACK_EMPTY;
		return 0;
	}
	return 1;
}

int yla_vm_stack_push(yla_vm *vm, yla_int_type value)
{
	if (!yla_stack_push(&vm->stack, value)) {
		vm->last_error = YLA_VM_ERROR_STACK_FULL;
		return 0;
	}
	return 1;
}

int yla_vm_stack_get_deep(yla_vm *vm, yla_int_type index, yla_int_type *value)
{
	yla_int_type state = yla_stack_get_deep(&vm->stack, index, value);
	if (state == -10) {
		vm->last_error = YLA_VM_ERROR_STACK_EMPTY;
		return 0;
	}
	if (state == -20) {
		vm->last_error = YLA_VM_ERROR_STACK_FULL;
		return 0;
	}
	return 1;
}

int yla_vm_stack_set_deep(yla_vm *vm, yla_int_type index, yla_int_type value)
{
	yla_int_type state = yla_stack_set_deep(&vm->stack, index, value);
	if (state == -10) {
		vm->last_error = YLA_VM_ERROR_STACK_EMPTY;
		return 0;
	}
	if (state == -20) {
		vm->last_error = YLA_VM_ERROR_STACK_FULL;
		return 0;
	}
	return 1;
}

int yla_vm_stack_tail_delete(yla_vm *vm, yla_int_type tail_size)
{
	if (!yla_stack_tail_delete(&vm->stack, tail_size)) {
		vm->last_error = YLA_VM_ERROR_STACK_TAIL_TOO_LONG;
		return 0;	
	}
	return 1;
}

/*
Perform command by code of operation
*/
int yla_vm_do_command_internal(yla_vm *vm, yla_cop_type cop)
{
	yla_int_type op1 = 0;
	yla_int_type op2 = 0;
	yla_int_type res = 0;
	char opchar[5];

	switch(cop) {

		case CNOP:	
			break;
		
		case CLOAD:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			op2 = vm->vartable[op1];
			if (!yla_vm_stack_push(vm, op2)) {
				return 0;
			}
			break;
		
		case CSAVE:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			vm->vartable[op1] = op2;
			break;

		case CPUSH:
			if (!yla_vm_get_value(vm, &res)) {
				return 0;
			}
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
		
		case CDUP:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, op1, &op2)) {
				return 0;
			}
			if (!yla_vm_stack_push(vm, op2)) {
				return 0;
			}
			break;
		
		case CGDUP:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			op2 = vm->vartable[op1];
			if (!yla_vm_stack_get_deep(vm, op2, &res)) {
				return 0;
			}
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
		
		case CDEEP:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			if (!yla_vm_stack_set_deep(vm, op1, op2)) {
				return 0;
			}
			break;
			
		case CGDEEP:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			op2 = vm->vartable[op1];
			if (!yla_vm_stack_pull(vm, &res)) {
				return 0;
			}
			if (!yla_vm_stack_set_deep(vm, op2, res)) {
				return 0;
			}
			break;
			
		case CSTK:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_tail_delete(vm, op1)) {
				return 0;
			}
			break;
		
		case CGSTK:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			op2 = vm->vartable[op1];
			if (!yla_vm_stack_tail_delete(vm, op2)) {
				return 0;
			}
			break;
		
		case CTEST:
			if (!yla_vm_stack_get_deep(vm, 0, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 1, &op2)) {
				return 0;
			}
			if (op1 == op2) {
				res = 1;
			} else {
				res = 0;
			}
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
		
		case CCMP:
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			if (op1 - op2 == 0) {
				res = 1;
			} else {
				res = 0;
			}
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
			
		case CITOA:
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			
			sprintf(opchar, "%ld", op1);
			
			for (int i = 0; i < 5; i++) {
				if ((int)opchar[i] == '\0'){
					continue;
				}
				if (!yla_vm_stack_push(vm, (int)opchar[i])) {
					return 0;
				}
			}
			
			break;
		
		case CALO:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_push(vm, vm->pc))	{
				return 0;
			}
			vm->pc = op1 - 1;
			break;
			
		case CRET:
			if (!yla_vm_stack_pull(vm, &op1))	{
				return 0;
			}
			vm->pc = op1;
			break;
			
		case CJMP:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			vm->pc = op1 - 1;
			break;
		
		case CJZ:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 0, &op2)) {
				return 0;
			}
			if (op2 == 0) {
				vm->pc = op1 - 1;
			}
			break;
			
		case CJNZ:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 0, &op2)) {
				return 0;
			}
			if (op2 != 0) {
				vm->pc = op1 - 1;
			}
			break;
			
		case CJE:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 0, &op2)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 1, &res)) {
				return 0;
			}
			if (op2 == res) {
				vm->pc = op1 - 1;
			}
			break;
			
		case CJNE:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 0, &op2)) {
				return 0;
			}
			if (!yla_vm_stack_get_deep(vm, 1, &res)) {
				return 0;
			}
			if (op2 != res) {
				vm->pc = op1 - 1;
			}
			break;

		case CADD:
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			res = op2 + op1;
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;

		case CSUB: 
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			res = op2 - op1;
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;

		case CMULT:
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			res = op2 * op1;
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;

		case CDIV: 
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &op2)) {
				return 0;
			}
			if (op1==0) {
				vm->last_error = YLA_VM_ERROR_DIV_BY_ZERO;
				return 0;
			}
			res = op2 / op1;
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
		
		case CNEG:
			if (!yla_vm_stack_pull(vm, &op1)) {
				return 0;
			}
			res = op1 * (-1);
			if (!yla_vm_stack_push(vm, res)) {
				return 0;
			}
			break;
		
		case COUT:
			if (!yla_vm_get_value(vm, &op1)) {
				return 0;
			}
			if (!yla_vm_stack_pull(vm, &res)) {
				return 0;
			}
			if (op1 == 0x0001) {
				if (res >= 0 && res <= 9) {
					fprintf(stdout, "%d \n", res);
				} else {
					fprintf(stdout, "%c \n", res);
				}
			} else if (op1 == 0x0000) {
				if (res >= 0 && res <= 9) {
					fprintf(stdin, "%d \n", res);
				} else {
					fprintf(stdin, "%d \n", res);
				}
			}
			break;
			
		case CHALT:
			return -1;

		default:
			vm->last_error = YLA_VM_ERROR_UNKNOWN_COMMAND;
			return 0;
	}
	return 1;
}

/*
Error messages
*/
char *yla_vm_error_message(int error_code)
{
	switch(error_code) {
		case YLA_VM_ERROR_OK:
			return "No error";
		case YLA_VM_ERROR_NO_PROGRAM_CODE:
			return "No program code";
		case YLA_VM_ERROR_CODE_SEG_EXCEED:
			return "Code segment exceed";
		case YLA_VM_ERROR_DIV_BY_ZERO:
			return "Divide by zero";
		case YLA_VM_ERROR_UNKNOWN_COMMAND:
			return "Unknown command";
		case YLA_VM_ERROR_STACK_EMPTY:
			return "Empty stack";
		case YLA_VM_ERROR_STACK_FULL:
			return "Stack full_value";
		default:
			return "Unknown error";
	}
}
