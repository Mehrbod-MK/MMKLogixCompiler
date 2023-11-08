// MMKLogixCompiler Header file.
// Developed by:  Mehrbod Molla Kazemi.
// Professor:  Dr. Safaei Mehrabani.

#pragma once

// Definitions.
#define COMPILER_ERROR_IDENTIFIER_EXPECTED 100
#define COMPILER_ERROR_ONE_ASSIGNMENT 101
#define COMPILER_ERROR_EXPECTED_ASSIGNMENT_BEFORE_OPERATOR 102
#define COMPILER_ERROR_ONE_OPERATOR_ALLOWED 103
#define COMPILER_ERROR_LOPERAND_NOT_MEMORY_ZONE 104
#define COMPILER_ERROR_DUPLICATED_OPERATORS 105
#define COMPILER_ERROR_INCOMPLETE_STATEMENT 106

#define INTERPRETER_ERROR_NULL_LOPERAND 200
#define INTERPRETER_ERROR_NULL_ROPERAND 201
#define INTERPRETER_ERROR_SYMBOLIC_NOT_FOUND 202

#define EXCEPTION_ARGUMENTEXCEPTION 10

// Includes.
#include <iostream>
#include <fstream>

#include <vector>
#include <string>

// Namespaces.
using namespace std;

// Structures.
// Compiler's symbolic table element structure.
typedef struct Struct_SymbolicTable
{
	string name;
	string value;

} Struct_SymbolicTable;

// Compiler's semantic table element structure.
typedef struct Struct_SemanticTable
{
	string l_operand;
	string first_operand;
	string middle_operator;
	string second_operand;
} Struct_SemanticTable;

// Compiler enumerations.
enum Compiler_Phase
{
	Undefined = 0,

	VariableName = 1,
	Assignment = 2,
	Operator = 3,
	Immediate_Integer = 4,
	EndOfStatement = 5,
};

enum Compiler_Mode
{
	Sequential = 0,
	Concurrent = 1,
};

// Function Prototypes.

// Generates symbolic and semantic tables from input line string.
void A_GenerateTables(string inputLine, 
	vector<Struct_SymbolicTable>& ref_vec_SymbolicTable,
	vector<Struct_SemanticTable>& ref_vec_SemanticTable);

// Checks if a symbol exists in compiler's symbolic table.
bool C_SymbolExists(string symbolName, 
	vector<Struct_SymbolicTable> vec_SymbolicTable);

// Adds a new symbolic element to a symbolic table, if it doesn't exist.
bool A_AddSymbolToTable(string symbolName, string symbolValue,
	vector<Struct_SymbolicTable>& ref_vec_SymbolicTable);

// Interprets a compiler-generated semantic token and updates symbolics' table.
void A_InterpretSemantic(Struct_SemanticTable semantic,
	vector<Struct_SymbolicTable>& ref_vec_Symbolics);

// Fetches the true value of a operand, by referring to symbolics' table.
int M_FetchMemoryZone(vector<Struct_SymbolicTable>& ref_vec_Symbolics,
	string operand);

// Fetches a symbolic index by looking up its name in symbolics' table.
size_t M_FetchSymbolicIndex(vector<Struct_SymbolicTable> vec_Symbolics,
	string symbolicName);

// Checks whether an input string is a number or not.
bool C_IsStringANumber(string str);

// Converts a numerical string to its integer representation.
int Conv_FromString_ToInt32(string str);

// Checks if a certain semantic has dependency on an operand.
bool C_SemanticDependsOn(Struct_SemanticTable semantic, string operandName);
