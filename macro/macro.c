#include<stdio.h>
#include<string.h>
void main()
{
    FILE *in,*out,*nam,*def,*arg;
    int len,i,pos=1;
    char arg1[33],mne[33],op[33],label[33],mName[33],mne1[33],op1[33],str1[33],str2[33];

    in=fopen("input.txt","r");
    out=fopen("output.txt","w+");
    nam=fopen("namtab.txt","w+");
    def=fopen("deftab.txt","w+");
    arg=fopen("argtab.txt","w+");

    fscanf(in,"%s %s %s", label,mne,op);
    while(strcmp(mne,"END")!=0)
    {
        if(strcmp(mne,"MACRO")==0)
        {
            fprintf(nam,"%s\n",label);
            rewind(nam);
            fprintf(def,"%s\t%s\n",label,op);
            fscanf(in,"%s %s %s", label,mne,op);
            while(strcmp(mne,"MEND")!=0)
            {
                if(op[0]=='&')
                {
                    sprintf(str1,"%d",pos);
                    strcpy(str2,"?");
                    strcpy(op,strcat(str2,str1));
                    pos++;
                    fprintf(def,"%s\t%s\n",mne,op);
                }
                else
                {
                    fprintf(def,"%s\t%s\n",mne,op);
                }
                fscanf(in,"%s %s %s", label,mne,op);
            }
            fprintf(def,"%s",mne);
        }
        else
        {
            fscanf(nam,"%s",mName);
            if(strcmp(mne,mName)==0)
            {
                len=strlen(op);
                for(i=0;i<len;i++)
                {
                    if(op[i]!=',')
                    {
                        fprintf(arg,"%c",op[i]);
                    }
                    else
                    {
                        fprintf(arg,"\n");
                    }
                }
                rewind(def);
                fprintf(out,"**\t . %s\t%s\n",mne,op);
                fscanf(def,"%s%s",mne1,op1);
                rewind(arg);
                while(strcmp(mne1,"MEND")!=0)
                {
                    if(op1[0]=='?')
                    {
                        fscanf(arg,"%s",arg1);
                        fprintf(out,"**\t%s\t%s\n",mne1,arg1);
                    }
                    else if(op1[0]!='&')
                    {
                        fprintf(out,"**\t%s\t%s\n",mne1,op1);
                    }
                    fscanf(def,"%s%s",mne1,op1);
                }
            }
            else
            {
                fprintf(out,"%s\t%s\t%s\n",label,mne,op);
            }
        }
        fscanf(in,"%s %s %s",label,mne,op);
    }
    fprintf(out,"%s\t%s\t%s\n",label,mne,op);
    fclose(in);
    fclose(out);
    fclose(nam);
    fclose(def);
    fclose(arg);
}