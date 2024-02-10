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
void getPassword(char *pass);

int m, r;
char inputUserName[50], password[50];
FILE *fpAllusers, *fpUserCol;

int main()
{
    srand(time(NULL));

    // create blank file if it already doesnt exist.
    if (fopen("UserCol.txt", "r") == NULL)
        fpUserCol = fopen("UserCol.txt", "w");
    if (fopen("collection.txt", "r") == NULL)
        fpAllusers = fopen("collection.txt", "w");

    system("color 0A");
    printf("\n\n WELCOME TO TBC BANKING SYSTEM \n\n");
    printf("ACCOUNT TYPE\n");
    printf("1. Admin\n");
    printf("2. User\n");
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
    printf("WELCOME TO BBB BANKING SYSTEM \n");
    int loginOrResigterChoice;
    if (m == 2)
    {
        printf("[1] . Login\n");
        printf("[2] . Register\n");
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
    printf("Enter Username : ");
    scanf(" %[^\n]", inputUserName);
    printf("Enter Password : ");
    // scanf(" %[^\n]", password);
    getPassword(password);
    printf("\n");
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
        printf("\n");
        printf("Enter your Last Name: ");
        scanf("%s", lname);
        printf("\n");
        printf("Create a password: ");
        // scanf("%s", pass);
        getPassword(password);
        printf("\n");
        printf("Initial balance: ");
        scanf("%d", &balance);
        printf("\n");
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
            printf("You Have Successfully Logged In : \" %s \" \n", inputUserName);
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
            printf("You Have Successfully Logged In : \" %s \" \n", inputUserName);
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
    printf("WELCOME TO MAIN MENU\n\n");
    printf("[1] . View Customer Accounts\n");
    printf("[2] . Delete Customer Account\n");
    printf("[3] . Search Customer Account\n");
    printf("[4] . Transaction\n");
    printf("[5] . Log Out\n");
    printf("Please Enter Your Choice [1-5] : ");
    option();
}

// takes user choice and goes to desired function .
void option()
{
    int choice;
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
int checkAccPass(int a, char *password)
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
    printf(" DELETE CUSTOMER ACCOUNT \n");
    printf("Enter Your Account Number To Delete : ");
    scanf("%d", &a);
    printf("\n");
    printf("Enter password : ");
    // scanf("%s", Password);
    getPassword(password);
    printf("\n");

    FILE *tmpcollection;
    if (checkAccPass(a,password) == 1)
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
            printf("deletion of tmpcollection failed at del function");

        printf("CUSTOMER ACCOUNT DELETED SUCCESSFULLY");
    }
    else
    {
        system("cls");
        printf("Invalid info.\n");
    }
    printf("Press any key to return....... ");
    getch();
    menu();
}

// search account by username or account number
void search()
{
    char username[50];
    int a, accNum, balance;
    printf("Please provide either of the two:\n");
    printf("1. Username: \n");
    printf("2. Account Number: \n");
    printf("Choose one: ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        system("cls");
        printf("Username of the account holder: ");
        scanf("%s", username);
    }
    else if (choice == 2)
    {
        system("cls");
        printf("Account number of the holder: ");
        scanf("%d", &a);
    }
    else
    {
        system("cls");
        printf("Invalid input.\n");
        search();
    }
    system("cls");
    char fname[50], lname[50], currentUserName[50], pass[50], Username[50];
    int userFound = 0;

    fpAllusers = fopen("collection.txt", "r");
    if (fpAllusers == NULL)
    {
        printf("file opening failed at view(admin) function\n");
        return;
    }

    while (fscanf(fpAllusers, "%d %s %s %s %s %d\n", &accNum, currentUserName, fname, lname, pass, &balance) != EOF)
    {
        if (strcmp(username, currentUserName) == 0 || a == accNum)
        {
            printf(" %-8s %-20s %-49s %-20s\n\n", "A/C", "Username", "Name", "Balance");
            printf("%-9d %-20s %-49s %-20d\n", accNum, currentUserName, strcat(strcat(fname, " "), lname), balance);
            userFound = 1;
        }
    }

    if (!userFound)
    {
        printf("User doesn't exist.\n");
    }
    fclose(fpAllusers);

    printf("\n\n");
    printf("Press any key to return......");
    getch();
    menu();
}

// main user interface.
void transaction()
{
    system("CLS");
    printf(" TRANSACTION MENU \n\n");
    printf("[1] . Balance Inquiry\n");
    printf("[2] . Cash Deposit\n");
    printf("[3] . Cash Withdrawal\n");
    if (m == 1)
    {
        printf("[4] . Main Menu\n");
    }
    else
    {
        printf("[4] . Exit\n");
    }
    printf("Please Enter Your Choice [1-4] : ");
    int choice;
    scanf("%d", &choice);
    printf("\n");
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
    printf(" BALANCE INQUIRY \n");
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    printf("\n");
    printf("Enter Password : ");
    // scanf("%s", &Password);
    getPassword(password);
    printf("\n");
    if (checkAccPass(a,password) == 1)
    {
        int availableBal = checkAccBalWithAccNum(a, password);
        system("cls");
        printf("Your Account balance is %d.\n", availableBal);
    }
    else
    {
        system("cls");
        printf("Account Doesn't Exist.\n");
    }
    printf("Press any key to return back to main menu. \n");
    getch();
    transaction();
}

//  adding or withdrawing amount to a account
void depsitOrWithdrawl(int IsDeposit)
{
    int a, bal, isValidAcc;
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    printf("\n");
    printf("Enter Amount : ");
    scanf("%d", &bal);
    printf("\n");
    if (!IsDeposit)
    {
        printf("Enter password:");
        // scanf("%s", Password);
        getPassword(password);
        printf("\n");
        isValidAcc = checkAccPass(a, password);
    }
    else
        isValidAcc = checkAccNum(a);
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
                        printf("Press any key to continue......");
                        getch();
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
            printf("deletion failed at depositOrWithdraw function\n");
        }

        if (IsDeposit)
            printf("Deposited successfully.\n");
        else
            printf("Withdrawn successfully.\n");
        printf("Press any key to return...... ");
        getch();
        transaction();
    }
    else
    {
        printf("Invalid Info.\n");
        printf("Press any key to return...... ");
        getch();
        transaction();
    }
}

// Generate password
void getPassword(char *pass)
{
    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();
        if (ch == 13)
        {
            pass[i] = '\0';
            break;
        }
        else
        {
            pass[i++] = ch;
            printf("*");
        }
    }
}
// logging out of the program.
void menuexit()
{
    system("cls");
    printf("Thank You for being with us.\n");
    printf("User :: %s \n", inputUserName);
    getch();
    exit(0);
}