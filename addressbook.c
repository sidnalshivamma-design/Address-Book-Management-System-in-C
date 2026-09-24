#include "addressbook.h"
int search_flag = 0;
/* Reads contact details from the CSV file into the address book */
void pull_file_data(struct AddressBook *addressBook, char *file)
{
    FILE *fp=fopen(file, "r");//open in read mode
    if (fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    addressBook->contactCount = 0;
    while (fscanf(fp,"%49[^,],%14[^,],%49[^\n]\n",addressBook->contacts[addressBook->contactCount].name,addressBook->contacts[addressBook->contactCount].phone,
			    addressBook->contacts[addressBook->contactCount].mail) == 3)// upon succesful scanning fscanf returns 3 so we are checking ==3
    {
	    addressBook->contactCount++;
    }
    fclose(fp);
}
/* Saves all contact details from the address book into the CSV file */
void push_data_file(struct AddressBook *addressBook, char *file)
{
    FILE *fp=fopen(file, "w");
    int i;
    if (fp == NULL)
    {
        printf("File cannot be opened\n");
        return;
    }
    for (i = 0; i < addressBook->contactCount; i++)
        fprintf(fp, "%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].mail);
    fclose(fp);
}
/* Adds a new contact after validating name, phone and email */
void add_contact(struct AddressBook *addressBook)
{
	if (addressBook->contactCount >= 100)
	{
           printf("Address book is full\n");
           return;
        }
    while(1)
    {
	printf("Enter name : ");
	scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].name);
	if(name_verify(addressBook->contacts[addressBook->contactCount].name))
		break;
        printf("Enter valid name\n");
    }
    while (1)
    {
        printf("Enter phone : ");
        scanf("%s", addressBook->contacts[addressBook->contactCount].phone);
        if (mobile_verify(addressBook->contacts[addressBook->contactCount].phone))
            break;
        printf("Enter valid mobile number\n");
    }
    while (1)
    {
        printf("Enter email : ");
        scanf("%s", addressBook->contacts[addressBook->contactCount].mail);
        if (gmail_verify(addressBook->contacts[addressBook->contactCount].mail))
		break;
        printf("Enter valid Gmail ID\n");
    }
    addressBook->contactCount++;
    printf("Contact added successfully\n");
}
/* Validates the contact name */
int name_verify(char name[])
{
	int i;
	if(strlen(name)<4)
	{
		printf("Name must contain at least 4 characters\n");
		return 0;
	}
	for(int i=0;name[i]!='\0';i++)
	{
		if((name[i]<'A'||name[i]>'Z')&& (name[i]<'a'||name[i]>'z'))
		{
			printf("Name should contain only alphabets\n");
			return 0;
		}
	}
	return 1;
}
/* Validates the mobile number */
int mobile_verify(char mobile[])
{
    int i;
    if (strlen(mobile) != 10)
    {
	    printf("Phone number must contain exactly 10 digits\n");
	    return 0;
    }
    if (mobile[0] < '6' || mobile[0] > '9')
    {
	    printf("First digit must be between 6 and 9\n");
	    return 0;
    }
    for (i = 0; i < 10; i++)
    {
        if (mobile[i] < '0' || mobile[i] > '9')
	{
		printf("Symbols are not allowed in phone number\n");
		return 0;
	}
    }
    return 1;
}
/* Validates the Gmail ID */
int gmail_verify(char gmail[])
{
    char *ptr = strstr(gmail, "@");
    if (ptr == NULL)
    {
        printf("Email ID must contain @ symbol\n");
        return 0;
    }
    if (strstr(ptr + 1, "@") != NULL)
    {
        printf("Multiple @ symbols are not allowed\n");
        return 0;
    }
    if (strchr(gmail, '.') == NULL)
    {
        printf("Email ID must contain . (dot)\n");
        return 0;
    }
    if (strchr(gmail, '.') < ptr)
    {
        printf("The . (dot) must appear after @\n");
        return 0;
    }
    if (ptr[1] == '.')
    {
        printf("There must be at least one character between @ and .\n");
        return 0;
    }
    if (strcmp(ptr, "@gmail.com") != 0)
    {
        printf("extra characters after the domain\n");
        return 0;
    }
    return 1;
}
/* Edits the details of an existing contact */
void edit_contact(struct AddressBook *addressBook)
{
    int i, choice, valid;
    char name[50];
    printf("Enter the name of the contact to edit: ");
    scanf(" %[^\n]", name);
    for (i = 0; i < addressBook->contactCount; i++)
    {

       if (strcmp(addressBook->contacts[i].name, name) == 0)
        {
	   while(1)
	   {
            printf("What do you want to edit?\n");
            printf("1. Name\n");
            printf("2. Phone\n");
            printf("3. Email\n");
            printf("4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice)
            {
                case 1:
		    do
		    {
                    printf("Enter new name: ");
                    scanf(" %[^\n]", addressBook->contacts[i].name);
		    valid=name_verify(addressBook->contacts[i].name);
		    if(!valid)
			    printf("Enter valid name\n");
	            } while(!valid);
                    printf("Contact updated successfully.\n");
                    break;
                case 2:
                    do
                    {
                        printf("Enter new phone: ");
                        scanf("%s", addressBook->contacts[i].phone);
                        valid = mobile_verify(addressBook->contacts[i].phone);
                        if (!valid)
                            printf("Enter valid mobile number\n");
                    } while (!valid);
                    printf("Contact updated successfully.\n");
                    break;
                case 3:
                    do
                    {
                        printf("Enter new email: ");
                        scanf("%s", addressBook->contacts[i].mail);
                        valid = gmail_verify(addressBook->contacts[i].mail);
                        if (!valid)
                            printf("Enter valid Gmail ID\n");
                    } while (!valid);
                    printf("Contact updated successfully.\n");
                    break;
                case 4:
                    printf("Exiting...\n");
                    return;
                default:
                    printf("Invalid choice\n");
                    return;
            }
	}
            return;
        }
    }
    printf("Contact not found\n");
}
/* Displays the search menu and searches for a contact */
void search_contact(struct AddressBook *addressBook)
{
    int choice;
    char name[50];
    char phone[15];
    char gmail[50];
    while(1)
    {
    search_flag = 0;
    addressBook->ir_size = 0;
    printf("\nSearch by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
            printf("Enter name to search : ");
            scanf(" %[^\n]", name);
            search_by_name(addressBook, name);
            break;
        case 2:
            printf("Enter phone to search : ");
            scanf("%s", phone);
            search_by_phone(addressBook, phone);
            break;
        case 3:
            printf("Enter email to search : ");
            scanf("%s", gmail);
            search_by_gmail(addressBook, gmail);
            break;
        case 4:
	    printf("Exiting...\n");
            return;
	default:
            printf("Invalid choice\n");
    }
   }
}
/* Searches for contacts using their name */
void search_by_name(struct AddressBook *addressBook, char name[])
{
    int i;
    int serial_no = 0;
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, name) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;
            search_flag = 1;
            serial_no++;
            printf("\n%d.%s ,%s ,%s\n",serial_no,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].mail);
        }
    }
    if (search_flag == 0)
        printf("Contact not found\n");
}
/* Searches for contacts using their phone number */
void search_by_phone(struct AddressBook *addressBook, char phone[])
{
    int i;
    int serial_no = 0;
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;
            search_flag = 1;
            serial_no++;
            printf("\n%d.%s ,%s ,%s\n",serial_no,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].mail);
        }
    }
    if (search_flag == 0)
        printf("Contact not found\n");
}
/* Searches for contacts using their Gmail ID */
void search_by_gmail(struct AddressBook *addressBook, char gmail[])
{
    int i;
    int serial_no = 0;
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].mail, gmail) == 0)
        {
            addressBook->index_record[addressBook->ir_size] = i;
            addressBook->ir_size++;
            search_flag = 1;
            serial_no++;
            printf("\n%d.%s ,%s ,%s\n",serial_no,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].mail);
        }
    }
    if (search_flag == 0)
        printf("Contact not found\n");
}
/* Deletes a contact after searching and getting confirmation */
void delete_contact(struct AddressBook *addressBook)
{
    int i, j;
    int choice, serial;
    char name[50];
    char phone[15];
    char mail[50];
    char opt;
    while (1)
    {
        printf("\n----- Delete Contact -----\n");
        printf("1. Delete by Name\n");
        printf("2. Delete by Phone\n");
        printf("3. Delete by Email\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        search_flag = 0;
        addressBook->ir_size = 0;
        switch (choice)
        {
            case 1:
                printf("Enter name to delete: ");
                scanf(" %[^\n]", name);
                search_by_name(addressBook, name);
                break;
            case 2:
                printf("Enter phone number to delete: ");
                scanf("%s", phone);
                search_by_phone(addressBook, phone);
                break;
            case 3:
                printf("Enter email ID to delete: ");
                scanf("%s", mail);
                search_by_gmail(addressBook, mail);
                break;
            case 4:
		printf("Exiting\n");
                return;
            default:
                printf("Invalid choice\n");
                continue;
        }
        if (search_flag == 0)
            continue;
        if (addressBook->ir_size > 1)
        {
            printf("\nEnter serial number of contact to delete: ");
            scanf("%d", &serial);
            if (serial < 1 || serial > addressBook->ir_size)
            {
                printf("Invalid serial number\n");
                continue;
            }
            i = addressBook->index_record[serial - 1];
            printf("\nSelected Contact:\n");
            printf("Name  : %s\n", addressBook->contacts[i].name);
            printf("Phone : %s\n", addressBook->contacts[i].phone);
            printf("Email : %s\n", addressBook->contacts[i].mail);
        }
        else
            i = addressBook->index_record[0];
        printf("\nAre you sure you want to delete this contact? (y/n): ");
        scanf(" %c", &opt);
        if (opt == 'n')
        {
            printf("Deletion Cancelled\n");
            continue;
        }
        else if (opt == 'y')
        {
            for (j = i; j < addressBook->contactCount - 1; j++)
            {
                addressBook->contacts[j] =
                    addressBook->contacts[j + 1];
            }
            addressBook->contactCount--;
            printf("Contact deleted successfully\n");
        }
        else
	{
            printf("Invalid choice. Please enter a valid option.\n");
            continue;
	}
	return;
    }
}
/* Displays all contacts in a formatted list */
void list_contacts(struct AddressBook *addressBook)
{
    int i;
    printf("\n================================ CONTACT LIST =================================\n");
    printf("| S.No | Name                    | Phone Number       | Email ID              |\n");
    printf("--------------------------------------------------------------------------------\n");
    for(i = 0; i < addressBook->contactCount; i++)
        printf("| %-4d | %-23s | %-18s | %-21s |\n",i + 1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].mail);
    printf("--------------------------------------------------------------------------------\n");
    printf("Total Contacts: %d\n", addressBook->contactCount);
}















