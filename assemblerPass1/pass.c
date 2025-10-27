#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int check_symtab(FILE *symtab,const char *label)
{
    char symtab_label[30];
    int address;
    rewind(symtab);
    while(fscanf(symtab,"%s\t%d",symtab_label,&address)==2)
    {
        if(strcmp(symtab_label,label)==0)
        {
            return 1;
        }
    }
    return 0;
}

void main()
{
    FILE *input,*optab,*symtab,*inter,*len;
    char label[30],opcode[30],operand[30],mneumonic[30],code[30]; 
    int locctr=0,start=0;

    input=fopen("input.txt","r");
    inter=fopen("inter.txt","w");
    optab=fopen("optab.txt","r");
    symtab=fopen("symtab.txt","w+");
    len=fopen("len.txt","w");

    if(input==NULL || optab==NULL || inter==NULL || len==NULL || symtab==NULL)
    {
        printf("Unable to open one or more files!");
        exit(1);
    }

    fscanf(input,"%s\t%s\t%s",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        start=atoi(operand);
        locctr=start;
        printf("START : %d\n",start);
        fprintf(inter,"%s\t%s\t%s\n",label,opcode,operand);
    }
    else
    {
        locctr=0;
    }

    fscanf(input,"%s\t%s\t%s",label,opcode,operand);
    while(strcmp(opcode,"END")!=0)
    {
        fprintf(inter,"%d\t",locctr);
        if(strcmp(label,"**")!=0)
        {
            if(check_symtab(symtab,label))
            {
                printf("Symbol Already present in symtab!");
            }
            else
            {
                if(fprintf(symtab,"%s\t%d\n",label,locctr)<0)
                {
                    printf("ERROR : Falied to write in symtab!");
                    fclose(len);
                    fclose(symtab);
                    fclose(inter);
                    fclose(input);
                    fclose(optab);
                    exit(1);
                }
            }
        }

        int found_opcode=0;
        rewind(optab);
        while(fscanf(optab,"%s\t%s",code,mneumonic)==2)
        {
            if(strcmp(code,opcode)==0)
            {
                locctr+=3;
                found_opcode=1;
                break;
            }
        }
        if(!found_opcode)
        {
            if(strcmp(opcode,"WORD")==0)
                locctr+=3;
            else if(strcmp(opcode,"RESB")==0)
                locctr+=atoi(operand);
            else if(strcmp(opcode,"RESW")==0)
                locctr+=3*atoi(operand);
            else if(strcmp(opcode,"BYTE")==0)
            {
                if(operand[0]=='C')
                    locctr+=strlen(operand)-3;
                else if(operand[0]=='X')
                    locctr+=(strlen(operand)-3)/2;
            }
        }
        fprintf(inter,"%s\t%s\t%s\n",label,opcode,operand);
        fscanf(input,"%s\t%s\t%s",label,opcode,operand);
    }
    fprintf(inter,"%s\t%s\t%s\n",label,opcode,operand);
    fprintf(len,"%d",locctr-start);
    printf("Length of the program : %d",locctr-start);
    fclose(len);
    fclose(symtab);
    fclose(inter);
    fclose(input);
    fclose(optab);
}