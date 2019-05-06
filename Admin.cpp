#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace oracle::occi;

// read database password from user input
// without showing the password on the screen
string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}

int main() {
    string userName = "jian";
    string password = "655081867";
    const string connectString = "sunfire.csci.viu.ca";  
    time_t now = time(0);
    tm* localtm = localtime(&now);
    string Nowtime = asctime(localtm);
       
    try {
        Environment *env = Environment::createEnvironment();
        Connection *conn = env->createConnection (userName, password, connectString);
        Statement *stmt = conn->createStatement();
        string queryStr = "select Account.SNO_INO, Account.USERNAME, count(POST_TIME) as POST, count(SOLD_TIME) as SOLD \
                           from Account left join Books on Account.SNO_INO = Books.SNO_INO \
                           group by Account.SNO_INO, Account.USERNAME \
                           order by POST desc ";
        ResultSet *rs = stmt->executeQuery(queryStr);          
            ofstream myfile;
            string filename = "Report-" + Nowtime + ".txt";
            myfile.open (filename.c_str());
            myfile << " CUSTOMER ACTIVITY REPORT\n\n";
            myfile << " REPORT TIME: " << asctime(localtm) << "\n";
            myfile << "   USER_ID        USER_NAME      POST #      SOLD# \n";
            myfile << " -------------  -------------  ----------  ----------\n";
            
            if (rs->next()){ 
                do {             
                    myfile << setw(12) << rs->getString(1);
                    myfile << setw(12) << rs->getString(2);
                    myfile << setw(12) << rs->getString(3);
                    myfile << setw(12) << rs->getString(4) << "\n\n";
                } while (rs->next());
            }
            myfile << " ----------------------------------------------------"<< endl;
        
        myfile.close();
        
        cout << "\nThe Report DONE as local date and time: " << asctime(localtm) << endl;
        
        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    } catch (SQLException & e) {
        cout << e.what();
        return 0;
    }

    return 0;
}

