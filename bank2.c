#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

void admin();
void login();
void Register();
void menu();
void transaction();
void option();
void menuexit();
void balanceInquiry();
void deposit();
void withdrawl();
void search();
void search_acc();
void search_name();
void view();
void add();
void edit();
void del();
void about();
int verify(); // for verifying admin and user
int checkAccNum(int a);
int checkAccBalWithAccNum(int a);
void depsitOrWithdrawl(int IsDeposit);

COORD coord = {0, 0};

int m, r; // m in main and r in record
char inputUserName[50], password[50];

struct record
{
    char name[25];
    int account;
    char phone[15];
    char address[25];
    char email[35];
    char citiz[20];
    double blnc;
    char UserID[10];
} rec;

void gotoxy(int a, int b)
{
    coord.X = a;
    coord.Y = b;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

FILE *fpAllusers, *fpUserCol;

int main()
{

    srand(time(NULL));

    // create blank file if it already doesnt exist.
    if (fopen("UserCol.txt", "r") == NULL)
    {
        fpUserCol = fopen("UserCol.txt", "w");
    }

    if (fopen("collection.txt", "r") == NULL)
    {
        fpAllusers = fopen("collection.txt", "w");
    }

    system("color f4");
    gotoxy(43, 4);
    printf(" WELCOME TO TBC BANKING SYSTEM ");
    gotoxy(50, 8);
    printf("ACCOUNT TYPE");
    gotoxy(44, 10);
    printf("[1] . ADMINISTRATOR ");
    gotoxy(44, 11);
    printf("[2] . USER ");
    gotoxy(44, 14);
    printf("Enter Your Choice .... ");
    scanf("%d", &m);
    if (m != 1 && m != 2)
    {
        system("cls");
        main();
    }
    system("cls");
    admin();
    return 0;
}

void admin()
{
    system("color f4");
    gotoxy(43, 4);
    printf(" WELCOME TO TBC BANKING SYSTEM ");
    int loginOrResigterChoice;
    printf("[1] . Login\n");
    printf("[2] . Register\n");
    printf("Please choose one: ");
    scanf("%d", &loginOrResigterChoice);

    if (loginOrResigterChoice == 2)
    {
        Register();
        admin();
    }
    else
    {
        login();
        if (verify() == 1)
        {
            if (m == 1)
                menu();
            else
                transaction();
        }
        else if (verify() == 0)
        {
            system("CLS");
            gotoxy(43, 16);
            printf("Incorrect Username / Password !!!!");
            admin();
        }
    }

    system("cls");
}

// Takes the user-name and password .
void login()
{
    int i = 0;
    gotoxy(44, 10);
    printf("Enter The Username : ");
    scanf(" %[^\n]", inputUserName);
    gotoxy(44, 12);
    printf("Enter The Password : ");
    scanf(" %[^\n]", password);
}

void Register()
{
    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int accNum, balance, userNameFound = 0;

    printf("Choose an User Name (without any space): \n");
    scanf("%s", currentUserName);

    fpUserCol = fopen("UserCol.txt", "r");
    if (fpUserCol == NULL)
    {
        printf("failed opening file in register.");
        return;
    }
    while (fscanf(fpUserCol, "%s", Username) != EOF)
    {
        if (strcmp(Username, currentUserName) == 0)
        {
            userNameFound = 1;
            break;
        }
    }
    fclose(fpUserCol);

    if (userNameFound)
    {
        printf("User Name already exists. Please try again.\n");
        Register();
    }
    else
    {
        printf("Enter your First Name: ");
        scanf("%s", fname);
        printf("Enter your Last Name: ");
        scanf("%s", lname);
        printf("Create a password: ");
        scanf("%s", pass);
        printf("Initial balance: ");
        scanf("%d", &balance);
        int max = 9999999, min = 1000000;
        accNum = rand() % (max - min + 1) + min;

        fpUserCol = fopen("UserCol.txt", "a");
        if (fpUserCol == NULL)
        {
            printf("file opening failed at Register\n");
            return;
        }
        fprintf(fpUserCol, "%s", currentUserName);
        fclose(fpUserCol);

        fpAllusers = fopen("collection.txt", "a");
        if (fpAllusers == NULL)
        {
            printf("file opening failed at Register\n");
            return;
        }
        fprintf(fpAllusers, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
        fclose(fpAllusers);

        system("cls");
    }
}

// verifies the user-name and password .
int verify()
{
    char a;
    if (m == 1)
    {
        if ((strcmp(inputUserName, "admin") == 0 && strcmp(password, "admin") == 0))
        {
            gotoxy(38, 16);
            printf("You Have Successfully Logged In : \" %s \" ", inputUserName);
            gotoxy(44, 20);
            printf("Press any key to continue .... ");
            getch();
            return 1;
        }
        else
            return 0;
    }
    else if (m == 2)
    {
        char username[50];
        int usernameFound = 0, userFound = 0;
        fpUserCol = fopen("UserCol.txt", "r");
        if (fpUserCol == NULL)
        {
            printf("file opening failed at function verify of user\n");
            return 0;
        }
        while (fscanf(fpUserCol, "%s", username) != EOF)
        {
            if (strcmp(username, inputUserName) == 0)
            {
                usernameFound = 1;
                break;
            }
        }
        fclose(fpUserCol);

        fpAllusers = fopen("collection.txt", "r");
        if (fpAllusers == NULL)
        {
            printf("file opening failed at function verify of user\n");
            return 0;
        }

        char fname[50], lname[50], currentUserName[50], pass[50];
        int accNum, balance;

        while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            if (strcmp(currentUserName, inputUserName) == 0)
            {
                if (strcmp(pass, password) == 0)
                {
                    userFound = 1;
                    break;
                }
            }
        }
        fclose(fpAllusers);

        if (userFound)
        {
            gotoxy(38, 16);
            printf("You Have Successfully Logged In : \" %s \" ", inputUserName);
            gotoxy(44, 20);
            printf("Press any key to continue .... ");
            getch();
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

// main Admin interface.
void menu()
{
    system("CLS");
    gotoxy(48, 4);
    printf("WELCOME TO MAIN MENU");
    gotoxy(44, 8);
    printf("[1] . View Customer Accounts");
    gotoxy(44, 9);
    printf("[2] . Customer Account Registration");
    gotoxy(44, 10);
    printf("[3] . Edit Customer Account");
    gotoxy(44, 11);
    printf("[4] . Delete Customer Account");
    gotoxy(44, 12);
    printf("[5] . Search Customer Account");
    gotoxy(44, 13);
    printf("[6] . Transaction");
    gotoxy(44, 14);
    printf("[7] . Log Out !!! ");
    gotoxy(44, 15);
    printf("[8] . About US ");
    gotoxy(43, 20);
    printf("Please Enter Your Choice [1-8] : ");
    option();
}

// takes user choice and goes to desired function .
void option()
{
    int i;
    scanf("%d", &i);
    system("CLS");
    switch (i)
    {
    case 1:
        view();
        break;
    case 2:
        add();
        break;
    case 3:
        edit();
        break;
    case 4:
        del();
        break;
    case 5:
        search();
    case 6:
        transaction();
        break;
    case 7:
        menuexit();
        break;
    case 8:
        about();
        break;
    default:
        menu();
    }
}

// module for viewing account details
void view()
{
    int i = 7;
    struct record rec;
    FILE *f;
    // rb = reading binary
    f = fopen("record.bin", "rb");
    gotoxy(40, 3);
    printf("CUSTOMERS LIST");
    gotoxy(5, 5);
    printf("A/C No.");
    gotoxy(13, 5);
    printf("Account Name");
    gotoxy(34, 5);
    printf("UserID");
    gotoxy(49, 5);
    printf("Email Address");
    gotoxy(85, 5);
    printf("Phone No.");
    gotoxy(99, 5);
    printf("Balance");
    gotoxy(5, 6);
    // fread() = for reading binary data
    // This reads the contents of a structure variable from the file and stores it in the variable rec.
    // The fread() function will keep returning 1 until there are records in the file.
    // As soon as the EOF is encountered fread() will return a value less than 1 and the condition in the while loop become false and the control comes out of the while loop.
    while (fread(&rec, sizeof(rec), 1, f))
    {
        gotoxy(7, i);
        printf("%d", rec.account);
        gotoxy(13, i);
        printf("%s", rec.name);
        gotoxy(34, i);
        for (r = 0; r < 10; r++)
        {
            printf("%d", rec.UserID[r]);
        }
        gotoxy(49, i);
        printf("%s", rec.email);
        gotoxy(85, i);
        printf("%s", rec.phone);
        gotoxy(99, i);
        printf("$%.2lf", rec.blnc);
        i++;
    }
    // close the file
    fclose(f);
    int x;
    gotoxy(42, i + 5);
    printf("Press [Enter] to return back to main menu. ");
    x = getch();
    // 13 = '\r' i.e carriage return
    if (x == 13)
    {
        menu();
    }
    else
        view();
}

// module for adding accounts.
void add()
{
    char c;
    struct record rec;
    FILE *f;
    // ab = append binary
    f = fopen("record.bin", "ab");
    int i = 0, x;
    // first we evaluate body and check condition
    do
    {
        system("CLS");
        gotoxy(24, 4);
        printf("\t\t\tCUSTOMER ACCOUNT REGISTRATION");
        gotoxy(36, 8);
        printf("[1] . Enter Your Name           : ");
        scanf(" %[^\n]", rec.name);
        gotoxy(36, 9);
        printf("[2] . Enter Your Account Number : ");
        scanf(" %d", &rec.account);
        gotoxy(36, 10);
        printf("[3] . Enter Your Phone Number   : ");
        scanf(" %[^\n]", rec.phone);
        gotoxy(36, 11);
        printf("[4] . Enter Your Address        : ");
        scanf(" %[^\n]", rec.address);
        gotoxy(36, 12);
        printf("[5] . Enter Your E-mail         : ");
        scanf(" %[^\n]", rec.email);
        gotoxy(36, 13);
        printf("[6] . Enter Your Citizenship No.: ");
        scanf(" %[^\n]", rec.citiz);
        gotoxy(36, 14);
        printf("[7] . Enter Amount To Deposit   : $");
        scanf(" %lf", &rec.blnc);
        for (r = 0; r < 10; r++)
        {
            rec.UserID[r] = rand() % 10;
        }
        // fwrite() = for writing binary data
        // This helps to write contents in a structure variable in the file and stores it in the variable rec.
        fwrite(&rec, sizeof(rec), 1, f);
        gotoxy(38, 17);
        printf("CUSTOMER ACCOUNT REGISTRATION SUCCESSFULL");
        i++;
        gotoxy(36, 19);
        printf("Do You Want To Add Another Record ? (Y/N) : ");
        scanf(" %c", &c);
    } while (c == 'Y' || c == 'y');
    fclose(f);
    gotoxy(40, 22);
    printf("Press any key to return back to main menu. ");
    char z = getch();
    if (z == 13)
    {
        menu();
    }
}

// module to check whether the entered account is in the database or not
int checkAccNum(int a)
{

    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int accNum, balance;
    fpAllusers = fopen("collection.txt", "r");
    if (fpAllusers == NULL)
    {
        printf("file opening failed at checkAccNum function\n");
        return 0;
    }
    // check whether we have reached end of file or not
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != 0)
    {
        if (a == accNum)
        {
            fclose(fpAllusers);
            return 1;
        }
    }
    fclose(fpAllusers);
    return 0;
}

int checkAccBalWithAccNum(int a)
{

    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int accNum, balance;
    fpAllusers = fopen("collection.txt", "r");
    if (fpAllusers == NULL)
    {
        printf("file opening failed at checkAccNum function\n");
        return 0;
    }
    // check whether we have reached end of file or not
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != 0)
    {
        if (a == accNum)
        {
            fclose(fpAllusers);
            return balance;
        }
    }
    return 0;
}

// module for editing details.
void edit()
{
    FILE *x, *y;
    int a, c;
// colon i.e ":" acts as id for goto function
re:
    gotoxy(48, 4);
    printf(" EDIT CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Your Account Number To Edit : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        while (fread(&rec, sizeof(rec), 1, x))
        {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
            else if (rec.account == a)
            {
                gotoxy(52, 21);
                printf("Account Number Matched. ");
                gotoxy(51, 10);
                printf("Enter Your Details");
                gotoxy(31, 12);
                printf("[1] . Enter Your Name           : ");
                scanf(" %[^\n]", rec.name);
                gotoxy(31, 13);
                printf("[2] . Enter Your Account Number : ");
                scanf(" %d", &rec.account);
                gotoxy(31, 14);
                printf("[3] . Enter Your Phone Number   : ");
                scanf(" %[^\n]", rec.phone);
                gotoxy(31, 15);
                printf("[4] . Enter Your Address        : ");
                scanf(" %[^\n]", rec.address);
                gotoxy(31, 16);
                printf("[5] . Enter Your E-mail         : ");
                scanf(" %[^\n]", rec.email);
                gotoxy(31, 17);
                printf("[6] . Enter Your Citizenship No : ");
                scanf(" %[^\n]", rec.citiz);
                fwrite(&rec, sizeof(rec), 1, y);
            }
        }
        fclose(x);
        fclose(y);
    }
    if (checkAccNum(a) == 0)
    {
        system("CLS");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. ");
        goto re;
    }
    remove("record.bin");
    rename("new.bin", "record.bin");
    gotoxy(45, 21);
    printf("CUSTOMER ACCOUNT UPDATE SUCCESSFULL");
    gotoxy(42, 24);
    printf("Press any key to return back to main menu . ");
    getch();
    menu();
}

// module for deleting account
void del()
{
    FILE *x, *y;
    int a, c;
re:
    gotoxy(48, 4);
    printf(" DELETE CUSTOMER ACCOUNT ");
    gotoxy(41, 9);
    printf("Enter Your Account Number To Delete : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        while (fread(&rec, sizeof(rec), 1, x))
        {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
        }
        fclose(x);
        fclose(y);
        x = fopen("record.bin", "wb");
        y = fopen("new.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, y))
            fwrite(&rec, sizeof(rec), 1, x);
        fclose(x);
        fclose(y);
    }
    else if (checkAccNum(a) == 0)
    {
        system("CLS");
        gotoxy(51, 15);
        printf("Account Doesn't Exist");
        goto re;
    }
    gotoxy(44, 15);
    printf("CUSTOMER ACCOUNT DELETED SUCCESSFULLY");
    gotoxy(42, 18);
    printf("Press any key to return back to main menu. ");
    getch();
    menu();
}

// module for searching account ( 2 types by number and name )
void search()
{
    system("cls");
    int a;
    gotoxy(55, 4);
    printf(" SEARCH MENU ");
    gotoxy(49, 10);
    printf("[1] . Search By Account ");
    gotoxy(49, 12);
    printf("[2] . Search By Name ");
    gotoxy(47, 17);
    printf("Enter Your Choice [1-2] : ");
    scanf("%d", &a);
    system("cls");
    if (a == 1)
    {
        search_acc();
    }
    else if (a == 2)
    {
        search_name();
    }
    else
        menu();
}

void search_acc()
{
    FILE *f;
    int a, x;
re:
    gotoxy(48, 4);
    printf(" SEARCH CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Your Account Number To Search : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        f = fopen("record.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, f))
        {
            if (rec.account == a)
            {
                gotoxy(52, 21);
                printf("Account Number Matched. User is Active.");
                gotoxy(45, 10);
                printf("Detail Information of %s", strupr(rec.name));
                gotoxy(37, 12);
                printf("[1] . Account Number : %d", rec.account);
                gotoxy(37, 13);
                printf("[2] . Name           : %s", rec.name);
                gotoxy(37, 14);
                printf("[3] . UserID         : ");
                for (r = 0; r < 10; r++)
                {
                    printf("%d", rec.UserID[r]);
                }
                gotoxy(37, 15);
                printf("[4] . Phone Number   : %s", rec.phone);
                gotoxy(37, 16);
                printf("[5] . Address        : %s", rec.address);
                gotoxy(37, 17);
                printf("[6] . E-mail         : %s", rec.email);
                gotoxy(37, 18);
                printf("[7] . Citizenship No : %s", rec.citiz);
                gotoxy(37, 19);
                printf("[8] . Current Balance: $%.2lf", rec.blnc);
                break;
            }
        }
        fclose(f);
    }
    if (checkAccNum(a) == 0)
    {
        system("CLS");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. User is Inactive. ");
        goto re;
    }
    gotoxy(42, 24);
    printf("Press [ENTER] to return back to main menu. ");
    char z = getch();
    if (z == 13)
        menu();
    else
        search();
}

void search_name()
{
    int i = 0, b, x;
    char nam[30];
    gotoxy(48, 4);
    printf(" SEARCH CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Your Full Name To Search : ");
    scanf(" %[^\n]", nam);
    FILE *f;
    f = fopen("record.bin", "rb");
    while (fread(&rec, sizeof(rec), 1, f))
    {
        b = 0;
        strupr(rec.name);
        strupr(nam);
        if (strcmp(rec.name, nam) == 0)
        {
            gotoxy(52, 21);
            printf("Account Number Matched. User is Active.");
            gotoxy(45, 10);
            printf("Detail Information of %s", rec.name);
            gotoxy(37, 12);
            printf("[1] . Account Number : %d", rec.account);
            gotoxy(37, 13);
            printf("[2] . Name           : %s", rec.name);
            gotoxy(37, 14);
            printf("[3] . UserID         : ");
            for (r = 0; r < 10; r++)
            {
                printf("%d", rec.UserID[r]);
            }
            gotoxy(37, 15);
            printf("[4] . Phone Number   : %s", rec.phone);
            gotoxy(37, 16);
            printf("[5] . Address        : %s", rec.address);
            gotoxy(37, 17);
            printf("[6] . E-mail         : %s", rec.email);
            gotoxy(37, 18);
            printf("[7] . Citizenship No : %s", rec.citiz);
            gotoxy(37, 19);
            printf("[8] . Current Balance: $%.2lf", rec.blnc);
            break;
        }
        else
            b = 1;
    }
    fclose(f);
    if (b == 1)
    {
        system("cls");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. User is Inactive.");
        search_name();
    }
    gotoxy(42, 24);
    printf("Press [ENTER] to return back to main menu. ");
    char z = getch();
    if (z == 13)
        menu();
    else
        search();
}

// main user interface.
void transaction()
{
    system("CLS");
    gotoxy(48, 4);
    printf(" TRANSACTION MENU ");
    gotoxy(49, 9);
    printf("[1] . Balance Inquiry");
    gotoxy(49, 10);
    printf("[2] . Cash Deposit");
    gotoxy(49, 11);
    printf("[3] . Cash Withdrawal");
    if (m == 1)
    {
        gotoxy(49, 12);
        printf("[4] . Main Menu");
    }
    else
    {
        gotoxy(49, 12);
        printf("[4] . Exit");
    }
    gotoxy(45, 17);
    printf("Please Enter Your Choice [1-4] : ");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        system("cls");
        balanceInquiry();
        break;
    case 2:
        system("cls");
        depsitOrWithdrawl(1);
        break;
    case 3:
        system("cls");
        depsitOrWithdrawl(0);
        break;
    case 4:
        if (m == 1)
            menu();
        else
            menuexit();
        break;
    default:
        transaction();
    }
}

// module for checking account balance and displaying it
void balanceInquiry()
{
    int a;
    gotoxy(48, 4);
    printf(" BALANCE INQUIRY ");
    gotoxy(47, 12);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        int availableBal = checkAccBalWithAccNum(a);
        system("cls");
        gotoxy(52, 15);
        printf("Your Account balance is %d.\n", availableBal);
    }

    else
    {
        system("cls");
        gotoxy(52, 15);
        printf("Account Doesn't Exist.");
        transaction();
    }
    gotoxy(46, 21);
    printf("Press any key to return back to main menu. ");
    getch();
    transaction();
}

// module for adding or withdrawing amount to a account
void depsitOrWithdrawl(int IsDeposit)
{
    int a,bal;
    gotoxy(48, 4);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    printf("Enter Amount : ");
    scanf("%d", &bal);
    FILE *tmpcollection;
    if (checkAccNum(a) == 1)
    {
        char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
        int accNum, balance, userNameFound = 0;

        tmpcollection = fopen("tmpcollection.txt", "a");
        fpAllusers = fopen("collection.txt", "r");
        if (tmpcollection == NULL || fpAllusers ==NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        //copying from collection to temporary collection while updating new balace
        while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            if (a != accNum)
            {
                fprintf(tmpcollection, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
            }
            else
            {
                // deposit
                if (IsDeposit)
                {
                    fprintf(tmpcollection, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance + bal);
                }

                // withdrawl
                else
                {
                    // check if enough balance is available or not.
                    if (bal > balance)
                    {
                        printf("Insufficient balance.\n");
                        transaction();
                    }
                    else
                    {
                        fprintf(tmpcollection, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance - bal);
                    }
                }
            }
        }

        
        fclose(fpAllusers);

        //clearing all the data from collection.
        fclose(fpAllusers);
        fpAllusers = fopen("collection.txt", "w");
        if (fpAllusers ==NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        fclose(fpAllusers);




        //apending updated data to collection from tmp collection.
        fclose(tmpcollection);
        tmpcollection = fopen("tmpcollection.txt", "r");
        fpAllusers = fopen("collection.txt", "a");
        if (fpAllusers ==NULL || tmpcollection == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        while(fscanf(tmpcollection, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            fprintf(fpAllusers, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
        
        }
        fclose(fpAllusers);
        fclose(tmpcollection);
        if(remove("tmpcollection.txt")!=0) {
            printf("deletion failed at depositOrWithdraw function");
        }
    }
}

void deposit()
{
    int a;
    double b;
    FILE *x, *y;
    gotoxy(54, 4);
    printf(" CASH DEPOSIT ");
    gotoxy(47, 10);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        while (fread(&rec, sizeof(rec), 1, x))
        {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
            else if (rec.account == a)
            {
                rec.name;
                rec.account;
                rec.phone;
                rec.address;
                rec.email;
                rec.citiz;
                gotoxy(45, 13);
                printf("Enter The Amount To Deposit : $ ");
                scanf("%lf", &b);
                rec.blnc += b;
                fwrite(&rec, sizeof(rec), 1, y);
            }
        }
        fclose(x);
        fclose(y);
        x = fopen("record.bin", "wb");
        y = fopen("new.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, y))
            fwrite(&rec, sizeof(rec), 1, x);
        fclose(x);
        fclose(y);
    }
    if (checkAccNum(a) == 0)
    {
        system("CLS");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. ");
        gotoxy(41, 22);
        deposit();
    }
    gotoxy(52, 21);
    printf("CASH DEPOSIT SUCCESSFULL");
    gotoxy(50, 24);
    printf("Press any key to return back...");
    getch();
    transaction();
}

// module to withdraw amount from account
void withdrawl()
{
    FILE *x, *y;
    int a;
    double b, z;
    gotoxy(54, 4);
    printf(" CASH WITHDRAWAL ");
    gotoxy(45, 10);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (checkAccNum(a) == 1)
    {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        gotoxy(42, 13);
        printf("Enter The Amount To Withdraw : $ ");
        scanf("%lf", &b);
        while (fread(&rec, sizeof(rec), 1, x))
        {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
            else if (rec.account == a)
            {
                rec.name;
                rec.account;
                rec.phone;
                rec.address;
                rec.email;
                rec.citiz;
                z = rec.blnc;
                if (b <= rec.blnc)
                {
                    rec.blnc -= b;
                    gotoxy(44, 21);
                    printf("    CASH WITHDRAWAL SUCCESSFULL     ");
                }
                fwrite(&rec, sizeof(rec), 1, y);
            }
        }
        fclose(x);
        fclose(y);
        remove("record.bin");
        rename("new.bin", "record.bin");
        if (z < b)
        {
            system("CLS");
            gotoxy(48, 21);
            printf("Current Balance is $%.2lf", z);
            gotoxy(42, 24);
            printf("Entered amount exceeds current balance");
            withdrawl();
        }
    }
    if (checkAccNum(a) == 0)
    {
        system("CLS");
        gotoxy(50, 21);
        printf("Account Doesn't Exist.");
        gotoxy(39, 22);
        withdrawl();
    }
    gotoxy(42, 24);
    printf("     Press Any Key To Continue ....   ");
    getch();
    transaction();
}

// module for logging out of the program.
void menuexit()
{
    system("cls");
    gotoxy(48, 10);
    printf("!!! THANK YOU !!!");
    gotoxy(50, 12);
    printf("USER :: %s", inputUserName);
    getch();
    gotoxy(0, 26);
    exit(0);
}

void about()
{
    gotoxy(48, 10);
    printf("!!!ABOUT US!!!");
    gotoxy(32, 12);
    printf("This project has been created by Aman Khadka.");
    gotoxy(10, 13);
    printf("It is a Simple Bank Management System Project for my 1st Semester based on C programming language.");
    gotoxy(40, 22);
    printf("Press any key to return back to main menu. ");
    char z = getch();
    if (z == 13)
    {
        menu();
    }
}