#include <iostream>
#include <iomanip>
#include <string.h>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <map>
using namespace std;
using namespace oracle::occi;
const char Login = 'L';
const char Register = 'R';
const char AllBooks = 'A';
const char SearchBook = 'S';
const char MyBooks = 'M';
const char PostBook = 'P';
const char UpdateSold = 'U';
const char Logout = 'O';
const char help = 'H';
const char quit = 'Q';

typedef enum {
    Greeting, Menu, LoginMenu, InvalidCommand
} MSGType;

void showMessage(MSGType type) {
    switch (type)
    {
       case Greeting:
           cout << "\n*** Welcome to BOOK BUY/SELL/TRADE System ***";
           break;
       case LoginMenu:
           cout << "\n--------------Login Center--------------" << endl
                << "Please enter:" << endl
                << "  L to Login" << endl               
                << "  R to register new account" << endl
                << "  H to help to see this menu" << endl
                << "  Q to exit to terminate this program" << endl;
           break;
       case Menu:
           cout << "\n***** Customer Center *****" << endl
                << "Please enter:" << endl
                << "  A to show ALL books" << endl
                << "  S to search a book" << endl
                << "  M to list my books" << endl
                << "  P to post a new book" << endl
                << "  U to update your book to sold" << endl
                << "  H to help to see this menu" << endl
                << "  O to Logout" << endl;
           break;
       case InvalidCommand:
           cout << endl
                << "Invalid command." << endl
                << endl;
           break;
    }
}

char getCommandFromInput() {
    char cmd;
    string garbage; 
    cout << "-----------------------------------------" << endl;
    cout << "Enter your command choice: ";
    cin >> cmd;
    getline(cin, garbage);     
    return toupper(cmd);
}

string readPassword() {
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

string getValidstr(string prompt, int min, int max) {
	string str;
	cout << prompt;
	getline(cin, str);
	while (str.length()<min || str.length()>max){
        cout <<"--Eorror! must be more than "<< min <<" characters and less than "<< max <<" characters. Try again!" << endl;
		getline(cin, str);
	}
	return str;
}

template <class T>
T getValidnum(string prompt, int min, int max) {
	float num;
	bool getVaildnum = false;
	string garbage;
	cout << prompt;
	do{
		cin >> num;
		if (cin.fail()||num<min||num>max){
			cout << "--Eorror! must be a number from "<< min <<" to "<< max <<" Try again!" << endl;
			cin.clear();
			getline(cin, garbage);
		} else{
			getVaildnum = true;
			getline(cin, garbage);
		}
	} while (getVaildnum == false);
	return num;
}

void showbooks(ResultSet* rs){
    if (rs->next()){
        do {
            cout << "--------------------------------"<< endl;
            cout << "  BOOK_ID: " << rs->getString(1) << endl;
            cout << "    TITLE: " << rs->getString(2) << endl;
            cout << "   AUTHOR: " << rs->getString(3) << endl;
            cout << "PUBLISHER: " << rs->getString(4) << endl;
            cout << "     ISBN: " << rs->getString(5) << endl;
            cout << "    PRICE: $" << rs->getString(6) << endl;
            cout << "    OWNER: " << rs->getString(7) << endl;
            cout << "  CONTACT: " << rs->getString(8) << endl;
            cout << "    PHOTO: " << rs->getString(9) << endl;		            
         } while (rs->next());
     }else{
         cout << " NONE" << endl;
     }
}

void showTextbooks(ResultSet* rs){
    if (rs->next()){
        do {
            cout << "--------------------------------"<< endl;
            cout << "   BOOK_ID: " << rs->getString(1) << endl;
            cout << "     TITLE: " << rs->getString(2) << endl;
            cout << "    AUTHOR: " << rs->getString(3) << endl;
            cout << " PUBLISHER: " << rs->getString(4) << endl;
            cout << "      ISBN: " << rs->getString(5) << endl;
            cout << "     PRICE: $" << rs->getString(6) << endl;
            cout << "     OWNER: " << rs->getString(7) << endl;
            cout << "   CONTACT: " << rs->getString(8) << endl;
            cout << "     PHOTO: " << rs->getString(9) << endl;
            cout << "   PROGRAM: " << rs->getString(10) << endl;
            cout << "    COURSE: " << rs->getString(11) << endl;
            cout << "INSTRUCTOR: " << rs->getString(12) << endl;			            
         } while (rs->next());
     }else{
         cout << " NONE" << endl;
     }
}

void CustomerCenter(Statement *stmtAllBooks, ResultSet *rsAllBooks, Statement *stmtListmyBooks, ResultSet *rsListmyBooks, Statement *stmtKeywordSearch, ResultSet *rsKeywordSearch, 
                    Statement *stmtCategorieSearch, ResultSet *rsCategorieSearch, Statement *stmtTextbookSearch, ResultSet *rsTextbookSearch, 
                    Statement *stmtPostBook, Statement *stmtTextBook, Statement *stmtUpdateSold, string userID){
    char cmd;                
    showMessage(Menu);   
    while (cmd != Logout) {
        cmd = getCommandFromInput();
        if (cmd == AllBooks){
            cout << "All books in system: " << endl;
            rsAllBooks = stmtAllBooks->executeQuery();
            showbooks(rsAllBooks);                            
        }else if (cmd == SearchBook){
            int key = getValidnum<int>("Please enter:\n 1 form Keyword\n 2 from Categorie\n 3 from Textbook\n", 0, 3);
            if (key == 1){
                string Keyword = "%" + getValidstr("Please enter keyword search for Title, Author or Publisher: ", 1, 70) + "%";
                stmtKeywordSearch->setString(1, Keyword);
                stmtKeywordSearch->setString(2, Keyword);
                stmtKeywordSearch->setString(3, Keyword);
                cout << "Search result: " << endl;
                rsKeywordSearch = stmtKeywordSearch->executeQuery();
                showbooks(rsKeywordSearch);                              
            }else if(key == 2){
                int Categorie = getValidnum<int>("Please choose Categorie (1 for Fiction, 2 for Non-Fiction, 3 for Kids): ", 1, 3);
                stmtCategorieSearch->setInt(1, Categorie);
                cout << "Search result: " << endl;
                rsCategorieSearch = stmtCategorieSearch->executeQuery();
                showbooks(rsCategorieSearch);                              
            }else if(key == 3){
                string Keyword = "%" + getValidstr("Please enter keyword search for Program, Course Code or Instructor: ", 1, 70) + "%";
                stmtTextbookSearch->setString(1, Keyword);
                stmtTextbookSearch->setString(2, Keyword);
                stmtTextbookSearch->setString(3, Keyword);
                cout << "Search result: " << endl;
                rsTextbookSearch = stmtTextbookSearch->executeQuery();
                showTextbooks(rsTextbookSearch);                           
            }
        }else if (cmd == MyBooks){        
            stmtListmyBooks->setString(1, userID);
            rsListmyBooks = stmtListmyBooks->executeQuery();
            cout << "All your books in system: " << endl;
            showbooks(rsListmyBooks);
        }else if (cmd == PostBook){
            string Title = getValidstr("Your Book's Title: ", 1, 70);
            string Author =  getValidstr("Your Book's Author: ", 1, 70);
            string Publisher = getValidstr("Your Book's Publisher: ", 0, 70);
            string ISBN =  getValidstr("Your Book's ISBN: ", 0, 70);
            float Price = getValidnum<float>("Your Book's Price: ", 0, 99999999);
            string Photo =  getValidstr("Your Book's Photo path: ", 1, 70);		                               
            int Categorie = getValidnum<int>("Please enter your Book's Categorie (1 for Fiction, 2 for Non-Fiction, 3 for Kids): ", 0, 3);                                                                  
            stmtPostBook->setString(1, Title);
            stmtPostBook->setString(2, Author);
            stmtPostBook->setString(3, Publisher);
            stmtPostBook->setString(4, ISBN);
            stmtPostBook->setFloat(5, Price);
            stmtPostBook->setString(6, Photo); 
            stmtPostBook->setInt(7, Categorie);                   
            stmtPostBook->setString(8, userID);
            int rsPostBook = stmtPostBook->executeUpdate();
            if (rsPostBook>0){
                cout << "**Your book success posted ;)" << endl;
            }else{
                cout << "**Soory, posting failed" << endl;
            }                                    
            int textbook = getValidnum<int>("Do you want add your book to TextBook index? (1 for YES, 0 for NO): ", 0, 1);                                       
            if (textbook==1){
                stmtListmyBooks->setString(1, userID);
                rsListmyBooks = stmtListmyBooks->executeQuery();
                int Book_ID;
                if (rsListmyBooks->next()){
                    Book_ID = rsListmyBooks->getInt(1);
                }
                                                                 
                string Program =  getValidstr("Your TextBook's Program: ", 1, 70); 
                string Course =  getValidstr("Your TextBook's Course code: ", 1, 70);
                string Instructor =  getValidstr("Your TextBook's Instructor name: ", 1, 70);
                stmtTextBook->setInt(1, Book_ID);
                stmtTextBook->setString(2, Program);
                stmtTextBook->setString(3, Course);
                stmtTextBook->setString(4, Instructor);
                int rsTextBook = stmtTextBook->executeUpdate();
                if (rsTextBook>0){
                    cout << "**Your Textbook updated ;)" << endl;
                }else{
                    cout << "**Soory, Textbook update failed" << endl;
                } 
            }                    
        }else if (cmd == UpdateSold){
            int Book_ID = getValidnum<int>("Enter your Book ID which you sold: ", 0, 99999999);
            stmtUpdateSold->setString(1, userID);
            stmtUpdateSold->setInt(2, Book_ID);
            int rsUpdateSold = stmtUpdateSold->executeUpdate();
            if (rsUpdateSold>0){
                cout << "**Your book updated to sold now ;)" << endl;
            }else{
                cout << "**Soory, update failed" << endl;
            }                    
        }else if (cmd == help){
            showMessage(Menu);
    	}else if (cmd != Logout){
    		showMessage(InvalidCommand);
        }
    }
}

int main() {
    string userName = "jian";
    string password = "655081867";
    const string connectString = "sunfire.csci.viu.ca";

    map<string, string> query;
    query["AllBooks"] = "select BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, USERNAME, CONTACT, PHOTO \
                        from Books natural join Account where SOLD_TIME is NULL";
    query["Register"] = "insert into Account values (:1, :2, :3, :4)";
    query["Vailduser"] = "select PASSWORD from Account where SNO_INO = :1";
    query["ListmyBooks"] = "select BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, USERNAME, CONTACT, PHOTO \
                            from Books natural join Account where SOLD_TIME is NULL and SNO_INO = :1 order by BOOK_ID desc";
    query["PostBook"] = "insert into Books (BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, PHOTO, POST_TIME, C_ID, SNO_INO) \
                         select MAX(Book_ID)+1, :1, :2, :3, :4, :5, :6, CURRENT_TIMESTAMP, :7, :8 from Books";
    query["TextBook"] = "insert into Text_Books values(:1, :2, :3, :4)";    
    query["UpdateSold"] = "update Books set SOLD_TIME = CURRENT_TIMESTAMP where SNO_INO = :1 and BOOK_ID = :2";    
    query["KeywordSearch"] = "select BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, USERNAME, CONTACT, PHOTO \
                            from (select * from Books natural join Account where SOLD_TIME is NULL) \
                            where TITLE like :1 or AUTHOR like :2 or PUBLISHER like :3 order by BOOK_ID desc";  
    query["CategorieSearch"] = "select BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, USERNAME, CONTACT, PHOTO \
                            from (select * from Books natural join Account where SOLD_TIME is NULL) \
                            where C_ID = :1 order by BOOK_ID desc"; 
    query["TextbookSearch"] = "select BOOK_ID, TITLE, AUTHOR, PUBLISHER, ISBN, PRICE, USERNAME, CONTACT, PHOTO, PROGRAM, COURSE, INSTRUCTOR \
                               from (select * from Books natural join Account natural join Text_Books where SOLD_TIME is NULL) \
                               where PROGRAM like :1 or  COURSE like :2 or INSTRUCTOR like :3 order by BOOK_ID desc";  
        
    char cmd; 
    string userID; 
             
    try {
        Environment *env = Environment::createEnvironment();
        Connection *conn = env->createConnection(userName, password, connectString);        

	    Statement *stmtVailduser = conn->createStatement(query["Vailduser"]);
        ResultSet *rsVailduser = 0;  
        Statement *stmtRegister = conn->createStatement(query["Register"]);
        stmtRegister->setAutoCommit(true);

        Statement *stmtAllBooks = conn->createStatement(query["AllBooks"]);
        ResultSet *rsAllBooks = 0;  
        Statement *stmtListmyBooks = conn->createStatement(query["ListmyBooks"]);
        ResultSet *rsListmyBooks = 0;
	    Statement *stmtKeywordSearch = conn->createStatement(query["KeywordSearch"]);
	    ResultSet *rsKeywordSearch = 0;
	    Statement *stmtCategorieSearch = conn->createStatement(query["CategorieSearch"]);
	    ResultSet *rsCategorieSearch = 0;
	    Statement *stmtTextbookSearch = conn->createStatement(query["TextbookSearch"]);
	    ResultSet *rsTextbookSearch = 0;
        Statement *stmtPostBook = conn->createStatement(query["PostBook"]);
        stmtPostBook->setAutoCommit(true);        
        Statement *stmtTextBook = conn->createStatement(query["TextBook"]);
        stmtTextBook->setAutoCommit(true);  
	    Statement *stmtUpdateSold = conn->createStatement(query["UpdateSold"]);
	    stmtUpdateSold->setAutoCommit(true);
 
        showMessage(Greeting);
        showMessage(LoginMenu);       
        while (cmd != quit) {
            cmd = getCommandFromInput();
            if (cmd == Login){
                userID = getValidstr("Your user ID (studen/Instructor ID): ", 1, 40);
                cout << "Your password: ";
                string userpassword ="";
                userpassword= readPassword();
                cout << endl;
                stmtVailduser->setString(1, userID);             
                rsVailduser = stmtVailduser->executeQuery();
                string temppassword;   
                if (rsVailduser->next()){
                  temppassword = rsVailduser->getString(1);                
                }
                
                if (temppassword == userpassword && temppassword != "" && userpassword != "" ){
                    CustomerCenter(stmtAllBooks, rsAllBooks, stmtListmyBooks, rsListmyBooks, stmtKeywordSearch, rsKeywordSearch, stmtCategorieSearch, rsCategorieSearch, 
                                    stmtTextbookSearch, rsTextbookSearch, stmtPostBook, stmtTextBook, stmtUpdateSold, userID);
                }else{
                    cout << "\nSorry you are not in our system! Try again or Register New Account" << endl;
                }
                showMessage(LoginMenu);
            }else if (cmd == Register){
               bool ID;
               string newuserID;
               string newuserpassword;
               string comfirmpassword;   
               do{
                    ID = true;
                    newuserID =  getValidstr("Your user ID (studen/Instructor ID): ", 1, 40);
                    stmtVailduser->setString(1, newuserID);             
                    rsVailduser = stmtVailduser->executeQuery();
                    if(rsVailduser->next()){
                        cout << "--Error! user ID exist, you can not register any more." << endl;
                        ID = false;
                    }
                } while (ID == false);
                do{
                    do{
                        cout << "Choose password: ";
                        newuserpassword = readPassword();
                        cout << endl;
                    } while (newuserpassword.length()<=0 || newuserpassword.length() >= 256);
                    do{
                        cout << "Comfirm password: ";
                        comfirmpassword = readPassword();
                        cout << endl;
                    } while (comfirmpassword.length()<=0 || comfirmpassword.length() >= 256);
                    
                    if(newuserpassword!=comfirmpassword){
                        cout << "--Error! comfirm password NOT match, try again." << endl;
                    }              
                } while (newuserpassword!=comfirmpassword);  
                string newusername =  getValidstr("Your Name: ", 1, 40);            
                string newcontact = getValidstr("Your Contact: ", 1, 70);                                         
                stmtRegister->setString(1, newuserID);
                stmtRegister->setString(2, newuserpassword);
                stmtRegister->setString(3, newusername);
                stmtRegister->setString(4, newcontact);                
                int rsRegister = stmtRegister->executeUpdate();
                if (rsRegister>0){
                    cout << "**Congratulation! You got New Account now ;)" << endl;
                }else{
                    cout << "**Soory, register failed" << endl;
                }
            }else if (cmd == help){
                    showMessage(LoginMenu);
            	}else if (cmd != quit){
            		showMessage(InvalidCommand);
                }
        }
        
        if (rsVailduser) {stmtVailduser->closeResultSet(rsVailduser);}
        conn->terminateStatement(stmtVailduser);
        if (rsAllBooks) {stmtAllBooks->closeResultSet(rsAllBooks);}
        conn->terminateStatement(stmtAllBooks);
        if (rsKeywordSearch) {stmtKeywordSearch->closeResultSet(rsKeywordSearch);}
        conn->terminateStatement(stmtKeywordSearch);
        if (rsCategorieSearch) {stmtCategorieSearch->closeResultSet(rsCategorieSearch);}
        conn->terminateStatement(stmtCategorieSearch);
        if (rsTextbookSearch) {stmtTextbookSearch->closeResultSet(rsTextbookSearch);}
        conn->terminateStatement(stmtTextbookSearch);
        if (rsListmyBooks) {stmtListmyBooks->closeResultSet(rsListmyBooks);}
        conn->terminateStatement(stmtListmyBooks);
        
        conn->terminateStatement(stmtPostBook);
        conn->terminateStatement(stmtTextBook);
        conn->terminateStatement(stmtUpdateSold);    
        conn->terminateStatement(stmtRegister);         
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);    
    } catch (SQLException & e) {
        cout << e.what();
        return 0;
    }
    return 0;
}
