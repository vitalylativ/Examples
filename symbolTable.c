#include <stdio.h>
#include <stdlib.h>
struct symbolStructure{
	char name[100];
	int value;
};
struct symbolStructure *symbolTable; 
int lengthSymbolTable = 0;
int valueForVarST = 16;

void addSymbolTable(char * name, int value)
{
	int i;	
	symbolTable[lengthSymbolTable].value = value;
	for (i = 0; name[i] != '\0' && name[i] != '\n'; i++)
		symbolTable[lengthSymbolTable].name[i] = name[i];
	symbolTable[lengthSymbolTable++].name[i] = '\0';
}
static void definigSymbolTable()
{
	symbolTable = (struct symbolStructure *)malloc(1000 * sizeof(struct symbolStructure));
	for (int i = 0; i < 16; i ++)
	{
		if (i < 10)
		{
			symbolTable[i].name[0] = 'R';
			symbolTable[i].name[1] =  '0' + i;
			symbolTable[i].name[2] =  '\0';
			symbolTable[i].value = i;
		}else
		{
			symbolTable[i].name[0] = 'R';
			symbolTable[i].name[1] =  '1';
			symbolTable[i].name[2] =  '0' + i - 10;
			symbolTable[i].name[3] =  '\0';
			symbolTable[i].value = i;
		}		
	}
	lengthSymbolTable += 16;
	addSymbolTable("SCREEN", 16384);
	addSymbolTable("KBD", 24576);
	addSymbolTable("SP", 0);
	addSymbolTable("LCL", 1);
	addSymbolTable("ARG", 2);
	addSymbolTable("THIS", 3);
	addSymbolTable("THAT", 4);
}
//to add variable: addSymbolTable(varName, valueForVarST++);

void printSymbolTable()
{
	for (int i = 0; i < lengthSymbolTable; i ++)
	{
		printf("%d %s %d\n",i, symbolTable[i].name, symbolTable[i].value);
	}
}
/*int main(int argc, char const *argv[])
{
	definigSymbolTable();
	printSymbolTable();
	return 0;
}*/