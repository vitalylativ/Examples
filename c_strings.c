#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.c"
#define SIZE 100

int programPosition;

//string basics
int strindex(char * str, char * find) 	//find a substring
{
	int i, j, k;
	for (i = 0; str[i] != '\0'; i ++)
	{
		for (j = i, k = 0; find[k] != '\0' && find[k] == str[j]; j++, k++)
		{
			if (find[k] == '\n')
				break;
		}
		if (k > 0 && (find[k] == '\0' || find[k] == '\n'))
			return i;
	}
	return -1;
}
void strCopy(char *to, char *from)
{
	while(*to++ = *from++)
		;
}
int strCoincide(char * str1, char * str2) //check if strings coinside
{
	if (strindex(str1, str2) >= 0 && strindex(str2, str1) >= 0)
		return 1;
	return 0;
}
int checkSymbolTable(char * name)		//find name in symbolTable
{
	for (int i = 0; i < lengthSymbolTable; i++)
	{
		if (strCoincide(symbolTable[i].name, name))
			return i;
	}
	return -1;
}

//text processing
char * cleanBlanks(const char * str, int len)	//removes empty spaces and comments
{
	int i = 0, j = 0, comment = 1;
	char* out = (char*)malloc(len);
	char c = *str;
	while (i < len)
	{
		if (c == '\0')
		{
			out[j] = '\0';
			return out;
		}else if (c == '\n')
		{
			if (j == 0)
			{
				out[0] = '\0';
				return out;
			}
			else
			{
				out[j] = '\n';
				out[j+1] = '\0';
			}	
			return out;
		}
		else if (c == ' ' || c == '\t')
		{
			i++;
		}else if (c == '/' && comment)
		{
			if (*(str + i + 1) == '/')
			{
				comment = 0;
				i += 2;
			} else
			{
				out[j] = c;
				j ++;
				i ++;
			}
		}else if (comment == 1)
		{
			out[j] = c;
			j ++;
			i ++;
		}else
		{
			i += 1;
		}
		c = *(str + i);
	}
	return out;
}
void addrToBinary(char * out, int addr)	//converts int to 16-bit string
{
	int base = 2;
	int bin = 0;
	for (int i = 0; i < 16; i ++)
	{
		bin = addr % base; //-> out + 15
		addr -= bin;
		base *= 2;
		if (bin > 0)
			*(out + 15 - i) = '1';
		else
			*(out + 15 - i) = '0';
	}
	*(out + 16) = '\n';
	*(out + 17) = '\0';
}
void aOperation(char * str, char * out) //A operation to 16-bit string
{
	int addr, temp;
	if (*(str + 1) >= '0' && *(str + 1) <= '9') //@digit
		addr = atoi(str + 1);	
	else
	{
		temp = checkSymbolTable(str + 1);
		if (temp >= 0)
			addr = symbolTable[temp].value;
		else
		{
			addSymbolTable(str + 1, valueForVarST);
			addr = valueForVarST++;
		}
	}
	addrToBinary(out, addr);
}
void cOperation(char * str, char * out) //C operation to 16-bit string
{
	int eqTemp = 0, colomnTemp = 20, jTemp, i;
	char tempOp[5];
	*(out) = '1';
	*(out + 1) = '1';
	*(out + 2) = '1';
	for (int i = 1; i < 4; i ++)
	{
		if (str[i] == '=')
		{
			if (0<= strindex(str, "A")&&strindex(str, "A") < i)
				*(out + 10) = '1';
			if (0 <= strindex(str, "D")&&strindex(str, "D") < i)
				*(out + 11) = '1';
			if (0 <= strindex(str, "M")&&strindex(str, "M") < i)
				*(out + 12) = '1';
			eqTemp = i;
		}
	}
	for (int i = eqTemp; i < 8; i ++)
	{
		if (str[i] == ';')
			colomnTemp = i;
	}	
	if (strindex(str, "JMP") >= 0)
		*(out + 13) = *(out + 14) = *(out + 15) = '1';
	else if (strindex(str, "JLE") >= 0)
		*(out + 13) = *(out + 14) = '1';
	else if (strindex(str, "JNE") >= 0)
		*(out + 13) = *(out + 15) = '1';
	else if (strindex(str, "JLT") >= 0)
		*(out + 13) = '1';
	else if (strindex(str, "JGE") >= 0)
		*(out + 15) = *(out + 14) = '1';
	else if (strindex(str, "JEQ") >= 0)
		*(out + 14) = '1';
	else if (strindex(str, "JGT") >= 0)
		*(out + 15) = '1';
	jTemp = (eqTemp > 0 ?  eqTemp + 1 : 0);
	for (i = jTemp; i < colomnTemp && *(str + i) != '\n'; i ++)
	{
		tempOp[i - jTemp] = *(str + i);
	}
	tempOp[i - jTemp] = '\0';
	if ((jTemp = strindex(tempOp, "M")) >= 0)
		*(out + 3) = '1';
	if ((jTemp = strindex(tempOp, "A")) >= 0)
		tempOp[jTemp] = 'M';
	if (strCoincide(tempOp, "D|M"))
		*(out + 5) = *(out + 7) = *(out + 9) = '1';
	if (strCoincide(tempOp, "M-D"))
		*(out + 7) = *(out + 8) = *(out + 9) = '1';
	if (strCoincide(tempOp, "D-M"))
		*(out + 5) = *(out + 8) = *(out + 9) = '1';
	if (strCoincide(tempOp, "D+M"))
		*(out + 8) = '1';
	if (strCoincide(tempOp, "M-1"))
		*(out + 4) = *(out + 5) = *(out + 8) = '1';
	if (strCoincide(tempOp, "M+1"))
		*(out + 4) = *(out + 5) = *(out + 8) = *(out + 7) = *(out + 9) = '1';
	if (strCoincide(tempOp, "-M"))
		*(out + 4) = *(out + 5) = *(out + 8) = *(out + 9) = '1';
	if (strCoincide(tempOp, "!M"))
		*(out + 4) = *(out + 5) = *(out + 9) = '1';
	if (strCoincide(tempOp, "M"))
		*(out + 4) = *(out + 5) = '1';
	if (strCoincide(tempOp, "0"))
		*(out + 4) = *(out + 6) = *(out + 8) = '1';
	if (strCoincide(tempOp, "1"))
		*(out + 4) = *(out + 6) = *(out + 8) =*(out + 9) =*(out + 5) =*(out + 7) = '1';
	if (strCoincide(tempOp, "-1"))
		*(out + 4) = *(out + 6) = *(out + 8) =*(out + 5) = '1';
	if (strCoincide(tempOp, "D"))
		*(out + 6) = *(out + 7) = '1';
	if (strCoincide(tempOp, "D&M"))
		;
	if (strCoincide(tempOp, "!D"))
		*(out + 6) = *(out + 7) = *(out + 9) = '1';
	if (strCoincide(tempOp, "-D"))
		*(out + 6) = *(out + 7) = *(out + 8) = *(out + 9) = '1';
	if (strCoincide(tempOp, "D+1"))
		*(out + 6) = *(out + 8) =*(out + 9) = *(out + 5) =*(out + 7) = '1';
	if (strCoincide(tempOp, "D-1"))
		*(out + 6) = *(out + 7) = *(out + 8) =  '1';
}
//implemented A and C operations don't check for errors

int checkIfEmpty(char * str) //checks if string is empty
{
	if (*str == '\n' || *str == '\0')
	{
		free(str);
		return 0;
	}
	return 1;
}
char *convertToBinary(char * str)	//
{
	char *out = NULL;
	out = (char *)malloc(18);
	if (checkIfEmpty(str) == 0)
		return NULL;

	for (int i = 0; i < 16; i ++)
	{
		*(out + i) = '0';
	}
	*(out + 16) = '\n';
	*(out + 17) = '\0';

	if (*str == '@')
		aOperation(str, out);
	else
		cOperation(str, out);
	return out;
}
int bracketsFirstPass(char *str)
{
	int out = 0, i;
	char *tempStr = (char *)malloc(100);
	if (checkIfEmpty(str) == 0)
		return 0;
	if (*str == '(')
	{
	
		for (i = 0; *(str + i + 1) != ')'; i ++)
			tempStr[i] =  str[i + 1];
		tempStr[i] = '\0';
		addSymbolTable(tempStr, programPosition);
		free(tempStr);
		return 0;
	}
	return 1;
}
void firstPass(const char * str)
{
	int i = 0, nread = 0;
	FILE * stream, *out;
	size_t len = SIZE;
	char * line = (char *)malloc(len);
	char * tempLine = NULL;
	
	stream = fopen(str, "r");
	if (stream == NULL) 
	{
		perror("fopen");
		exit(EXIT_FAILURE);
    }
    out = fopen("out_temp.txt", "w");
    if (out == NULL) 
	{
		perror("fopen");
		exit(EXIT_FAILURE);
    }

    while (((nread = getline(&line, &len, stream)) != -1 ))
	{
		tempLine = cleanBlanks(line, nread);
		if (bracketsFirstPass(tempLine) != 0)
		{
			fputs(tempLine, out);
			programPosition++;
		}
		free(tempLine);
	}
    free(line);
	fclose(stream);
	fclose(out);
}
void secondPass()
{
	int i = 0, nread = 0;
	FILE * stream, *out;
	size_t len = SIZE;
	char * line = (char *)malloc(len);
	char * tempLine = NULL;
	char * binaryTempLine = NULL;

	stream = fopen("out_temp.txt", "r");
	if (stream == NULL) 
	{
		perror("fopen");
		exit(EXIT_FAILURE);
    }
    out = fopen("out.txt", "w");
    if (out == NULL) 
	{
		perror("fopen");
		exit(EXIT_FAILURE);
    }
    while (((nread = getline(&line, &len, stream)) != -1 ))
	{
		//tempLine = cleanBlanks(line, nread);
		tempLine = line;
		binaryTempLine = convertToBinary(tempLine);
		if (binaryTempLine != NULL)
			fputs(binaryTempLine, out);
		free(tempLine);
		free(binaryTempLine);
	}
	fclose(stream);
	fclose(out);
	free(line);
   	if (remove("out_temp.txt"))
    	printf("the temp file is not deleted");
}
void convertToAsm(const char * str)
{
	definigSymbolTable();
	firstPass(str);
	secondPass();
	//printSymbolTable();
	free(symbolTable);
}
int main(int argc, char const *argv[])
{
	convertToAsm(argv[1]);
	return 0;
}