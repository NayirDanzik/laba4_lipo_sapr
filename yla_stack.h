/*
    Int stack header file

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

#ifndef _yla_stack_h
#define _yla_stack_h

#include <stddef.h>

#include "yla_type.h"

typedef struct {
    int *ptr;
    size_t size;
    size_t count;
} yla_stack;

void yla_stack_init(yla_stack* stack, size_t size);
void yla_stack_done(yla_stack* stack);

int yla_stack_push(yla_stack* stack, yla_int_type value);
int yla_stack_pull(yla_stack* stack, yla_int_type *result);

int yla_stack_set_deep(yla_stack* stack, size_t index, yla_int_type value);
int yla_stack_get_deep(yla_stack* stack, size_t index, yla_int_type *result);

int yla_stack_top(yla_stack* stack, yla_int_type *result);

int yla_stack_tail_delete(yla_stack* stack, yla_int_type tail_size);

int yla_stack_is_empty(yla_stack* stack);
int yla_stack_is_full(yla_stack* stack);

#endif
