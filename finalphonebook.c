/*
    Final Project: Phonebook
    In this program, we worked together as a group to make an interactive, formatted phone book, that will allow you to search, edit, add, and print different contacts

    Authors: Khylei, Shiv, Hu 

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

typedef struct phonebook{
// Information inside of the phone book
// define a new data typr, then use the data type to define structure variables directly as follows
    char name[100]; // first name
    char name2[100]; // last name
    char num[100]; // phone number
}phonebook;

struct phonebook contact[1000]; // Contact

struct phonebook tmp; // Temporary variable for sorting function

int len; // Keeping track of the length of the file

void read(void);
void save(void);
void print_num_by_name(void);
void print_name_by_num(void);
void print_one(int i);
void edit_contact();
void delete_contact(void);
void print_all(void);
void add_contact(void);
void sort(void);
void menu(void);

int main(){

	read();
    menu();

}

void read(void){    // read files
    // This function simply just reads over the contact text file and ensures that information is present from the contact list

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

		while (fscanf(fp,"%s\t%s\t%s\n",contact[i].name,contact[i].name2,contact[i].num)!= EOF)    // read till end of the line
		{
			i++;
			len++;
		 }
			  fclose(fp);   // close file
	}

}

void save(void){    // save the file
    // This function will rewrite file and save the information given to ensure that the current information held in the structure strings are saved
	FILE *fp;
	int i;
	fp=fopen("phonebookcontacts.txt","w"); // writing to the text file
	if(fp==NULL)
	{
		printf("File opens error!"); // Empty file, or error while opening

		exit(0);

	}else
	{
		for(i=0;i<len;i++) // Loop until the entire length of the file is written
		{
			fprintf(fp,"%s\t%s\t%s\n",contact[i].name,contact[i].name2,contact[i].num);  //rewriting kinda
		}
			fclose(fp);
			printf("File saved successfully!\n\n");
	}
}

void print_num_by_name(void)
    /*
        This function will take in the name being searched and compare it against the current names
        in the file, if a match is found then the file will print the contacts with the matching name
    */
{

	int i = 0;
    int k = 0;
    char fname[100]; // First name
    char lname[100]; // Last name

    if (len <= 0){  // if file is empty, return error

        printf("No information researched! Please press ENTER to continue.\n");

		getch();
		system("cls");
        menu();

    }
    else{

        printf("Please enter the name: ");
        scanf("%s %s", fname, lname);

		printf("--------------------------------------\n");
        printf("Contact Name      |   Phone Number   |\n");
        printf("-------------------------------------|\n");
        for(k=0;k<len;k++) // Loop through the length of the file
		{
			if(!strcmp(contact[k].name,fname)&&!strcmp(contact[k].name2,lname)) // Checking the input name and name in the file
			{
				print_one(k);

            }
            else
                i++; // Keeping track of the lines in the file
		}
        if (i >= len){ // when I becomes greater than the length of the file no information is found if a match is found i will never be greater than the length of the file

            printf("\n No information researched! Please press ENTER to continue.\n");
			getch();
			system("cls"); // Clears screen
			menu();
        }
        else{

			printf("Please press ENTER to continue.\n");
			getch();
			system("cls"); // Clear
        	menu();
        }
    }

	}


void print_name_by_num(void)
        /*
        This function will take in the number being searched and compare it against the current numbers
        in the file, if a match is found then the file will print the contacts with the matching number
        */
{
    int k=0;
    char pNum[100]; // Input Phone number
    if(len <= 0){
        printf("No information researched! Please press ENTER to continue.\n");

		getch();
		system("cls");
        menu();

 }
    else{
        printf("\nPlease enter your phone number:\n ");
        scanf("%s",pNum);
		while(strcmp(contact[k].num,pNum) && k<len)	// compare the input phone number with the number in the file
		{
			k++; // Increment until the end of the file is reached

		}
		if(k>=len)	// if there is no number matched
		{
		    printf("\nNo information researched!\n");
			printf("Please press ENTER to continue.\n");

			getch();
			system("cls");
			menu();
		}
		else
		{
			printf("\nContact information: \n");
			printf("--------------------------------------\n");
            printf("Contact Name      |   Phone Number   |\n");
            printf("-------------------------------------|\n");
			print_one(k);

			printf("Please press ENTER to continue.\n");

			getch();
			system("cls");
			menu();
		}
	 }
}

void print_one(int i)
        // Simple function that will format the needed contacts
{

    printf("%s %s\t  |   %s   |\n", contact[i].name, contact[i].name2, contact[i].num);
    printf("-------------------------------------|\n");
    puts(" ");
}

void print_all(void)
        // Printing all the contacts within the contact list
{

    int i = 0;
    sort();
    printf("--------------------------------------\n");
    printf("Contact Name      |   Phone Number   |\n");
    printf("-------------------------------------|\n");


    for (i=0; i < len; i++ ){	// iterate through the file

        printf("%s %s\t  |   %s   |\n", contact[i].name, contact[i].name2, contact[i].num);
        printf("-------------------------------------|\n");

    }

	puts(" ");
	printf("Please press ENTER to continue.\n");
	getch();
    menu();
}

void add_contact(void)
    /*
        This function will take in the number of contacts to be added at one time, then ask for the contact information for the added contacts,
        These contacts are then added to struct arrays and saved to the phone book file,
    */
{

	int n; // Number of new contacts needed to be add
	int temp=0; // Holder variable
	int k=0;

	printf("Please enter the number of new contacts to be entered: ");
	scanf("%d", &n);

	puts(" ");
	printf("Please enter the contact information to be entered. \n");
	temp = len;
	len = len+n;


	for(k=temp; k<len; k++){

	    printf("Contact information\n");
		printf("Name: ");
	    scanf("%s %s",contact[k].name,contact[k].name2); // Saving to the structure arrays to be saved in our file

        contact[k].name[0]= toupper(contact[k].name[0]); // changes the first letter to uppercase
        contact[k].name2[0]= toupper(contact[k].name2[0]);
		printf("Phone number: ");
		scanf("%s",contact[k].num);

}

	save();

	printf("\nContact has been added! \n");

	printf("Please press ENTER to continue.\n");
	getch();
	system("cls");
	menu();
}

void sort(void)    // bubble sort
{
    //char temp[100]={0};
    int i,j; // i is compared down the line of names which is j

    for(i=0;i<len;i++) // i is being compared to the rest of the file
    {
        for(j=i+1;j<len;j++)
        {
            int fcmp = strcmp(contact[i].name,contact[j].name); // First name compared
            int lcmp = strcmp(contact[i].name2,contact[j].name2); // Last name compared
            if((fcmp > 0) || ((fcmp == 0 ) && (lcmp > 0))) //
            {
                    tmp = contact[j];
                    contact[j] = contact[i];
                    contact[i] = tmp;
            }
        }
    }
    save();
}
void edit_contact() //void
    /*
        This function will read in the contact name that is wanted to be edited, then loops through and compares the contacts name with the other contacts in the file.
        If the contact is found it will take in the new contact information
    */
{
    printf("Please enter the name of the contact to edit: ");
    char name[100], name2[100]; // user input name that you want to change
    scanf("%s %s", name, name2);
    int index;
    int i = 0; // count lines
    for(index = 0; index < len;  index++)
    {
         if(strcmp(name, contact[index].name) == 0 && strcmp(name2, contact[index].name2) == 0){

         /*else{
            printf("No information researched. Please press ENTER to continue\n");
            getch();
            system("cls");
            menu();}   */

            printf("\nEnter the new first and last name: ");
            char name[100], name2[100];  //new name
            scanf("%s %s", contact[index].name, contact[index].name2);
            contact[index].name[0]= toupper(contact[index].name[0]); // changes the first letter to uppercase
            contact[index].name2[0]= toupper(contact[index].name2[0]);
            printf("\n Enter the new phone number: ");
            char num[100];  //new number
            scanf("%s", contact[index].num);
            }

        else{

            i++;
        }

    }

    if(i >= len){

        printf("No information researched. Please press ENTER to continue\n");
        getch();
        system("cls");
        menu();
    }

    else{

        save();

        printf("\nContact has been edited! \n");

        printf("Please press ENTER to continue.\n");
        getch();
        system("cls");
        menu();

    }
}


void delete_contact(void)
    /*
        This function will get the name of the contact that is being deleted and compares them with the contacts in the file,
        when the name is matching it will start at the ith line of the file where the match is found and iterate to the end of the file to delete the contact
    */
{

    int error = 0;	// flag variable

    char fname[100];
	char lname[100];
	printf("Please enter the contact name you want to delete: ");
    scanf("%s %s",&fname,&lname);
    for (int i = 0; i < len; i++)
    {
        if (!strcmp(contact[i].name, fname) && !strcmp(contact[i].name2, lname))	// if input name is matched
        {
            for (int j = i; j < len - 1;j ++)	// iteration start with the input name, end with length - 1
            {
                error = 1;
                contact[j] = contact[j + 1];	// cover the input name with the name after it

            }
            len = len - 1; // Updating the file length tracker
            printf("Delete sucessfully!\n");
			save();
			printf("Please press ENTER to continue.\n");
			getch();
			//system("cls");
			menu();

            break;
        }

    }
    if (!error)
    {
        printf("Contact doesn't exist");
		printf("Please press ENTER to continue.\n");
		getch();
		//system("cls");
		menu();
    }

}

void menu(void)
    // Menu for the different options available in the program
{

	int f;

    printf("\t\t\t\t\t\t\t\t-------------------------------------------------------------\n");
    printf("\t\t\t\t\t\t\t\t|               1. Print all contacts.                       |\n\n");
    printf("\t\t\t\t\t\t\t\t|               2. Print phone number by his/her name.       |\n\n");
    printf("\t\t\t\t\t\t\t\t|               3. Print name by his/her phone number.       |\n\n");
    printf("\t\t\t\t\t\t\t\t|               4. Add new contact.                          |\n\n");
    printf("\t\t\t\t\t\t\t\t|               5. Edit a contact.                           |\n\n");
	printf("\t\t\t\t\t\t\t\t|               6. Delete a contact.                         |\n\n");
	printf("\t\t\t\t\t\t\t\t|               7. Exit function.                            |\n");
	printf("\t\t\t\t\t\t\t\t-------------------------------------------------------------\n");
    puts(" ");
    printf("Please enter a number to indicate the function: ");
	scanf("%d", &f);

	system("cls");

	//while(f){
		//if (f >=1 && x <= 7){
			switch (f){
			    case 1: print_all(); break;

				case 2: print_num_by_name(); break;

				case 3: print_name_by_num(); break;

				case 4: add_contact(); break;

                case 5: edit_contact(); break;

				case 6: delete_contact(); break;

				case 7: exit(0); break;


			}
	//}
	//}
}

