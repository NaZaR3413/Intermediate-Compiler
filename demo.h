#pragma once

struct InstructionNode* parse_generate_intermediate_representation();
void parse_inputs();
struct InstructionNode* parse_body();
struct InstructionNode* parse_stmt_list();
struct InstructionNode* parse_stmt();
struct InstructionNode* parse_while();
struct InstructionNode* parse_if();
struct InstructionNode* parse_assign();
struct InstructionNode* parse_IN();
void parse_id_list();
void hard_parse();
struct InstructionNode* test_assignment_basic1();