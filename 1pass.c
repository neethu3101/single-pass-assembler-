#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int search(FILE *f,char value[],char symbol2[]){
    char symbol1[10];
    fseek(f,0,SEEK_SET);
    while(!feof(f))
    {
        fscanf(f,"%s%s",symbol1,symbol2);
        if(strcmp(value,symbol1)==0)
            return 1;
    }
    return 0;
}
void main()
{
    FILE *f1,*f2,*f3,*f4;
    int flag=0,locctr,startadr,len;
    char operand[10],label[10],opcode[10],mne[10],opc[10],symaddr[10],symbol[10];

    f1=fopen("input.txt","r");
    f2=fopen("optab.txt","r");
    f3=fopen("symtab.txt","w+");
    f4=fopen("list.txt","w+");
    fscanf(f1,"%s%s%s",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        startadr=atoi(operand);
        locctr=startadr;
    }
    printf("H^%s^00%d^000000\n",label,startadr);
    fscanf(f1,"%s%s%s",label,opcode,operand);
    printf("T^00%d^",startadr);
    while(strcmp(opcode,"END")!=0)
    {
        if(strcmp(label,"-")!=0){
            flag = search(f3,label,symaddr);   //searching for label in symtab
            if(flag == 1){
                if(strcmp(symaddr,"0000")==0){          //if symbol address is null
                    fseek(f3,-strlen(symaddr),SEEK_CUR);
                    fprintf(f3,"%d",locctr);
                    flag = search(f4,label,symaddr);    //searching list of forward references
                    if(flag == 1){
                        printf("\nT^%s^02^%d",symaddr,locctr);
                        //if(strcmp(opcode,"RESW")!= 0  &&  strcmp(opcode,"RESB")!= 0)
                            printf("\nT^%d^",locctr);
                    }
                }
            }
            else if(flag == 0)
               fprintf(f3,"%s\t%d\n",label,locctr);

        }
        //searching optab for opcode
        flag = search(f2,opcode,opc);
        if(flag == 1){
           //searching symtab for operand
            flag = search(f3,operand,symaddr);
            if(flag == 0){
                printf("%s0000^",opc);
                fprintf(f3,"%s\t0000\n",operand);
                fprintf(f4,"%s\t%d\n",operand,locctr+1);
            }
            else{
                printf("%s%s^",opc,symaddr);
            }
            locctr += 3;
        }
        else if(strcmp(opcode,"WORD")==0){
            locctr += 3;
            printf("00000%s^",operand);
        }
        else if(strcmp(opcode,"RESW")==0){
            locctr += 3*atoi(operand);
        }
        else if(strcmp(opcode,"RESB")==0){
            locctr += atoi(operand);
        }
        else if(strcmp(opcode,"BYTE")==0){
            len=strlen(operand);
            for(int i=2;i<len-1;i++)
              printf("%x",operand[i]);
            printf("^");
            locctr += len- 3;
        }
        fscanf(f1,"%s%s%s",label,opcode,operand);
    }
    printf("\nE^00%d",startadr);
    fclose(f2);
    fclose(f3);
    fclose(f4);
}
