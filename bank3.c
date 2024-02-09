#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int checkAccPass(int a, char *password);
void admin();
void login();
void Register();
void menu();
void transaction();
void option();
void menuexit();
void balanceInquiry();
void search();
void view();
void del();
int verify();
int checkAccNum(int a);
int checkAccBalWithAccNum(int a, char *password);
void depsitOrWithdrawl(int IsDeposit);

COORD coord = {0, 0};

int m, r;
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

void moveIt(int a, int b)
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

    system("color 0A");

    moveIt(43, 4);
    printf(" WELCOME TO TBC BANKING SYSTEM ");
    moveIt(50, 8);
    printf("ACCOUNT TYPE");
    moveIt(44, 10);
    printf("1. Admin");
    moveIt(44, 11);
    printf("2. User");
    moveIt(44, 14);
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
    system("color 0A");

    moveIt(43, 4);
    printf("WELCOME TO BBB BANKING SYSTEM \n");
    int loginOrResigterChoice;
    if (m == 2)
    {
        moveIt(43, 6);
        printf("[1] . Login\n");
        moveIt(43, 7);
        printf("[2] . Register\n");
        moveIt(43, 9);
        printf("Please choose one: ");
        scanf("%d", &loginOrResigterChoice);
    }
    else
        loginOrResigterChoice = 1;

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
            moveIt(43, 16);
            printf("Incorrect Username / Password !!!!");
            admin();
        }
    }

    system("cls");
}

void login()
{
    system("cls");
    int i = 0;
    moveIt(44, 10);
    printf("Enter The Username : ");
    scanf(" %[^\n]", inputUserName);
    moveIt(44, 12);
    printf("Enter The Password : ");
    scanf(" %[^\n]", password);
}

void Register()
{
    system("cls");
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
    while (fscanf(fpUserCol, "%s\n", Username) != EOF)
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
        printf("\n\n");
        printf("Press any key to continue....");
        getch();
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
        fprintf(fpUserCol, "%s\n", currentUserName);
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
            moveIt(38, 16);
            printf("You Have Successfully Logged In : \" %s \" ", inputUserName);
            moveIt(44, 20);
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
            moveIt(38, 16);
            printf("You Have Successfully Logged In : \" %s \" ", inputUserName);
            moveIt(44, 20);
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
    moveIt(48, 4);
    printf("WELCOME TO MAIN MENU");
    moveIt(44, 8);
    printf("[1] . View Customer Accounts");
    moveIt(44, 9);
    printf("[2] . Delete Customer Account");
    moveIt(44, 10);
    printf("[3] . Search Customer Account");
    moveIt(44, 11);
    printf("[4] . Transaction");
    moveIt(44, 12);
    printf("[5] . Log Out\n");
    moveIt(44, 20);
    printf("Please Enter Your Choice [1-5] : \n");
    option();
}

// takes user choice and goes to desired function .
void option()
{
    int choice;
    moveIt(44, 21);
    scanf("%d", &choice);
    system("CLS");
    switch (choice)
    {
    case 1:
        view();
        break;
    case 2:
        del();
        break;
    case 3:
        search();
    case 4:
        transaction();
        break;
    case 5:
        menuexit();
        break;
    default:
        menu();
    }
}

// viewing all accounts
void view()
{
    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int accNum, balance;

    fpAllusers = fopen("collection.txt", "r");
    if (fpAllusers == NULL)
    {
        printf("file opening failed at view(admin) function\n");
        return;
    }

    printf(" %-8s %-20s %-49s %-20s\n\n", "A/C", "Username", "Name", "Balance");
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
    {
        printf("%-9d %-20s %-49s %-20d\n", accNum, currentUserName, strcat(strcat(fname, " "), lname), balance);
    }

    fclose(fpAllusers);

    printf("\n\n");
    printf("Press any key to return......");
    getch();
    menu();
}

// check whether the entered account is in the database or not
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
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
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


// check password
int checkAccPass(int a,char *password)
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
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
    {
        if (a==accNum && strcmp(pass, password)==0)
        {
            fclose(fpAllusers);
            return 1;
        }
    }
    fclose(fpAllusers);
    return 0;
}

int checkAccBalWithAccNum(int a, char *password)
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
    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
    {
        if (a == accNum && strcmp(pass, password) == 0)
        {
            fclose(fpAllusers);
            return balance;
        }
    }
    return 0;
}

// delete an account
void del()
{
    int a, userFound = 0;
    char Password[50];
    moveIt(48, 4);
    printf(" DELETE CUSTOMER ACCOUNT ");
    moveIt(41, 9);
    printf("Enter Your Account Number To Delete : ");
    scanf("%d", &a);
    moveIt(41, 10);
    printf("Enter password : ");
    scanf("%s", Password);
    moveIt(48, 4);

    FILE *tmpcollection;
    if (checkAccNum(a) == 1)
    {
        char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
        int accNum, balance;
        userFound = 1;

        tmpcollection = fopen("tmpcollection.txt", "a");
        fpAllusers = fopen("collection.txt", "r");
        if (tmpcollection == NULL || fpAllusers == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }

        // copying all from collection to temporary collection execpt the deleting one.
        while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            if (a != accNum)
            {
                fprintf(tmpcollection, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
            }
        }

        fclose(fpAllusers);

        // clearing all the data from collection.
        fclose(fpAllusers);
        fpAllusers = fopen("collection.txt", "w");
        if (fpAllusers == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        fclose(fpAllusers);

        // apending updated data to collection from tmp collection.
        fclose(tmpcollection);
        tmpcollection = fopen("tmpcollection.txt", "r");
        fpAllusers = fopen("collection.txt", "a");
        if (fpAllusers == NULL || tmpcollection == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        while (fscanf(tmpcollection, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            fprintf(fpAllusers, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
        }
        fclose(fpAllusers);
        fclose(tmpcollection);
        if (remove("tmpcollection.txt") != 0)
        {
            printf("deletion of tmpcollection failed at del function");
        }

        moveIt(44, 15);
        printf("CUSTOMER ACCOUNT DELETED SUCCESSFULLY");
    }
    else
    {
        system("cls");
        moveIt(42, 8);
        printf("Account doesn't exist.\n");
    }

    moveIt(42, 18);
    printf("Press any key to return....... ");
    getch();
    menu();
}

// search account by username or account number
void search()
{
    char username[50];
    int a, accNum, balance;
    moveIt(42, 4);
    printf("Please provide either of the two:\n");
    moveIt(44, 8);
    printf("1. Username: \n");
    moveIt(44, 9);
    printf("2. Account Number: \n");
    moveIt(44, 11);
    printf("Choose one: ");
    int choice;
    moveIt(56, 11);
    scanf("%d", &choice);
    if (choice == 1)
    {
        system("cls");
        moveIt(44, 8);
        printf("Username of the account holder: ");
        scanf("%s", username);
    }
    else if (choice == 2)
    {
        system("cls");
        moveIt(44, 8);
        printf("Account number of the holder: ");
        scanf("%d", &a);
    }
    else
    {
        system("cls");
        moveIt(44, 8);
        printf("Invalid input.\n");
        search();
    }

    system("cls");

    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int userFound = 0;

    fpAllusers = fopen("collection.txt", "r");
    if (fpAllusers == NULL)
    {
        moveIt(44, 12);
        printf("file opening failed at view(admin) function\n");
        return;
    }

    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
    {
        if (strcmp(username, currentUserName) == 0 || a == accNum)
        {
            moveIt(4, 4);
            printf(" %-8s %-20s %-49s %-20s\n\n", "A/C", "Username", "Name", "Balance");
            moveIt(4, 5);
            printf("%-9d %-20s %-49s %-20d\n", accNum, currentUserName, strcat(strcat(fname, " "), lname), balance);
            userFound = 1;
        }
    }

    if (!userFound)
    {
        moveIt(46, 8);
        printf("User doesn't exist.\n");
    }

    fclose(fpAllusers);

    printf("\n\n");
    moveIt(46, 12);
    printf("Press any key to return......");
    getch();
    menu();
}

// main user interface.
void transaction()
{
    system("CLS");
    moveIt(48, 4);
    printf(" TRANSACTION MENU ");
    moveIt(49, 9);
    printf("[1] . Balance Inquiry");
    moveIt(49, 10);
    printf("[2] . Cash Deposit");
    moveIt(49, 11);
    printf("[3] . Cash Withdrawal");
    if (m == 1)
    {
        moveIt(49, 12);
        printf("[4] . Main Menu");
    }
    else
    {
        moveIt(49, 12);
        printf("[4] . Exit");
    }
    moveIt(45, 17);
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
    transaction();
}

// check account balance and display it
void balanceInquiry()
{
    int a;
    char Password[50];
    moveIt(48, 4);
    printf(" BALANCE INQUIRY ");
    moveIt(47, 12);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    moveIt(47, 13);
    printf("Enter Password : ");
    scanf("%s", &Password);
    if (checkAccNum(a) == 1)
    {
        int availableBal = checkAccBalWithAccNum(a, Password);
        system("cls");
        moveIt(44, 10);
        printf("Your Account balance is %d.\n", availableBal);
    }

    else
    {
        system("cls");
        moveIt(44, 10);
        printf("Account Doesn't Exist.");
        transaction();
    }
    moveIt(42, 18);
    printf("Press any key to return back to main menu. ");
    getch();
    transaction();
}

//  adding or withdrawing amount to a account
void depsitOrWithdrawl(int IsDeposit)
{
    int a, bal, isValidAcc;
    char Password[50];
    moveIt(48, 4);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    printf("Enter Amount : ");
    scanf("%d", &bal);
    if (!IsDeposit)
    {
        printf("Enter password:");
        scanf("%s", Password);
        isValidAcc = checkAccPass(a,Password);
    } else isValidAcc = checkAccNum(a);
    FILE *tmpcollection;
    if (isValidAcc)
    {
        char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
        int accNum, balance, userNameFound = 0;

        tmpcollection = fopen("tmpcollection.txt", "a");
        fpAllusers = fopen("collection.txt", "r");
        if (tmpcollection == NULL || fpAllusers == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        // copying from collection to temporary collection while updating new balace
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

        // clearing all the data from collection.
        fclose(fpAllusers);
        fpAllusers = fopen("collection.txt", "w");
        if (fpAllusers == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        fclose(fpAllusers);

        // apending updated data to collection from tmp collection.
        fclose(tmpcollection);
        tmpcollection = fopen("tmpcollection.txt", "r");
        fpAllusers = fopen("collection.txt", "a");
        if (fpAllusers == NULL || tmpcollection == NULL)
        {
            printf("file opening failed at depositOrWithdrawl function.\n");
            return;
        }
        while (fscanf(tmpcollection, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
        {
            fprintf(fpAllusers, "%d %s %s %s %s %d\n", accNum, currentUserName, fname, lname, pass, balance);
        }
        fclose(fpAllusers);
        fclose(tmpcollection);
        if (remove("tmpcollection.txt") != 0)
        {
            printf("deletion failed at depositOrWithdraw function");
        }

        if (IsDeposit)
            printf("Deposited successfully.\n");
        else
            printf("Withdrawn successfully.\n");
        moveIt(42, 18);
        printf("Press any key to return...... ");
        getch();
        transaction();
    }
    else
    {
        printf("Invalid Info.\n");
        moveIt(42, 18);
        printf("Press any key to return...... ");
        getch();
        transaction();
    }
}

// logging out of the program.
void menuexit()
{
    system("cls");
    moveIt(48, 10);
    printf("Thank You for being with us.");
    moveIt(48, 12);
    printf("User :: %s", inputUserName);
    getch();
    moveIt(0, 26);
    exit(0);
}