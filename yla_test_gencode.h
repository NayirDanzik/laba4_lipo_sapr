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

#ifndef yla_test_gencode_h
#define yla_test_gencode_h

#include "yla_type.h"
#include "yla_compliance.h"
#include <stddef.h>

void put_commd(yla_cop_type** prg, yla_cop_type value);

void put_value(yla_cop_type** prg, yla_int_type value);

void put_header(yla_cop_type** prg, size_t stack_size, size_t vartable_size, size_t code_size);

int link_program(yla_cop_type *prog_ptr, yla_int_type prog_counter, compliance_table *link_table);

#endif
