#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() 
{
    FILE *optab,*symtab,*inter,*objcode;
    char *temp,name[6],opcode[10],address[10],label[10],operand[10],obj[100],tRecord[100],str[20];
    int rLength=0,cLength=0,start=0,end=0,i,tAddress=0,nextAddress,flag,len;

    optab=fopen("optab.txt","r");
    symtab=fopen("symtab.txt","r");
    inter=fopen("inter.txt","r");
    objcode=fopen("objectcode.txt","w");

    if(optab==NULL ||symtab==NULL ||inter==NULL ||objcode==NULL)
    {
        printf("Unable to open one or more files");
        return 1;
    }

    fseek(inter,-20,SEEK_END);
    fscanf(inter,"%s",address);
    end=strtol(address,&temp,16);
    rewind(inter);
    fscanf(inter,"%s\t%s\t%s",name,opcode,address);
    if(strcmp(opcode,"START")==0)
    {
        start=strtol(address,&temp,16);
        printf("%x  ,%x  ",start,end);
        int pLength=end-start;
        fprintf(objcode,"H^%6s^%06x^%06x\n",name,start,pLength);
        fscanf(inter,"%s\t%s\t%s\t%s",address,label,opcode,operand);
    }
    
    rLength=0,tAddress=start,nextAddress=start;
    strcpy(tRecord,"");
    while(strcmp(opcode,"END")!=0)
    { 
        char mne[10],code[10];
        strcpy(obj,"");
        flag=0;
        rewind(optab);
        while(fscanf(optab,"%s\t%s",mne,code)==2)
        {
            if(strcmp(mne,opcode)==0)
            {
                flag=1;
                strcat(obj,code);
                break;
            }
        }
        if(flag==1)
        {
            if(strcmp(operand,"")!=0)
            {
                char symbol[10],add[10];
                flag=0;
                rewind(symtab);
                while(fscanf(symtab,"%s\t%s",symbol,add)==2)
                {
                    if(strcmp(symbol,operand)==0)
                    {
                        flag=1;
                        strcat(obj,add);
                        break;
                    }
                }
            }
            else    
            {
                strcat(obj,"0000");
            }
        }
        else if(strcmp(opcode,"BYTE")==0)
        {
            len=strlen(operand);
            if(operand[0]=='X')
            {
                strncat(obj,operand+2,len-3);
            }
            else
            {
                for(i=2;i<len-1;i++)
                {
                    int asci=operand[i];
                    sprintf(str,"%x",asci);
                    strcat(obj,str);
                }
            }
        }
        else if(strcmp(opcode,"WORD")==0)
        {
            sprintf(str,"%06d",atoi(operand));
            strcat(obj,str);
        }
        else if(!(strcmp(opcode,"RESW") || strcmp(opcode,"RESB")))
        {
            printf("Undefined RESW or RESB");
        }

        rLength=cLength;
        cLength+=strlen(obj);

        if(cLength<=60)
        {
            if(strlen(obj)!=0)
            {
                strcat(obj,"^");
                strcat(tRecord,obj);
            }
        }

        int l=strlen(tRecord);
        tRecord[l]='\0';

        nextAddress=strtol(address,&temp,16);
        fscanf(inter,"%s\t%s\t%s\t%s",address,label,opcode,operand);

        if(cLength>60 || strcmp(opcode,"END")==0)
        {
            fprintf(objcode,"T^%06x^%02x^%s\n",tAddress,rLength/2,tRecord);
            strcpy(tRecord,"");
            cLength=strlen(obj);
            strcpy(obj,"^");
            strcat(tRecord,obj);

        }
    }

    fprintf(objcode,"E^%06x^\n",start);
    fclose(optab);
    fclose(inter);
    fclose(symtab);
    fclose(objcode);
    return 0;
}