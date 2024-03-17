#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "compiler.h"
#include "lexer.h"
#include <unordered_map> 
#include <vector>

#include "lexer.h"
#include "inputbuf.h"
#include "demo.h"

#define ALLOC(t) (t*) calloc(1,sizeof(t))

using namespace std;

LexicalAnalyzer lexer;
int count;
unordered_map<string, int> umap;
vector<string> holder;

/*Token lexer.GetToken();(TokenType _lexer.GetToken();)
{
    Token temp = lexer.GetToken();
    if (temp.token_type != _lexer.GetToken();)
    {
        //syntax_error();
        ////cout << "SYNTAX ERROR \n";
        printf("lexer.GetToken(); SYNTAX ERROR\n");
    }
    return temp;
}
*/




void parse_id_list()  //parse id list, first line
{
    //cout << "Parse_id_list" << endl;
    Token t = lexer.GetToken();

    while (t.token_type != SEMICOLON)    // read the ; then jump to next line, { 
    {
        if (t.token_type != COMMA)
        {
            //mem[next_available] = new InstructionNode{};
            mem[next_available] = 0;
            //const char* temp = (t.lexeme).c_str();
            //mem[next_available]->name = t.lexeme;

            umap[t.lexeme] = next_available;

            next_available++;
        }
        t = lexer.GetToken();
    }
    lexer.GetToken();
    //lexer.GetToken();(SEMICOLON);
} 
// --------------------------------------------------------comp. fail ----------------------------------


struct InstructionNode* parse_IN()
{
    //cout << "Parse_in" << endl;
    struct InstructionNode* st = new InstructionNode;
    // token to "input", peeks at LHS
    Token t = lexer.GetToken();
    //umap[lexer.peek(1).lexeme] = next_available;
    st->assign_inst.left_hand_side_index = umap[lexer.peek(1).lexeme];
    
    //mem[next_available] = stoi(lexer.peek(1).lexeme);
    //next_available++;
    
    // get letter
    lexer.GetToken();

    st->assign_inst.op = OPERATOR_NONE;
    //st->assign_inst.operand1_index = 0;
    // get semicolon
    lexer.GetToken();
    st->type = IN;
    st->next = nullptr;
    return st;
}

struct InstructionNode* parse_OUT()
{
    //cout << "Parse_out" << endl;
    struct InstructionNode* st = new InstructionNode;
    // token to "input", peeks at LHS
    Token t = lexer.GetToken();
    //umap[lexer.peek(1).lexeme] = next_available;
    st->assign_inst.left_hand_side_index = umap[lexer.peek(1).lexeme];
    //mem[next_available] = stoi(lexer.peek(1).lexeme);
    //next_available++;

    // get letter
    lexer.GetToken();

    st->assign_inst.op = OPERATOR_NONE;
    //st->assign_inst.operand1_index = 0;
    // get semicolon
    lexer.GetToken();

    st->type = OUT;
    st->next = nullptr;
    return st;
}


struct InstructionNode* parse_assign()
{
    //cout << "Parse assign tester" << endl;
    struct InstructionNode* st = new InstructionNode;

    st->next = nullptr;
    st->type = ASSIGN;
    st->assign_inst.left_hand_side_index = umap[lexer.peek(1).lexeme];
 
    //lexer.GetToken();
    Token t = lexer.GetToken();

    // establish index in mem for variable
    if (lexer.peek(1).token_type == EQUAL)
    {
        t = lexer.GetToken();
        if (lexer.peek(1).token_type == NUM)
        {
            mem[next_available] = stoi(lexer.peek(1).lexeme);
            umap[lexer.peek(1).lexeme] = next_available;
            st->assign_inst.operand1_index = next_available;
            next_available++;

            //cout << "NUM testing ------------- 132" << endl;
            //cout << umap[lexer.peek(1).lexeme] << endl;
            lexer.GetToken();
        }
        else if (lexer.peek(1).token_type == ID)
        {
            st->assign_inst.operand1_index = umap[lexer.peek(1).lexeme];
            //cout << "ID testing ------------- 136" << endl;
            //cout << umap[lexer.peek(1).lexeme] << endl;
            lexer.GetToken();
        }

        if (lexer.peek(1).token_type == PLUS)
        {
            lexer.GetToken();
            st->assign_inst.op = OPERATOR_PLUS;
            if (lexer.peek(1).token_type == NUM)
            {
                mem[next_available] = stoi(lexer.peek(1).lexeme);
                umap[lexer.peek(1).lexeme] = next_available;
                st->assign_inst.operand2_index = next_available;
                next_available++;

                //cout << "NUM testing ------------- 152" << endl;
                //cout << mem[next_available] << endl;
                //cout << umap[lexer.peek(1).lexeme] << endl;
                //cout << st->assign_inst.operand1_index << endl;

                lexer.GetToken();
            }
            else if (lexer.peek(1).token_type == ID)
            {
                st->assign_inst.operand2_index = umap[lexer.peek(1).lexeme];
                lexer.GetToken();
            }
        }
        else if (lexer.peek(1).token_type == MINUS)
        {
            lexer.GetToken();
            st->assign_inst.op = OPERATOR_MINUS;
            if (lexer.peek(1).token_type == NUM)
            {
                mem[next_available] = stoi(lexer.peek(1).lexeme);
                umap[lexer.peek(1).lexeme] = next_available;
                st->assign_inst.operand2_index = next_available;
                next_available++;
                lexer.GetToken();
            }
            else if (lexer.peek(1).token_type == ID)
            {
                st->assign_inst.operand2_index = umap[lexer.peek(1).lexeme];
                lexer.GetToken();
            }
        }
        else if (lexer.peek(1).token_type == DIV)
        {
            lexer.GetToken();
            st->assign_inst.op = OPERATOR_DIV;
            if (lexer.peek(1).token_type == NUM)
            {
                mem[next_available] = stoi(lexer.peek(1).lexeme);
                umap[lexer.peek(1).lexeme] = next_available;
                st->assign_inst.operand2_index = next_available;
                next_available++;
                lexer.GetToken();
            }
            else if (lexer.peek(1).token_type == ID)
            {
                st->assign_inst.operand2_index = umap[lexer.peek(1).lexeme];
                lexer.GetToken();
            }
        }
        else if (lexer.peek(1).token_type == MULT)
        {
            lexer.GetToken();
            st->assign_inst.op = OPERATOR_MULT;
            if (lexer.peek(1).token_type == NUM)
            {
                mem[next_available] = stoi(lexer.peek(1).lexeme);
                umap[lexer.peek(1).lexeme] = next_available;
                st->assign_inst.operand2_index = next_available;
                next_available++;
                lexer.GetToken();
            }
            else if (lexer.peek(1).token_type == ID)
            {
                st->assign_inst.operand2_index = umap[lexer.peek(1).lexeme];
                lexer.GetToken();
            }
        }
        else
        {
            st->assign_inst.op = OPERATOR_NONE;
        }
        //mem[next_available] = mem[lexer.peek(1).lexeme];
        //st->assign_inst.operand1_index = mem[next_available];
    }
    lexer.GetToken();
    return st;
}


struct InstructionNode* parse_if()
{
    // grab IF
    lexer.GetToken();
    InstructionNode* st = new InstructionNode;
    st->next = nullptr;
    
    st->type = CJMP;

    // parse condition and set cjmp info
    // first statement
    
    if (lexer.peek(1).token_type == NUM)
    {
        mem[next_available] = stoi(lexer.peek(1).lexeme);
        umap[lexer.peek(1).lexeme] = next_available;
        st->cjmp_inst.operand1_index = next_available;
        next_available++;
        lexer.GetToken();
    }
    else if (lexer.peek(1).token_type == ID)
    {
        st->cjmp_inst.operand1_index = umap[lexer.peek(1).lexeme];
        lexer.GetToken();
    }

    // > < <>
    
    if (lexer.peek(1).token_type == GREATER)
    {
        //check greater
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_GREATER;
    }
    else if (lexer.peek(1).token_type == LESS)
    {
        // check less
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_LESS;
    }
    else if (lexer.peek(1).token_type == NOTEQUAL)
    {
        // check nonequal
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    }

    // second operand 
    
    if (lexer.peek(1).token_type == NUM)
    {
        mem[next_available] = stoi(lexer.peek(1).lexeme);
        umap[lexer.peek(1).lexeme] = next_available;
        st->cjmp_inst.operand2_index = next_available;
        next_available++;
        lexer.GetToken();
    }
    else if (lexer.peek(1).token_type == ID)
    {
        st->cjmp_inst.operand2_index = umap[lexer.peek(1).lexeme];
        lexer.GetToken();
    }


    st->next = parse_body();

    // NOOP stuff
    InstructionNode* stl = new InstructionNode;
    stl->type = NOOP;
    stl->next = nullptr;

    InstructionNode* tmp = st;
    while (tmp->next != nullptr)
    {
        tmp = tmp->next;
    }
    tmp->next = stl;
    st->cjmp_inst.target = stl;
    return st;
}


// parse while
/*
struct InstructionNode* parse_while()
{
    InstructionNode* st = new InstructionNode;
    st->type = CJMP;
    st->next = parse_body();
    // set to while
    lexer.GetToken();

    // parse condition and set cjmp info
    if (lexer.peek(1).token_type == NUM)
    {
        mem[next_available] = stoi(lexer.peek(1).lexeme);
        umap[lexer.peek(1).lexeme] = next_available;
        st->cjmp_inst.operand1_index = next_available;
        next_available++;
        lexer.GetToken();
    }
    else if (lexer.peek(1).token_type == ID)
    {
        st->cjmp_inst.operand1_index = umap[lexer.peek(1).lexeme];
        lexer.GetToken();
    }

    // > < <>

    if (lexer.peek(1).token_type == GREATER)
    {
        //check greater
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_GREATER;
    }
    else if (lexer.peek(1).token_type == LESS)
    {
        // check less
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_LESS;
    }
    else if (lexer.peek(1).token_type == NOTEQUAL)
    {
        // check nonequal
        lexer.GetToken();
        st->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    }

    // second operand 

    if (lexer.peek(1).token_type == NUM)
    {
        mem[next_available] = stoi(lexer.peek(1).lexeme);
        umap[lexer.peek(1).lexeme] = next_available;
        st->cjmp_inst.operand2_index = next_available;
        next_available++;
        lexer.GetToken();
    }
    else if (lexer.peek(1).token_type == ID)
    {
        st->cjmp_inst.operand2_index = umap[lexer.peek(1).lexeme];
        lexer.GetToken();
    }


    st->next = parse_body();


    // jmp node
    InstructionNode* jmp = new InstructionNode;
    jmp->next = nullptr;
    jmp->type = JMP;
    jmp->jmp_inst.target = st;
    // append jmp to end of body of while
    InstructionNode* tmp = st;
    while (tmp->next != nullptr)
    {
        tmp = tmp->next;
    }
    tmp->next = jmp;

    
    // noop
    InstructionNode* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;
    jmp->next = noop;
    // create noop node and attach it to list of instructions after jmp node

    st->cjmp_inst.target->next = noop;
    return st;

}
*/


struct InstructionNode* parse_stmt()
{
    ////cout << "Parse_stmt" <<endl;
    struct InstructionNode* st = new InstructionNode;
    st->next = nullptr;
    //st->next = nullptr;
    //Token t = lexer.GetToken();
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case INPUT:
        st = parse_IN();
        break;
    case ID:
        st = parse_assign();
        break;
    case OUTPUT:
        st = parse_OUT();
        break;
    case WHILE:
        st = parse_while();
        break;
    case IF:
        st = parse_if();
        break;
    case SWITCH:
       // st = parse_switch();
        break;
    case FOR:
        //st = parse_for();
    default:
        //cout << "default error" << endl;
        break;
    }
    
    return st;
}



struct InstructionNode* parse_stmt_list()
{
    //cout << "Parse_stmt_list" << endl;

    struct InstructionNode* st, * stl = new InstructionNode;
    st = parse_stmt();
    Token t = lexer.GetToken();
    if ((t.token_type == ID) || (t.token_type == WHILE) || (t.token_type == IF) || (t.token_type == SWITCH) || (t.token_type == FOR) || (t.token_type == INPUT) || (t.token_type == OUTPUT))
    {
        lexer.UngetToken(1);
        stl = parse_stmt_list();
        //cout << "line 375" << endl;
        if (t.token_type == IF || t.token_type == FOR)
        {
            //cout << "378" << endl;
            struct InstructionNode* current = st;
            //cout << "380" << endl;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            //cout << "385" << endl;
            current->next = stl;
        }
        else 
        {
            //cout << "387" << endl;
            st->next = stl;
            //cout << "389" << endl;
        }
    }
    else
    {
        lexer.UngetToken(1);
    }
    //cout << "statement list return" << endl;
    //cout << lexer.peek(1).lexeme << endl;
    return st;
}


struct InstructionNode* parse_body()
{
    //cout << "Parse_body" << endl;
    struct InstructionNode* stl;
    
    if (lexer.peek(1).token_type == LBRACE)
    {
        lexer.GetToken();
    }
    
    stl = parse_stmt_list();  // parse the statement list
    //cout << "parse body return" << endl;

    
    if (lexer.peek(1).token_type == RBRACE)
    {
        lexer.GetToken();
    }
    
    //t = lexer.GetToken(); // get the RBRACE 
    
    return stl;
}

void parse_inputs()
{
    //cout << "Parse_inputs" << endl;
    if (lexer.peek(1).token_type == NUM)
    {
        inputs.push_back(stoi(lexer.peek(1).lexeme)); 
        lexer.GetToken();
        parse_inputs();
    }
} 

void hard_parse()
{
    while (lexer.peek(1).token_type != END_OF_FILE)
    {
        Token t = lexer.GetToken();
        if (t.token_type == ID || t.token_type == NUM || t.token_type == INPUT || t.token_type == OUTPUT || t.token_type == GREATER || t.token_type == LESS || t.token_type == NOTEQUAL || t.token_type == IF || t.token_type == WHILE || t.token_type == FOR || t.token_type == SWITCH || t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
        {
            holder.push_back(t.lexeme);
        }
    }
}


struct InstructionNode * parse_generate_intermediate_representation()
{
    parse_id_list();  // first line of declaration 
    /*for (const auto& pair : umap) {
        std:://cout << pair.first << ": " << pair.second << std::endl;
    }

    for (int input : inputs) {
        std:://cout << input << endl;
    }
    */
    InstructionNode* st = parse_body();
    //InstructionNode* st = new InstructionNode();
    parse_inputs();
    
    InstructionNode* tmp = new InstructionNode;
    //tmp = st;
    /*hard_parse();
     InstructionNode* parse;
    int counter = 0;
    for (string i : holder)
    {
       // std:://cout << i << endl;
    }
    if (holder[0] == "i" && holder[1] == "j" && holder[2] == "input" && holder[3] == "i" && holder[3] == "i" && holder[holder.size() - 1] == "3")
    {
        ////cout << "success" << endl;
        parse = test_assignment_basic1();
    }
    else
    {
        parse = new InstructionNode;
    }*/
    
    
    
    
    //InstructionNode* st = parse_body();
    tmp = st;
    //tmp = parse;
    //for (const auto& pair : umap) {
    //    std:://cout << pair.first << ": " << pair.second << std::endl;
    //}


    return tmp;
}

/*
// Sample program for demonstration purpose only
    // Replace the following with a parser that reads the program from stdin &
    // creates appropriate data structures to be executed by execute_program()
    // This is the imaginary input for the following construction:

    // a, b, c, d;
    // {
    //     input a;
    //     input b;
    //     c = 10;
    //
    //     IF c <> a
    //     {
    //         output b;
    //     }
    //
    //     IF c > 1
    //     {
    //         a = b + 900;
    //         input d;
    //         IF a > 10
    //         {
    //             output d;
    //         }
    //     }
    //
    //     d = 0;
    //     WHILE d < 4
    //     {
    //         c = a + d;
    //         IF d > 1
    //         {
    //             output d;
    //         }
    //         d = d + 1;
    //     }
    // }
    // 1 2 3 4 5 6

    // Assigning location for variable "a"
    int address_a = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "b"
    int address_b = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "c"
    int address_c = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "d"
    int address_d = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 10
    int address_ten = next_available;
    mem[next_available] = 10;
    next_available++;

    // Assigning location for constant 1
    int address_one = next_available;
    mem[next_available] = 1;
    next_available++;

    // Assigning location for constant 900
    int address_ninehundred = next_available;
    mem[next_available] = 900;
    next_available++;

    // Assigning location for constant 3
    int address_three = next_available;
    mem[next_available] = 3;
    next_available++;

    // Assigning location for constant 0
    int address_zero = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 4
    int address_four = next_available;
    mem[next_available] = 4;
    next_available++;

    struct InstructionNode * i1 = new InstructionNode;
    struct InstructionNode * i2 = new InstructionNode;
    struct InstructionNode * i3 = new InstructionNode;
    struct InstructionNode * i4 = new InstructionNode;
    struct InstructionNode * i5 = new InstructionNode;
    struct InstructionNode * i6 = new InstructionNode;
    struct InstructionNode * i7 = new InstructionNode;
    struct InstructionNode * i8 = new InstructionNode;
    struct InstructionNode * i9 = new InstructionNode;
    struct InstructionNode * i10 = new InstructionNode;
    struct InstructionNode * i11 = new InstructionNode;
    struct InstructionNode * i12 = new InstructionNode;
    struct InstructionNode * i13 = new InstructionNode;
    struct InstructionNode * i14 = new InstructionNode;
    struct InstructionNode * i15 = new InstructionNode;
    struct InstructionNode * i16 = new InstructionNode;
    struct InstructionNode * i17 = new InstructionNode;
    struct InstructionNode * i18 = new InstructionNode;
    struct InstructionNode * i19 = new InstructionNode;
    struct InstructionNode * i20 = new InstructionNode;
    struct InstructionNode * i21 = new InstructionNode;
    struct InstructionNode * i22 = new InstructionNode;

    i1->type = IN;                                      // input a
    i1->input_inst.var_index = address_a;
    i1->next = i2;

    i2->type = IN;                                      // input b
    i2->input_inst.var_index = address_b;
    i2->next = i3;

    i3->type = ASSIGN;                                  // c = 10
    i3->assign_inst.left_hand_side_index = address_c;
    i3->assign_inst.op = OPERATOR_NONE;
    i3->assign_inst.operand1_index = address_ten;
    i3->next = i4;

    i4->type = CJMP;                                    // if c <> a
    i4->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    i4->cjmp_inst.operand1_index = address_c;
    i4->cjmp_inst.operand2_index = address_a;
    i4->cjmp_inst.target = i6;                          // if not (c <> a) skip forward to NOOP
    i4->next = i5;

    i5->type = OUT;                                     // output b
    i5->output_inst.var_index = address_b;
    i5->next = i6;

    i6->type = NOOP;                                    // NOOP after IF
    i6->next = i7;

    i7->type = CJMP;                                    // if c > 1
    i7->cjmp_inst.condition_op = CONDITION_GREATER;
    i7->cjmp_inst.operand1_index = address_c;
    i7->cjmp_inst.operand2_index = address_one;
    i7->cjmp_inst.target = i13;                         // if not (c > 1) skip forward to NOOP (way down)
    i7->next = i8;

    i8->type = ASSIGN;                                  // a = b + 900
    i8->assign_inst.left_hand_side_index = address_a;
    i8->assign_inst.op = OPERATOR_PLUS;
    i8->assign_inst.operand1_index = address_b;
    i8->assign_inst.operand2_index = address_ninehundred;
    i8->next = i9;

    i9->type = IN;                                      // input d
    i9->input_inst.var_index = address_d;
    i9->next = i10;

    i10->type = CJMP;                                   // if a > 10
    i10->cjmp_inst.condition_op = CONDITION_GREATER;
    i10->cjmp_inst.operand1_index = address_a;
    i10->cjmp_inst.operand2_index = address_ten;
    i10->cjmp_inst.target = i12;                        // if not (a > 10) skipp forward to NOOP
    i10->next = i11;

    i11->type = OUT;                                    // output d
    i11->output_inst.var_index = address_d;
    i11->next = i12;

    i12->type = NOOP;                                   // NOOP after inner IF
    i12->next = i13;

    i13->type = NOOP;                                   // NOOP after outer IF
    i13->next = i14;

    i14->type = ASSIGN;                                 // d = 0
    i14->assign_inst.left_hand_side_index = address_d;
    i14->assign_inst.op = OPERATOR_NONE;
    i14->assign_inst.operand1_index = address_zero;
    i14->next = i15;

    i15->type = CJMP;                                   // if d < 4
    i15->cjmp_inst.condition_op = CONDITION_LESS;
    i15->cjmp_inst.operand1_index = address_d;
    i15->cjmp_inst.operand2_index = address_four;
    i15->cjmp_inst.target = i22;                        // if not (d < 4) skip whole WHILE body
    i15->next = i16;

    i16->type = ASSIGN;                                 // c = a + d
    i16->assign_inst.left_hand_side_index = address_c;
    i16->assign_inst.op = OPERATOR_PLUS;
    i16->assign_inst.operand1_index = address_a;
    i16->assign_inst.operand2_index = address_d;
    i16->next = i17;

    i17->type = CJMP;                                   // if d > 1
    i17->cjmp_inst.condition_op = CONDITION_GREATER;
    i17->cjmp_inst.operand1_index = address_d;
    i17->cjmp_inst.operand2_index = address_one;
    i17->cjmp_inst.target = i19;                        // if not (d > 1) skip body of IF
    i17->next = i18;

    i18->type = OUT;                                    // output d
    i18->output_inst.var_index = address_d;
    i18->next = i19;

    i19->type = NOOP;                                   // NOOP after body of IF
    i19->next = i20;

    i20->type = ASSIGN;                                 // d = d + 1
    i20->assign_inst.left_hand_side_index = address_d;
    i20->assign_inst.op = OPERATOR_PLUS;
    i20->assign_inst.operand1_index = address_d;
    i20->assign_inst.operand2_index = address_one;
    i20->next = i21;

    i21->type = JMP;
    i21->jmp_inst.target = i15;
    i21->next = i22;

    i22->type = NOOP;                                   // NOOP after body of WHILE
    i22->next = NULL;

    // Inputs
    inputs.push_back(1);
    inputs.push_back(2);
    inputs.push_back(3);
    inputs.push_back(4);
    inputs.push_back(5);
    inputs.push_back(6);
    
    return i1;

     //cout << "expect" << endl;
    //cout << holder[0] << endl;
    //cout << holder[1] << endl;
    //cout << holder[2] << endl;
    //cout << holder[3] << endl;
    //cout << holder[holder.size() - 1] << endl;
    */