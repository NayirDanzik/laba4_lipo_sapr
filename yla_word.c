#include "yla_word.h"

int char_to_int(char letter) 
{
	char hex_letter[2 + 1];
	sprintf(hex_letter, "%d", letter);
	return atoi(hex_letter);
}

void put_string_repeat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr)
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type repeat = global_var[1];
	yla_int_type i = global_var[2];
	yla_int_type ret = global_var[3];
	
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0111, prog_count);
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0119);					prog_count += 2;	
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0112, prog_count);
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	put_commd(prog_ptr, CJNZ);						prog_count++;
	put_value(prog_ptr, 0x0113);					prog_count += 2;	
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0112);					prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0113, prog_count);
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0111);					prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0119, prog_count);
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	put_commd(prog_ptr, CRET);						prog_count++;
	
	*prog_counter = prog_count;
}

void put_string_concat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr)
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type ret = global_var[3];
	
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0121, prog_count);	
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0122);					prog_count += 2;	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CGDEEP);					prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0121);					prog_count += 2;	
	
	compliance_table_set_addr(compliance, 0x0122, prog_count);	
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0129, prog_count);	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	put_commd(prog_ptr, CRET);						prog_count++;
	
	*prog_counter = prog_count;
}

void put_count_word_length(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[])
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type ret = global_var[4];
	
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	put_commd(prog_ptr, CDUP);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, YLA_WORD_NULL_TERMINATOR);	prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	
	put_commd(prog_ptr, CJNZ);						prog_count++;
	put_value(prog_ptr, 0x0109);					prog_count += 2;	
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	compliance_table_set_addr(compliance, 0x0101, prog_count);
	
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, YLA_WORD_NULL_TERMINATOR);	prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0102);					prog_count += 2;	
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0109);					prog_count += 2;	
	
	compliance_table_set_addr(compliance, 0x0102, prog_count);
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0101);					prog_count += 2;	
	
	compliance_table_set_addr(compliance, 0x0109, prog_count);
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;	
	
	put_commd(prog_ptr, CLOAD);						prog_count++;	
	put_value(prog_ptr, ret);						prog_count += 2;	
	put_commd(prog_ptr, CRET);						prog_count++;	
	
	*prog_counter = prog_count;
}