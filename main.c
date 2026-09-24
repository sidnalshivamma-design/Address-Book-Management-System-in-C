#include "addressbook.h"
int main()
{
    struct AddressBook addressBook;
    int choice;
    addressBook.contactCount = 0;
    /* Read contacts from database.csv */
    pull_file_data(&addressBook, "database.csv");
    do
    {
        printf("\nAddress Book Menu\n");
        printf("1. Add contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                add_contact(&addressBook);
                break;
            case 2:
                search_contact(&addressBook);
                break;
            case 3:
                edit_contact(&addressBook);
                break;
            case 4:
                delete_contact(&addressBook);
                break;
            case 5:
                list_contacts(&addressBook);
                break;
            case 6:
                push_data_file(&addressBook, "database.csv");
                printf("Saving and Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    }
    while (choice != 6);
    return 0;
}






