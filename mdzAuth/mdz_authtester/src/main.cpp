#include <cstdio>
#include <mdz_auth_db/manager_db.h>
#include <mdz_db_sqlite3/sqlconnector_sqlite3.h>
#include <mdz_hlp_functions/crypto.h>

#include <time.h>

using namespace Mantids::Authentication;
using namespace Mantids::Database;
using namespace Mantids;

void testAuth(Manager_DB *auth)
{
    clock_t start, end;

    start = clock();

    printf("-test begin-\n");

    // Create Basic Accounts
    auth->initScheme();
    auth->initAccounts();

    Authentication::Secret secretData;
    secretData.hash = Helpers::Crypto::calcSHA256("123456XYZ");
    secretData.passwordFunction = Authentication::FN_SHA256;
    secretData.forceExpiration = false;

    // Change password for "admin"
    auth->accountChangeSecret("admin", secretData, 0 );

    // Create an application called test.
    auth->applicationAdd("test","Testing app", "123", "admin");

    // Add attributes to application test.
    auth->attribAdd({"test","power"}, "Account can do power related operations");
    auth->attribAdd({"test","network"}, "Account can do network related operations");

    // Add groups
    auth->groupAdd("computers", "Accounts belonging to computers");
    auth->groupAdd("users", "Accounts belonging to users");

    // Add account admin to group users.
    auth->groupAccountAdd("users","admin");
    // Add account admin to application test
    auth->applicationAccountAdd("test","admin");

    // Set attributes for group/accounts
    auth->attribAccountAdd({"test","network"}, "admin");
    auth->attribGroupAdd({"test","power"},"computers");
    auth->attribGroupAdd({"test","power"},"users");

    printf("printing admin attributes:\n");
    for ( const auto & i : auth->accountUsableAttribs("admin") )
    {
        printf("- attrib: %s, app: %s\n", i.attribName.c_str(), i.appName.c_str());
    }

    printf("Gen confirmation token: %s\n", auth->accountConfirmationToken("admin").c_str());

    sClientDetails det;
    det.sIPAddr = "127.0.0.1";
    det.sUserAgent = "local";

    Reason reason = auth->authenticate("test", det,"admin","123456XYZ");

    printf("Validating password...: %d (%s)\n", reason, getReasonText(reason));

    end = clock();

    printf("-test end (%f seconds)-\n-------------------------------------------------------------------\n\n",  (double)(end-start)/CLOCKS_PER_SEC);
    fflush(stdout);
}

int main(int argc, char * argv[])
{
    SQLConnector_SQLite3 sqlConnection;
    sqlConnection.connectInMemory();
    Manager_DB sqliteAuthTest(&sqlConnection);

    //IAuth_SQLite3 sqliteAuthTest("cxauthtester");
    printf("Testing SQLite3 Based user db\n");
    testAuth(&sqliteAuthTest);

    return 0;
}
