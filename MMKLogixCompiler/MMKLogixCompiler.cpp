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
    Compiler_Mode compilerMode;

    // Open program.txt.
    ifstream file_Program("program.txt");

    // Check if file exists.
    if (!file_Program.is_open())
    {
        cout << "File program.txt not found!\b" << endl;
        system("PAUSE");

        return -2;
    }

    // Ask user to run in sequential or concurrent mode?
    cout << "Do you want to run the compiler in Concurrent mode? (N= Sequential, Y= Concurrent), then press ENTER:\t";
    char c = getchar();
    if (c == 'y' || c == 'Y')
        compilerMode = Concurrent;
    else
        compilerMode = Sequential;

    // Read program line by line.
    string line;
    int lineCounter = 1;
    while (getline(file_Program, line))
    {
        try
        {
            A_GenerateTables(line, compiler_Symbolics, compiler_Semantics);

            lineCounter++;
        }
        catch (int errCode)
        {
            cout << "ERROR Line " << lineCounter << ":\t";

            switch (errCode)
            {
            case COMPILER_ERROR_IDENTIFIER_EXPECTED:
                cout << "Expected identifier.";
                break;

            case COMPILER_ERROR_ONE_ASSIGNMENT:
                cout << "Only one assignment operator allowed.";
                break;

            case COMPILER_ERROR_EXPECTED_ASSIGNMENT_BEFORE_OPERATOR:
                cout << "Expected assignment before operator.";
                break;

            case COMPILER_ERROR_ONE_OPERATOR_ALLOWED:
                cout << "Only one secondary operator allowed.";
                break;

            case COMPILER_ERROR_LOPERAND_NOT_MEMORY_ZONE:
                cout << "Left-Operand must be a memory zone/variable name, not immediate.";
                break;

            case COMPILER_ERROR_DUPLICATED_OPERATORS:
                cout << "Duplicated operators.";
                break;

            case COMPILER_ERROR_INCOMPLETE_STATEMENT:
                cout << "Incomplete statement.";
                break;
            }

            cout << endl << endl;

            system("PAUSE");
            return errCode * -1;
        }
    }

    cout << endl << endl << "File compiled successfully.";
    cout << endl << "Executing..." << endl << endl;

    for (size_t i = 0; i < compiler_Semantics.size(); i++)
    {

    }
}

// Generates symbolic and semantic tables from input line string.
void A_GenerateTables(string inputLine,
    vector<Struct_SymbolicTable>& ref_vec_SymbolicTable,
    vector<Struct_SemanticTable>& ref_vec_SemanticTable)
{
    string identifier = "";
    Compiler_Phase phase = Undefined;

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
        }
        else if (semanticTable.second_operand == "")
        {
            semanticTable.second_operand = identifier;
        }
    }
    else if (phase == Immediate_Integer)
    {
        cout << "Detected Immediate Integer:  " << identifier << endl;

        if (semanticTable.first_operand == "")
        {
            semanticTable.first_operand = identifier;
        }
        else if (semanticTable.second_operand == "")
        {
            semanticTable.second_operand = identifier;
        }
    }
    else
    {
        throw COMPILER_ERROR_INCOMPLETE_STATEMENT;
    }

    ref_vec_SemanticTable.push_back(semanticTable);

    // Semantic generated.
    cout << "Semantic generated:  ";
    cout << semanticTable.l_operand << " = ";
    cout << semanticTable.first_operand << " ";
    if (semanticTable.second_operand != "")
    {
        cout << semanticTable.middle_operator << " " << semanticTable.second_operand;
    }
    cout << endl << endl;
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

    return true;
}

// Interprets a compiler-generated semantic token and updates symbolics' table.
void A_InterpretSemantic(Struct_SemanticTable semantic,
    vector<Struct_SymbolicTable>& ref_vec_symbolics)
{
    string target = semantic.l_operand;
    if (target == "")
        throw INTERPRETER_ERROR_NULL_LOPERAND;

    string firstOperand = semantic.first_operand;


}
