#include <cx_auth/iauth_validation_account.h>
#include <cx_auth_fs/iauth_fs.h>
#include <cx_auth_sqlite3/iauth_sqlite3.h>

#include <time.h>

void TestIAuth(IAuth *auth)
{
    clock_t start, end;

    start = clock();

    printf("-test begin-\n");
    auth->accountChangePassword("admin", IAuth::genPassword("123456XYZ",PASS_MODE_SSHA256), 1 );
    auth->groupAdd("computers", "Accounts belonging to computers");
    auth->groupAdd("users", "Accounts belonging to users");
    auth->attribAdd("power", "Account can do power related operations");
    auth->attribAdd("network", "Account can do network related operations");

    auth->groupAccountAdd("users","admin");
    auth->attribAccountAdd("network", "admin");
    auth->attribGroupAdd("power","computers");
    auth->attribGroupAdd("power","users");

    printf("printing admin attributes:\n");

    for ( std::string i : auth->accountUsableAttribs("admin") )
    {
        printf("- attrib: %s\n", i.c_str());
    }

    printf("Gen confirmation token: %s\n", auth->accountConfirmationToken("admin","default").c_str());

    AuthReason reason = auth->authenticate("admin","","123456XYZ",1);
    printf("Validating password...: %d (%s)\n", reason, getAuthReasonText(reason));

    end = clock();

    printf("-test end (%f seconds)-\n-------------------------------------------------------------------\n\n",  (double)(end-start)/CLOCKS_PER_SEC);
    fflush(stdout);
}

int main(int argc, char * argv[])
{
    sPasswordData passwd;
    passwd.hash = "XYZ";

    IAuth_FS fsAuthTest("cxauthtester");
    printf("Testing Directory Based user db\n");
    TestIAuth(&fsAuthTest);

    IAuth_SQLite3 sqliteAuthTest("cxauthtester");
    printf("Testing SQLite3 Based user db\n");
    TestIAuth(&sqliteAuthTest);

    return 0;
}
