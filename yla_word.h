/*
    String functions for virtual machine
*/
#ifndef yla_word_h
#define yla_word_h

#include "yla_vm.h"
#include "yla_cop.h"
#include "yla_type.h"
#include "yla_test.h"
#include "yla_compliance.h"

int char_to_int(char letter);

void put_string_concat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr);
void put_string_repeat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr);
void put_count_word_length(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[]);

#endif