/*
    Khylei Neal, Shiv Mali, Hailong Hu
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct phonebook
{

    char name[100]; // first name
    char name2[100]; // last name
    char num[100]; // phone number
}phonebook;

struct phonebook nd[1000];

struct phonebook tmp;


int len;

void read(void);
void save(void);
void print_num_by_name(void);
void print_name_by_num(void);
void print_one(int i);
void print_all(void);
void add_contact(void);
void sort(void);
void menu(void);

int main()
{
    read();
    menu();
}

void read(void)
{
    // read files
    int i=0;
    FILE *fp;
    fp=fopen("phonebookcontacts.txt","r");
    if(fp==NULL)    // return errors if text file is empty
    {
        printf("File opens error!\n");
        exit(0);
    }
    else
    {
        printf("Read file success!\n");
        puts(" ");

        while (fscanf(fp,"%s\t%s\t%s\n",nd[i].name,nd[i].name2,nd[i].num)!= EOF)
        // read till end of the line
        {
            i++;
            len++;
        }
        fclose(fp);   // close file
    }
}

void save(void)
{
    // save the file

    FILE *fp;
    int i;
    fp=fopen("phonebookcontacts.txt","w");
    if(fp==NULL)
    {
        printf("File opens error!");
        exit(0);
    }
    else
    {
        for(i=0;i<len;i++)
        {
            fprintf(fp,"%s\t%s\t%s\n",nd[i].name,nd[i].name2,nd[i].num);
        }
        fclose(fp);
        printf("File saved successfully!\n\n");
    }
}

void print_num_by_name(void)
{

    int i = 0;
    int k = 0;
    char b[100];
    char h[100];

    if (len <= 0)
    {  // if file is empty, return error
        printf("No information researched! Please press enter to continue.\n");
        getch();
        menu();
    }
    else
    {
        printf("Please enter the name: ");
        scanf("%s %s", b, h);
        printf("\nContact information: \n");
        printf("    Name\t\tPhone number\n");
        for(k=0;k<len;k++)
        {
            if(!strcmp(nd[k].name2,h)&&!strcmp(nd[k].name,b))
            {
                print_one(k);
            }
            else
                i++;
        }
        if (i >= len)
            {
                printf("No information researched! Please press enter to continue.\n");
                getch();
                menu();
            }
        else
        {
            printf("Please press enter to continue.\n");
            getch();
            menu();
        }
    }
}

void print_name_by_num(void)
{
    int k=0;
    char b[100];
    if(len <= 0)
    {
        printf("No information researched! Please press enter to continue.\n");
        getch();
        menu();
    }
    else
    {
        printf("\nPlease enter your phone number:\n ");
        scanf("%s",b);
        while(strcmp(nd[k].num,b) && k<len) // compare the input name with the name in the file
        {
            k++;
        }
        if(k>=len) // if there is no name matched
        {
            printf("\nNo information researched!\n");
            printf("Please press enter to continue.\n");
            getch();
            menu();
        }
        else
        {
            printf("\nContact information: \n");
            printf("    Name\t\tPhone number\n");
            print_one(k);
            printf("Please press enter to continue.\n");
            getch();
            menu();
        }
    }
}

void print_one(int i)
{
    printf("%s\t%s\t\t%s\n", nd[i].name, nd[i].name2, nd[i].num);
    puts(" ");
}

void print_all(void)
{
    int i = 0;
    sort();
    printf("    Name\t\t Phone Number\n");
    for (i=0; i < len; i++ )
    { // iterate through the file
        printf("%s\t%s\t\t%s\n", nd[i].name, nd[i].name2, nd[i].num);
    }
    puts(" ");
    printf("Please press enter to continue.\n");
    getch();
    menu();
}
void add_contact(void)
{

    int n;
    int i=0;
    int k=0;
    int m=0;
    char v;

    printf("Please enter the numbe of new contacts to be entered: ");
    scanf("%d", &n);
    puts(" ");
    printf("Please enter the contact information to be entered. \n");
    i = len;
    len = len+n;
    for(k=i; k<len; k++)
    {
        printf("Contact information\n");
        printf("Name: ");
        scanf("%s %s",nd[k].name,nd[k].name2);
        printf("Phone number: ");
        scanf("%s",nd[k].num);
    }
    save();
    printf("\nContact has been added! \n");
    printf("Please press enter to continue.\n");
    getch();
    menu();
}

void sort() // bubble sort
{
    char temp[100]={0};
    int i,j;
    for(i=0;i<len;i++)
    {
        for(j=0;j<len-i;j++)
        {
            if(strcmp(nd[j].name,nd[j+1].name)>0)
            {
                tmp = nd[j];
                nd[j] = nd[j+1];
                nd[j+1] = tmp;
            }
        }
    }
    save();
}

void menu(void)
{
    int f;
    printf("1. Print contact phone number by his/her name.\n");
    printf("2. Print contact name by his/her phone number.\n");
    printf("3. Add new contact.\n");
    printf("4. Edit a contact.\n");
    printf("5. Print all contacts.\n");
    printf("6. Delete a contact.\n");
    printf("7. Exit function.\n");
    puts(" ");
    printf("Please enter a number to indicate the function: ");
    scanf("%d", &f);

    //while(f){
    //if (f >=1 && x <= 7){
    switch (f)
    {
    case 1: print_num_by_name(); break;

    case 2: print_name_by_num(); break;

    case 3: add_contact(); break;

    case 4: NULL;

    case 5: print_all(); break;

    case 6: NULL;

    case 7: exit(0); break;
    }
//}
//}
}
