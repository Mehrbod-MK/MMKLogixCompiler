// MMKLogixCompiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Developed by:  Mehrbod Molla Kazemi.
// Professor:  Dr. Safaei Mehrabani.

#include "MMKLogixCompiler.h"

int main()
{
    // Welcome text.
    cout << "MMKLogixCompiler Program\nDeveloped by: Mehrbod Molla Kazemi\nProfessor:  Dr. Safaei Mehrabani\n\n";

    // Compiler definitions.
    vector<Struct_SymbolicTable> compiler_Symbolics;
    vector<Struct_SemanticTable> compiler_Semantics;

    // Open program.txt.
    ifstream file_Program("program.txt");

    // Check if file exists.
    if (!file_Program.is_open())
    {
        cout << "File program.txt not found!\b" << endl;
        system("PAUSE");

        return -2;
    }

    // Read program line by line.
    string line;
    while (getline(file_Program, line))
    {
        A_GenerateTables(line, compiler_Symbolics, compiler_Semantics);
    }
}

// Generates symbolic and semantic tables from input line string.
void A_GenerateTables(string inputLine,
    vector<Struct_SymbolicTable>& ref_vec_SymbolicTable,
    vector<Struct_SemanticTable>& ref_vec_SemanticTable)
{
    static string identifier = "";
    static Compiler_Phase phase = Undefined;

    // Compiler semantics.
    Struct_SemanticTable semanticTable = { "", "", "", "" };

    for (size_t i = 0; i < inputLine.length(); i++)
    {
        char x = inputLine[i];        

        // x is a supported character type.
        if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'))
        {
            if (phase == Undefined)
            {
                identifier = "";

                identifier += x;
                phase = VariableName;
            }
            else if (phase == VariableName)
                identifier += x;
            else if (phase == Assignment)
            {
                identifier = "";

                identifier += x;
                phase = VariableName;
            }
            else if (phase == Operator)
            {
                identifier = "";

                identifier += x;
                phase = VariableName;
            }
            else if (phase == Immediate_Integer)
                identifier += x;
        }
        // x is a digit 0-9.
        else if ((x >= '0') && (x <= '9'))
        {
            if (phase == Undefined)
            {
                throw COMPILER_ERROR_IDENTIFIER_EXPECTED;
            }
            else if (phase == VariableName)
            {
                identifier += x;
            }
            else if (phase == Assignment)
            {
                identifier = "";

                phase = Immediate_Integer;
                identifier += x;
            }
            else if (phase == Operator)
            {
                identifier = "";

                phase = Immediate_Integer;
                identifier += x;
            }
            else if (phase == Immediate_Integer)
                identifier += x;
        }
        // x is assignment operator.
        else if(x == '=')
        {
            // Undefined is not allowed, throw error.
            if (phase == Undefined)
            {
                throw COMPILER_ERROR_IDENTIFIER_EXPECTED;
            }
            else if (phase == VariableName)
            {
                if (semanticTable.l_operand == "")
                    semanticTable.l_operand = identifier;
                else
                {
                    // Throw one assignment error.
                    throw COMPILER_ERROR_ONE_ASSIGNMENT;
                }

                if (A_AddSymbolToTable(identifier, "", ref_vec_SymbolicTable))
                {
                    cout << "Added symbolic identifier:  " << identifier << endl;
                }
                else
                {
                    cout << "Identifier found:  " << identifier << endl;
                }

                phase = Assignment;
            }
            else if (phase == Assignment)
            {
                // Throw one assignment error.
                throw COMPILER_ERROR_ONE_ASSIGNMENT;
            }
            else if (phase == Operator)
            {
                throw COMPILER_ERROR_IDENTIFIER_EXPECTED;
            }
            else if (phase == Immediate_Integer)
            {
                throw COMPILER_ERROR_LOPERAND_NOT_MEMORY_ZONE;
            }
        }
        // x is another operator.
        else if (x == '+' || x == '-' || x == '*' || x == '/')
        {
            // Undefined is not allowed, throw error.
            if (phase == Undefined)
            {
                throw COMPILER_ERROR_IDENTIFIER_EXPECTED;
            }
            else if (phase == VariableName)
            {
                if (semanticTable.l_operand == "")
                    throw COMPILER_ERROR_EXPECTED_ASSIGNMENT_BEFORE_OPERATOR;

                if (semanticTable.middle_operator != "")
                    throw COMPILER_ERROR_ONE_OPERATOR_ALLOWED;

                semanticTable.middle_operator = x;

                if (A_AddSymbolToTable(identifier, "", ref_vec_SymbolicTable))
                {
                    cout << "Added symbolic identifier:  " << identifier << endl;
                }
                else
                {
                    cout << "Identifier found:  " << identifier << endl;
                }

                if (semanticTable.first_operand == "")
                {
                    semanticTable.first_operand = identifier;
                    phase = Operator;
                }
                else
                {
                    throw COMPILER_ERROR_ONE_OPERATOR_ALLOWED;
                }
            }
            else if (phase == Assignment)
            {
                throw COMPILER_ERROR_IDENTIFIER_EXPECTED;
            }
            else if (phase == Operator)
            {
                throw COMPILER_ERROR_DUPLICATED_OPERATORS;
            }
            else if (phase == Immediate_Integer)
            {
                if (semanticTable.l_operand == "")
                    throw COMPILER_ERROR_EXPECTED_ASSIGNMENT_BEFORE_OPERATOR;

                if (semanticTable.middle_operator != "")
                    throw COMPILER_ERROR_ONE_OPERATOR_ALLOWED;

                semanticTable.middle_operator = x;

                if (semanticTable.first_operand == "")
                {
                    cout << "Detected Immediate Integer:  " << identifier << endl;

                    semanticTable.first_operand = identifier;
                    phase = Operator;
                }
                else
                {
                    throw COMPILER_ERROR_ONE_OPERATOR_ALLOWED;
                }
            }
        }
    }

    // Finish the tail.
    if (phase == VariableName)
    {
        if (semanticTable.second_operand == "")
        {
            if (A_AddSymbolToTable(identifier, "", ref_vec_SymbolicTable))
            {
                cout << "Added symbolic identifier:  " << identifier << endl;
            }
            else
            {
                cout << "Identifier found:  " << identifier << endl;
            }
            
            semanticTable.second_operand = identifier;
        }
    }
}

// Checks if a symbol exists in compiler's symbolic table.
bool C_SymbolExists(string symbolName,
    vector<Struct_SymbolicTable> vec_SymbolicTable)
{
    for (size_t i = 0; i < vec_SymbolicTable.size(); i++)
    {
        if (vec_SymbolicTable[i].name == symbolName)
            return true;
    }
    return false;
}

bool A_AddSymbolToTable(string symbolName, string symbolValue,
    vector<Struct_SymbolicTable>& ref_vec_SymbolicTable)
{
    if (C_SymbolExists(symbolName, ref_vec_SymbolicTable))
        return false;

    ref_vec_SymbolicTable.push_back({ symbolName, symbolValue });
}
