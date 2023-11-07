// MMKLogixCompiler Header file.
// Developed by:  Mehrbod Molla Kazemi.
// Professor:  Dr. Safaei Mehrabani.

#pragma once

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
};

// Compiler's semantic table element structure.
typedef struct Struct_SemanticTable
{
	string l_operand;
	string first_operand;
	string middle_operator;
	string second_operand;
};

// Function Prototypes.

// Generates symboliv and semantic tables from input line string.
bool A_GenerateTable(string inputLine, 
	vector<Struct_SymbolicTable>& ref_vec_SymbolicTable,
	vector<Struct_SemanticTable>& ref_vec_SemanticTable);
