/*
	Name 1: Sanjith Raman 
	Name 2: Sushruth Kaniyar 
	UTEID 1: sr57298
	UTEID 2: ssk3346
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_LINE_LENGTH 255
#define MAX_LABEL_LEN   20
#define MAX_SYMBOLS     255

enum { DONE, OK, EMPTY_LINE };

typedef struct { 
    int address;
    char label[MAX_LABEL_LEN + 1]; 
} TableEntry; 

TableEntry symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
 
FILE* infile  = NULL;
FILE* outfile = NULL;

static const char* opcodes[] = {
    "add", "and", "br", "brn", "brz", "brp", "brnz", "brnp", "brzp", "brnzp",
    "halt", "jmp", "jsr", "jsrr", "ldb", "ldw", "lea", "nop", "not", "ret",
    "lshf", "rshfl", "rshfa", "rti", "stb", "stw", "trap", "xor"
};

#define NUM_OPCODES ((int)(sizeof(opcodes) / sizeof(opcodes[0])))
 
int isOpcode(char* pStr) {
    for (int i = 0; i < NUM_OPCODES; i++)
        if (strcmp(pStr, opcodes[i]) == 0)
            return i;
    return -1;
}

int lookupLabel(char* label) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].label, label) == 0) {
            return symbolTable[i].address;
        }
    }
    printf("Error: undefined label %s\n", label);
    exit(4);
}

int toNum( char * pStr )
{
   char * t_ptr;
   char * orig_pStr;
   int t_length,k;
   int lNum, lNeg = 0;
   long int lNumLong;

   orig_pStr = pStr;
   if( *pStr == '#' )				/* decimal */
   { 
     pStr++;
     if( *pStr == '-' )				/* dec is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isdigit(*t_ptr))
       {
	 printf("Error: invalid decimal operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNum = atoi(pStr);
     if (lNeg)
       lNum = -lNum;
 
     return lNum;
   }
   else if( *pStr == 'x' )	/* hex     */
   {
     pStr++;
     if( *pStr == '-' )				/* hex is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isxdigit(*t_ptr))
       {
	 printf("Error: invalid hex operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
     lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
     if( lNeg )
       lNum = -lNum;
     return lNum;
   }
   else
   {
	printf( "Error: invalid operand, %s\n", orig_pStr);
	exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
   }
}

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
	** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
	)
	{
	   char * lRet, * lPtr;
	   int i;
	   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
	   for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
	   
           /* convert entire line to lowercase */
	   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	   /* ignore the comments */
	   lPtr = pLine;

	   while( *lPtr != ';' && *lPtr != '\0' &&
	   *lPtr != '\n' ) 
		lPtr++;

	   *lPtr = '\0';
	   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
		return( EMPTY_LINE );

	   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
	   {
		*pLabel = lPtr;
		if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   }
	   
           *pOpcode = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   
           *pArg1 = lPtr;
	   
           if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg2 = lPtr;
	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg3 = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg4 = lPtr;

	   return( OK );
	}

	/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */

    int main(int argc, char* argv[]) {
    char  lLine[MAX_LINE_LENGTH + 1];
    char *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
    int   lRet;
 
    if (argc < 3) {
        printf("Error: usage: %s <source.asm> <output.obj>\n", argv[0]);
        exit(4);
    }
 
    infile  = fopen(argv[1], "r");
    outfile = fopen(argv[2], "w");
    if (!infile)  { printf("Error: Cannot open file %s\n", argv[1]); exit(4); }
    if (!outfile) { printf("Error: Cannot open file %s\n", argv[2]); exit(4); }

      int currentAddress = -1;
    do {
        lRet = readAndParse(infile, lLine, &lLabel, &lOpcode,
                            &lArg1, &lArg2, &lArg3, &lArg4);
        if (lRet != DONE && lRet != EMPTY_LINE) {
            /* TODO*/
             if (strcmp(lOpcode, ".orig") == 0) {
                currentAddress = toNum(lArg1);
                continue; 
            }
            if (strcmp(lOpcode, ".end") == 0) {
                break; 
            }
            if (lLabel[0] != '\0') {
        symbolTable[symbolCount].address = currentAddress;
        strcpy(symbolTable[symbolCount].label, lLabel);
        symbolCount++;
        }
        currentAddress += 2; 
        }
        

    } while (lRet != DONE);
 
    rewind(infile); 

    int seenEnd = 0;

    do {
        lRet = readAndParse(infile, lLine, &lLabel, &lOpcode,
                            &lArg1, &lArg2, &lArg3, &lArg4);
        if (lRet != DONE && lRet != EMPTY_LINE) {
            /* TODO: pass 2 encoding + fprintf (see comment block above) */

            // PSEUDO CODE
            // START

            if(strcmp(lOpcode, ".orig") == 0){
              int temp = toNum(lArg1);
              if((temp >= 0) && (temp <= 65535) && (temp % 2 == 0)){
                currentAddress = temp;
                fprintf(outfile, "0x%.4X\n", currentAddress);
              }else{
                printf("Error: .orig argument is out of range\n"); 
                exit(4);
              }
              continue;
            }
            
            if(currentAddress == -1){
              printf("Error: missing .orig at the start of the program\n"); 
              exit(4);   
            }           
            
            if(strcmp(lOpcode, ".end") == 0){
              seenEnd = 1;
              break;
            }
            
            if(strcmp(lOpcode, ".fill") == 0){
              fprintf(outfile, "0x%.4X\n", toNum(lArg1) & 0xFFFF);
              currentAddress += 2;
              continue;
            }
            
            // processing opcodes

            // case ADD
            // 2 outcomes
            if(strcmp(lOpcode, "add") == 0){
              int isImm  = (lArg3[0] == '#' || lArg3[0] == 'x');
              int dr = lArg1[1] - '0';
              int sr1 = lArg2[1] - '0';
 
              int instruction = (0b0001 << 12 | (dr << 9) | (sr1 << 6));
              if(isImm){
              int imm = toNum(lArg3);
              if(imm > 15 || imm < -16){
                printf("Error: immediate out of range for ADD, %s\n", lArg3);
                exit(4);
              }
              int imm5 = imm & 0x1F; 
                instruction |= (1 << 5) | (imm5); 
              }else{
                int sr2 = lArg3[1] - '0';
                instruction |= sr2;
              }
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case AND
            // 2 outcomes
            if(strcmp(lOpcode, "and") == 0){
              int isImm  = (lArg3[0] == '#' || lArg3[0] == 'x');
              int dr = lArg1[1] - '0';
              int sr1 = lArg2[1] - '0';
 
              int instruction = (0b0101 << 12 | (dr << 9) | (sr1 << 6));
              if(isImm){
              int imm = toNum(lArg3);
              if(imm > 15 || imm < -16){
                printf("Error: immediate out of range for AND, %s\n", lArg3);
                exit(4);
              }
              int imm5 = imm & 0x1F; 
                instruction |= (1 << 5) | (imm5); 
              }else{
                int sr2 = lArg3[1] - '0';
                instruction |= sr2;
              }
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case BR (with any nzp bits)
            if(strncmp(lOpcode, "br", 2) == 0){
              int n = 0, z = 0, p = 0;
              char* nzpBits = lOpcode + 2;
              if (strlen(nzpBits) == 0){
                n = z = p = 1; 
              }
              if(strchr(nzpBits, 'n')){
                n = 1;
              }
              if(strchr(nzpBits, 'z')){
                z = 1;
              }
              if(strchr(nzpBits, 'p')){
                p = 1;
              }
              int targetAddr = lookupLabel(lArg1);
              int offset = (targetAddr - (currentAddress + 2)) / 2;
              if(offset < -256 || offset > 255){
                printf("Error: PC offset out of range for BR, %s\n", lArg1);
                exit(4);
              }
              int offset9 = offset & 0x1FF;
              int instruction = (0b0000 << 12 | n << 11 | z << 10 | p << 9 | offset9);
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case HALT
            if (strcmp(lOpcode, "halt") == 0) {
              int instruction = (0b1111 << 12) | 0x25;   // TRAP x25
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case JMP
            if(strcmp(lOpcode, "jmp") == 0){
              int baseR = lArg1[1] - '0';
              int instruction = (0b11 << 14) | (baseR << 6);
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case RET
            if(strcmp(lOpcode, "ret") == 0){
              int instruction = 0b1100000111000000;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case JSR
            if(strcmp(lOpcode, "jsr") == 0){
              int target = lookupLabel(lArg1);
              int offset = (target - (currentAddress + 2)) / 2;
              if((offset < -1024 ) || (offset > 1023)){
                printf("Error: PC offset out of range for JSR, %s\n", lArg1);
                exit(4);
              }
              int offset11 = offset & 0x7FF;
              int instruction = (0b01001 << 11) | offset11;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case JSRR
            if(strcmp(lOpcode, "jsrr") == 0){
              int baseR = lArg1[1] - '0';
              int instruction = (0b01 << 14) | (baseR << 6);
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case LDB
            if(strcmp(lOpcode, "ldb") == 0){
              int dr = lArg1[1] - '0';
              int baseR = lArg2[1] - '0';
              int offset = toNum(lArg3);
              if(offset < -32 || offset > 31){
                printf("Error: boffset6 out of range for LDB, %s\n", lArg3);
                exit(4);
              }
              int offset6 = offset & 0x3F;
              int instruction = (0b0010 << 12) | (dr << 9) | (baseR << 6) | offset6;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case LDW
            if(strcmp(lOpcode, "ldw") == 0){
              int dr = lArg1[1] - '0';
              int baseR = lArg2[1] - '0';
              int offset = toNum(lArg3);
              if(offset < -32 || offset > 31){
                printf("Error: boffset6 out of range for LDW, %s\n", lArg3);
                exit(4);
              }
              int offset6 = offset & 0x3F;
              int instruction = (0b0110 << 12) | (dr << 9) | (baseR << 6) | offset6;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case LEA
            if(strcmp(lOpcode, "lea") == 0){
              int dr = lArg1[1] - '0';
              int targetAddr = lookupLabel(lArg2);
              int offset = (targetAddr - (currentAddress + 2)) / 2;
              if(offset < -256 || offset > 255){
                printf("Error: PC offset out of range for LEA, %s\n", lArg2);
                exit(4);
              }
              int offset9 = offset & 0x1FF; 
              int instruction = (0b1110 << 12) | (dr << 9) | (offset9);
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case NOT
            if(strcmp(lOpcode, "not") == 0){
              int dr = lArg1[1] - '0';
              int sr = lArg2[1] - '0';
              int instruction = (0b1001 << 12) | (dr << 9) | (sr << 6) | 0b111111;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue; 
            }

            // case RTI
            if(strcmp(lOpcode, "rti") == 0){
              int instruction = (0b1 << 15);
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;  
            }     

            // case SHF
            if((strcmp(lOpcode, "lshf") == 0) || (strcmp(lOpcode, "rshfl") == 0) || (strcmp(lOpcode, "rshfa") == 0)){
              int bits54;
              int dr = lArg1[1] - '0';
              int sr = lArg2[1] - '0';
              int amount = toNum(lArg3);
              if((amount < 0) || amount > 15){
                printf("Error: shift amount out of range, %s\n", lArg3);
                exit(4);  
              }
              int amount4 = amount & 0xF;
              if(strcmp(lOpcode, "lshf") == 0){
                bits54 = 0b00;
              }else if(strcmp(lOpcode, "rshfl") == 0){
                bits54 = 0b01;
              }
              else{
                bits54 = 0b11;
              }
              int instruction = (0b1101 << 12) | (dr << 9) | (sr << 6) | (bits54 << 4) | amount4;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue; 
            }    

            // case STB
            if(strcmp(lOpcode, "stb") == 0){
              int sr = lArg1[1] - '0';
              int baseR = lArg2[1] - '0';
              int offset = toNum(lArg3); 
              if(offset < -32 || offset > 31){
                printf("Error: boffset6 out of range for STB, %s\n", lArg3);
                exit(4);
              }  
              int offset6 = offset & 0x3F;
              int instruction = (0b0011 << 12) | (sr << 9) | (baseR << 6) | offset6;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;     
            }

            // case STW
            if(strcmp(lOpcode, "stw") == 0){
              int sr = lArg1[1] - '0';
              int baseR = lArg2[1] - '0';
              int offset = toNum(lArg3); 
              if(offset < -32 || offset > 31){
                printf("Error: offset6 out of range for STW, %s\n", lArg3);
                exit(4);
              }  
              int offset6 = offset & 0x3F;
              int instruction = (0b0111 << 12) | (sr << 9) | (baseR << 6) | offset6;
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;     
            }

            // case TRAP
            if(strcmp(lOpcode, "trap") == 0){
            int trapvect = toNum(lArg1);
            if(trapvect < 0 || trapvect > 255){
              printf("Error: trapvect8 out of range for TRAP, %s\n", lArg1);
              exit(4);
            }
            int trapvect8 = trapvect & 0xFF;
            int instruction = (0b1111 << 12) | trapvect8;
            fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
            currentAddress += 2;
            continue; 
            }

            // case XOR
            // 2 outcomes
            if(strcmp(lOpcode, "xor") == 0){
              int isImm  = (lArg3[0] == '#' || lArg3[0] == 'x');
              int dr = lArg1[1] - '0';
              int sr1 = lArg2[1] - '0';
 
              int instruction = (0b1001 << 12 | (dr << 9) | (sr1 << 6));
              if(isImm){
              int imm = toNum(lArg3);
              if(imm > 15 || imm < -16){
                printf("Error: immediate out of range for XOR, %s\n", lArg3);
                exit(4);
              }
              int imm5 = imm & 0x1F; 
                instruction |= (1 << 5) | (imm5); 
              }else{
                int sr2 = lArg3[1] - '0';
                instruction |= sr2;
              }
              fprintf(outfile, "0x%.4X\n", instruction & 0xFFFF);
              currentAddress += 2;
              continue;
            }

            // case NOP
            if(strcmp(lOpcode, "nop") == 0){
              fprintf(outfile, "0x%.4X\n", 0x0000);
              currentAddress += 2;
              continue;
            }

            // PSEUDO CODE
            // END
        }
    } while (lRet != DONE);

    if(!seenEnd){
      printf("Error: file is missing .end\n");
      exit(4);
    }
 
    fclose(infile);
    fclose(outfile);
    return 0;   
}




