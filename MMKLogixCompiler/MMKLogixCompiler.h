// MMKLogixCompiler Header file.
// Developed by:  Mehrbod Molla Kazemi.
// Professor:  Dr. Safaei Mehrabani.

#pragma once

// Definitions.
#define COMPILER_ERROR_IDENTIFIER_EXPECTED 100

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
	EndOfStatement = 4,
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
