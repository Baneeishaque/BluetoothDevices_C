/*Contact Management Database using a dynamic - single linked list*/

/* Define libraries to be included */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

/* Define Functions*/
void clearInput(void);

void addNewContact(void);

void listAll(void);

void deleteContact(void);

void modifyContact(void);

int findContact(void);

int prompt(void);

int findnum(int);

/* Define Structures*/
typedef struct contact {
    int number;        /*unique account number*/
    char name[20];     /*contains name*/
    char phone[15];    /*contains phone number*/
    char email[20];           /*contains email address*/
    struct contact *next; /*next is used to navigate through structures.*/
    int count;     /*count is used to input comments into array*/
} Contact;

Contact *firstRecord, *currentRecord, *newRecord; /*pointers*/
/* firstRecord is used to point to first record in list, currentRecord points to current record in list, newRecord contains address of new structure/node */

int currentRecordNumber = 0; /*gives unique account numbers*/

/* Main Function */
int main() {

    FILE *datafile;
    char *filename = "contactDatabase.dat";/*declare file name*/
    char ch;
    firstRecord = NULL;

    datafile = fopen(filename, "r");/* open file for reading*/

    if (datafile) {

        firstRecord = (struct contact *) malloc(sizeof(struct contact));
        /*use of malloc to set aside memory relative to size of structure contact*/
        currentRecord = firstRecord;       /*make first record current*/
        while (1) /*endless while loop. a NULL pointer in final node ends loop*/
        {
            newRecord = (struct contact *) malloc(sizeof(struct contact));
            fread(currentRecord, sizeof(struct contact), 1, datafile);
            if (currentRecord->next == NULL)   /* NULL indicates end of node list*/
                break;
            currentRecord->next = newRecord;       /* pointer referencing next node*/
            currentRecord->count = 0;           /* initiates count for comments*/
            currentRecord = newRecord;             /* make current record new*/
        }
        fclose(datafile);                /* close file - good practice to cloe files after use*/
        currentRecordNumber = currentRecord->number;
    }

    do {
        fflush(stdin);
        puts("\nWelcome To The Contact Database");/* print menu messages*/
        puts("-- -----------------------------");
        puts("1 - Add a new contact");
        puts("2 - Delete contact");
        puts("3 - List all contacts");
        puts("4 - Modify contact");
        puts("5 - Find a contact by name");
        puts("-- -----------------------------");
        puts("Q - Save and quit\n");
        printf("\tYour choice:");
        ch = getchar();
        ch = toupper(ch);/*changes user input case to upper case*/
        switch (ch)     /*stores in ch variable.*/
        {
            case '1':
                puts("Add a new contact\n");
                fflush(stdin);
                addNewContact();//call addNewContact function
                break;
            case '2':
                puts("Delete a contact\n");
                deleteContact();
                break;
            case '3':
                puts("List all contacts\n");
                listAll();
                break;
            case '4':
                puts("Modify a contact\n");
                modifyContact();
                break;
            case '5':
                puts("Find a contact by name\n");
                findContact();
                break;
            case 'Q':
                puts("Save and quit\n");
            default:
                break;
        }
    } while (ch != 'Q');

    /*
     * Save the records to disk
     */
    currentRecord = firstRecord;
    if (currentRecord == NULL)
        return (0);        /*no data to write*/

    datafile = fopen(filename, "w");   /*open file to write*/
    if (datafile == NULL) {
        printf("Error writing to %s\n", filename);
        return (1);
    }

    /* Write each record to disk*/
    while (currentRecord != NULL) {
        fwrite(currentRecord, sizeof(struct contact), 1, datafile);
        currentRecord = currentRecord->next;
    }
    fclose(datafile);             /*closes data file*/
    return (0);
}

void addNewContact(void) /* add new contact function*/
{
    newRecord = (struct contact *) malloc(sizeof(struct contact));
    /*allocates memory for new structure.*/

    /*
     * Checks to see whether this is the first record in file
     * If so, then all pointers are initialized to this record,
     */
    if (firstRecord == NULL)
        firstRecord = currentRecord = newRecord;
        /*
         * if not, end of structure list is obtained
         */
    else {
        currentRecord = firstRecord;      /* make the first record the current one*/

        while (currentRecord->next != NULL)currentRecord = currentRecord->next;
        /* and loop through all records*/
        currentRecord->next = newRecord;  /* pointer to next node */
        currentRecord = newRecord;        /* make current record the new one*/
    }

    /* update the structure */
    currentRecordNumber++;
    printf("%27s: %5i\n", "contact number", currentRecordNumber);
    currentRecord->number = currentRecordNumber;    /*currentRecordNumber is used to give unique account numbers*/

    printf("%27s: ", "Enter contact name");
    gets(currentRecord->name);
    printf("%27s: ", "Enter contact Phone number");
    gets(currentRecord->phone);
    printf("%27s: ", "Enter contact email");
    gets(currentRecord->email);
    printf("contact added!");
    currentRecord->count = 0;

    /*
     * gives the new record a NULL pointer
     * to show it's the last record:
     */
    currentRecord->next = NULL;
}

void listAll(void) /* list all contacts function*/
{
    if (firstRecord == NULL)
        puts("There are no contacts to display!"); /*prints message*/

    else {

        printf("%6s %-20s %-15s %-15s\n", "Acct#", "Name", "Phone", "Email");
        puts("------ -------------------- ------------- -------------------");
        /*prints table titles*/
        currentRecord = firstRecord;

        do {

            printf("%6d: %-20s %-15s %-20s\n", \
                currentRecord->number, \
                currentRecord->name, \
                currentRecord->phone, \
                currentRecord->email);
            /*prints values of number, name, phone and email*/
        } while ((currentRecord = currentRecord->next) != NULL);
    }
}

void deleteContact(void)     /*delete contact function */
{
    int record;
    struct contact *previousa;

    if (firstRecord == NULL) {
        puts("There are no contacts to delete!");
        return;
    }

    listAll();        /* show all records*/
    printf("Enter contact account number to delete: ");
    scanf("%d", &record);
    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (currentRecord->number == record) {

            if (currentRecord == firstRecord)    /*if record to be deleted is the first record*/
                firstRecord = currentRecord->next; /*reset firstRecord to point at next record as first*/
            else
                previousa->next = currentRecord->next;/*previous pointer used if record*/

            /*to delete is not the first*/
            free(currentRecord); /*frees memory <deletes>*/
            printf("contact %d deleted!\n", record);
            return;

        } else {

            previousa = currentRecord;
            currentRecord = currentRecord->next;
        }
    }
    printf("contact %d not found!\n", record);
}

void modifyContact(void)   /*modify contact function*/
{
    int record, result;

    if (firstRecord == NULL) {
        puts("There are no contacts to modify!");
        return;
    }

    listAll();        /* show all records */
    printf("Enter contact account number to modify or change: ");
    scanf("%d", &record);  /*scan user input to record*/
    result = findnum(record);

    if (result > 0) {

        printf("Contact %d:\n", currentRecord->number);
        printf("Name: %s\n", currentRecord->name);

        if (prompt())
            gets(currentRecord->name);
        printf("Phone: %s\n", currentRecord->phone);
        if (prompt())
            gets(currentRecord->phone);
        printf("Email: %s\n", currentRecord->email);
        if (prompt())
            gets(currentRecord->email);
        return;
    }
    printf("contact %d was not found!\n", record);
}

int findnum(int recordnum) {

    int record;
    record = recordnum;
    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (currentRecord->number == record) {
            return 1;
        } else {
            currentRecord = currentRecord->next;
        }
    }
    return -1;
}

int findContact(void) /* find contact function*/
{
    char buff[20];

    if (firstRecord == NULL) {
        puts("There are no contacts to find!");
        return 1;
    }

    printf("Enter contact name: ");
    fflush(stdin);/*clears any text from the input stream*/
    gets(buff);

    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (strcmp(currentRecord->name, buff) == 0) {

            printf("%6s %-20s %-15s %-15s\n", "Acct#", "Name", "Phone", "Email");
            /*prints table titles*/
            printf("%6d: %-20s %-15s %-20s\n", \
            currentRecord->number, \
            currentRecord->name, \
            currentRecord->phone, \
            currentRecord->email);
            /*prints values of number, name, phone and email*/
            return 0;

        } else {
            currentRecord = currentRecord->next;
        }
    }

    printf("contact %s was not found!\n", buff);
    return 1;
}

int prompt(void) {

    char ch;

    fflush(stdin);
    printf("Update? (Y to update any other key to not)");
    ch = getchar();
    ch = toupper(ch);
    fflush(stdin);

    if (ch == 'Y') {
        printf("Enter new value: ");
        return (1);
    } else
        return (0);
}

/* END OF PROGRAM */
