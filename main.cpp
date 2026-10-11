#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <limits>
using namespace std;
class Book
{
private:
    int id;
    string title;
    string author;
    string category;
    bool issued;
public:
    Book()
    {
        id = 0;
        title = "";
        author = "";
        category = "";
        issued = false;
    }
    Book(int id, string title, string author,
         string category, bool issued = false)
    {
        this->id = id;
        this->title = title;
        this->author = author;
        this->category = category;
        this->issued = issued;
    }
    int getId() const
    {
        return id;
    }
    string getTitle() const
    {
        return title;
    }
    string getAuthor() const
    {
        return author;
    }
    string getCategory() const
    {
        return category;
    }
    bool isIssued() const
    {
        return issued;
    }
    void setTitle(string title)
    {
        this->title = title;
    }
    void setAuthor(string author)
    {
        this->author = author;
    }
    void setCategory(string category)
    {
        this->category = category;
    }
    void setIssued(bool status)
    {
        issued = status;
    }
    void display() const
    {
        cout << id << "\t"
             << title << "\t"
             << author << "\t"
             << category << "\t"
             << (issued ? "Issued" : "Available")
             << endl;
    }
    string serialize() const
    {
        return to_string(id) + "|" +
               title + "|" +
               author + "|" +
               category + "|" +
               (issued ? "1" : "0");
    }
    static Book deserialize(string line)
    {
        stringstream ss(line);
        string data[5];
        string temp;
        int i = 0;
        while (getline(ss, temp, '|') && i < 5)
        {
            data[i++] = temp;
        }
        return Book(
            stoi(data[0]),
            data[1],
            data[2],
            data[3],
            data[4] == "1"
        );
    }
};

class Member
{
private:
    int id;
    string name;
    string phone;
    string email;
public:
    Member()
    {
        id = 0;
        name = "";
        phone = "";
        email = "";
    }
    Member(int id, string name, string phone, string email)
    {
        this->id = id;
        this->name = name;
        this->phone = phone;
        this->email = email;
    }
    int getId() const
    {
        return id;
    }
    string getName() const
    {
        return name;
    }
    string getPhone() const
    {
        return phone;
    }
    string getEmail() const
    {
        return email;
    }
    void setName(string name)
    {
        this->name = name;
    }
    void setPhone(string phone)
    {
        this->phone = phone;
    }
    void setEmail(string email)
    {
        this->email = email;
    }
    void display() const
    {
        cout << id << "\t"
             << name << "\t"
             << phone << "\t"
             << email
             << endl;
    }
    string serialize() const
    {
        return to_string(id) + "|" +
               name + "|" +
               phone + "|" +
               email;
    }
    static Member deserialize(string line)
    {
        stringstream ss(line);
        string data[4];
        string temp;
        int i = 0;
        while (getline(ss, temp, '|') && i < 4)
        {
            data[i++] = temp;
        }
        return Member(
            stoi(data[0]),
            data[1],
            data[2],
            data[3]
        );
    }
};
struct IssueRecord
{
    int bookId;
    int memberId;
    string issueDate;
    string dueDate;
    bool returned;
    string serialize() const
    {
        return to_string(bookId) + "|" +
               to_string(memberId) + "|" +
               issueDate + "|" +
               dueDate + "|" +
               (returned ? "1" : "0");
    }
    static IssueRecord deserialize(string line)
    {
        stringstream ss(line);
        string data[5];
        string temp;
        int i = 0;
        while (getline(ss, temp, '|') && i < 5)
        {
            data[i++] = temp;
        }
        IssueRecord record;
        record.bookId = stoi(data[0]);
        record.memberId = stoi(data[1]);
        record.issueDate = data[2];
        record.dueDate = data[3];
        record.returned = (data[4] == "1");
        return record;
    }
};
// DATE FUNCTION
string getDate(int days = 0)
{
    time_t currentTime = time(nullptr);
    currentTime +=
        static_cast<time_t>(days) *
        24 * 60 * 60;
    tm* localTime = localtime(&currentTime);
    char buffer[20];
    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d",
        localTime
    );

    return string(buffer);
}
class Library
{
private:
    vector<Book> books;
    vector<Member> members;
    vector<IssueRecord> records;
    const string BOOK_FILE = "books.dat";
    const string MEMBER_FILE = "members.dat";
    const string RECORD_FILE = "issue_records.dat";
    void loadBooks()
    {
        books.clear();
        ifstream file(BOOK_FILE);
        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                books.push_back(
                    Book::deserialize(line)
                );
            }
        }
        file.close();
    }
    void loadMembers()
    {
        members.clear();
        ifstream file(MEMBER_FILE);
        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                members.push_back(
                    Member::deserialize(line)
                );
            }
        }
        file.close();
    }

    void loadRecords()
    {
        records.clear();
        ifstream file(RECORD_FILE);
        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                records.push_back(
                    IssueRecord::deserialize(line)
                );
            }
        }
        file.close();
    }
    void saveBooks() const
    {
        ofstream file(BOOK_FILE);
        for (const Book& book : books)
        {
            file << book.serialize() << endl;
        }
        file.close();
    }
    void saveMembers() const
    {
        ofstream file(MEMBER_FILE);
        for (const Member& member : members)
        {
            file << member.serialize() << endl;
        }
        file.close();
    }
    void saveRecords() const
    {
        ofstream file(RECORD_FILE);
        for (const IssueRecord& record : records)
        {
            file << record.serialize() << endl;
        }
        file.close();
    }
    // NEXT BOOK ID
    int nextBookId() const
    {
        int id = 0;
        for (const Book& book : books)
        {
            if (book.getId() > id)
            {
                id = book.getId();
            }
        }
        return id + 1;
    }
    // NEXT MEMBER ID
    int nextMemberId() const
    {
        int id = 0;
        for (const Member& member : members)
        {
            if (member.getId() > id)
            {
                id = member.getId();
            }
        }
        return id + 1;
    }
public:

    Library()
    {
        loadBooks();
        loadMembers();
        loadRecords();
    }
    void addBook()
    {
        string title;
        string author;
        string category;
        cout << "\n====================================\n";
        cout << "===============ADD NEW BOOK================\n";
        cout << "Enter Book Title: ";
        getline(cin >> ws, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
        cout << "Enter Category: ";
        getline(cin, category);
        int id = nextBookId();
        Book book(
            id,
            title,
            author,
            category
        );
        books.push_back(book);
        saveBooks();
        cout << "\nBook added successfully!\n";
        cout << "Book ID: " << id << endl;
    }

    void displayBooks() const
    {
        cout << "===============ALL BOOKS================\n";
        if (books.empty())
        {
            cout << "No books available.\n";
            return;
        }
        cout << "ID\tTitle\tAuthor\tCategory\tStatus\n";
        cout << "------------------------------------------------------------\n";

        for (const Book& book : books)
        {
            book.display();
        }
    }

    void searchBook() const
    {
        string keyword;
        cout << "===============SEARCH BOOK=============\n";
        cout << "Enter title/author/category: ";
        getline(cin >> ws, keyword);
        bool found = false;
        for (const Book& book : books)
        {
            string searchData =
                book.getTitle() + " " +
                book.getAuthor() + " " +
                book.getCategory();
            if (searchData.find(keyword) != string::npos)
            {
                if (!found)
                {
                    cout << "\nID\tTitle\tAuthor\tCategory\tStatus\n";

                    cout << "------------------------------------------------------------\n";
                }
                book.display();

                found = true;
            }
        }
        if (!found)
        {
            cout << "\nNo matching book found.\n";
        }
    }

    void updateBook()
    {
        int id;
        cout << "=========== UPDATE BOOK==================\n";
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
        for (Book& book : books)
        {
            if (book.getId() == id)
            {
                string title;
                string author;
                string category;
                cout << "Enter New Title: ";
                getline(cin, title);
                cout << "Enter New Author: ";
                getline(cin, author);
                cout << "Enter New Category: ";
                getline(cin, category);
                book.setTitle(title);
                book.setAuthor(author);
                book.setCategory(category);
                saveBooks();
                cout << "\nBook updated successfully!\n";
                return;
            }
        }
        cout << "\nBook not found.\n";
    }
    void deleteBook()
    {
        int id;
        cout << "=============DELETE BOOK=============\n";
        cout << "Enter Book ID: ";
        cin >> id;
        for (const IssueRecord& record : records)
        {
            if (record.bookId == id &&
                !record.returned)
            {
                cout << "\nBook is currently issued.\n";
                cout << "Return the book before deleting it.\n";
                 return;
            }
        }
        auto it = remove_if(
            books.begin(),
            books.end(),
            [id](const Book& book)
            {
                return book.getId() == id;
            }
        );
        if (it != books.end())
        {
            books.erase(it, books.end());
            saveBooks();
            cout << "\nBook deleted successfully!\n";
        }
        else
        {
            cout << "\nBook not found.\n";
        }
    }
    void addMember()
    {
        string name;
        string phone;
        string email;
        cout << "=============ADD NEW MEMBER=============\n";
        cout << "Enter Member Name: ";
        getline(cin >> ws, name);
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        cout << "Enter Email: ";
        getline(cin, email);
        int id = nextMemberId();
        Member member(
            id,
            name,
            phone,
            email
        );
        members.push_back(member);
        saveMembers();
        cout << "\nMember added successfully!\n";
        cout << "Member ID: " << id << endl;
    }
    void displayMembers() const
    {       
        cout << "==============ALL MEMBERS==============\n";
        if (members.empty())
        {
            cout << "No members found.\n";
            return;
        }
        cout << "ID\tName\tPhone\tEmail\n";
        cout << "------------------------------------------------------------\n";
        for (const Member& member : members)
        {
            member.display();
        }
    }


    // --------------------------------------------------------
    // UPDATE MEMBER
    // --------------------------------------------------------

    void updateMember()
    {
        int id;

        cout << "=============UPDATE MEMBER=============\n";

        cout << "Enter Member ID: ";
        cin >> id;
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
        for (Member& member : members)
        {
            if (member.getId() == id)
            {
                string name;
                string phone;
                string email;
                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Phone: ";
                getline(cin, phone);
                cout << "Enter New Email: ";
                getline(cin, email);
                member.setName(name);
                member.setPhone(phone);
                member.setEmail(email);
                saveMembers();
                cout << "\nMember updated successfully!\n";
                return;
            }
        }
        cout << "\nMember not found.\n";
    }

    void deleteMember()
    {
        int id;
        cout << "=============DELETE MEMBER=============\n";
        cout << "Enter Member ID: ";
        cin >> id;
        for (const IssueRecord& record : records)
        {
            if (record.memberId == id &&
                !record.returned)
            {
                cout << "\nMember has an issued book.\n";
                cout << "Return the book before deleting member.\n";

                return;
            }
        }
        auto it = remove_if(
            members.begin(),
            members.end(),
            [id](const Member& member)
            {
                return member.getId() == id;
            }
        );
        if (it != members.end())
        {
            members.erase(it, members.end());
            saveMembers();
            cout << "\nMember deleted successfully!\n";
        }
        else
        {
            cout << "\nMember not found.\n";
        }
    }

    void issueBook()
    {
        int bookId;
        int memberId;
        cout << "===============ISSUE BOOK===============\n";
        cout << "Enter Book ID: ";
        cin >> bookId;
        cout << "Enter Member ID: ";
        cin >> memberId;
        Book* selectedBook = nullptr;
        for (Book& book : books)
        {
            if (book.getId() == bookId)
            {
                selectedBook = &book;
                break;
            }
        }
        if (selectedBook == nullptr)
        {
            cout << "\nBook not found.\n";
            return;
        }
        bool memberExists = false;
        for (const Member& member : members)
        {
            if (member.getId() == memberId)
            {
                memberExists = true;
                break;
            }
        }
        if (!memberExists)
        {
            cout << "\nMember not found.\n";
            return;
        }
        if (selectedBook->isIssued())
        {
            cout << "\nBook is already issued.\n";
            return;
        }
        selectedBook->setIssued(true);
        IssueRecord record;
        record.bookId = bookId;
        record.memberId = memberId;
        record.issueDate = getDate(0);
        // 14 day issue period
        record.dueDate = getDate(14);
        record.returned = false;
        records.push_back(record);
        saveBooks();
        saveRecords();
        cout << "\nBook issued successfully!\n";
        cout << "Issue Date : "
             << record.issueDate
             << endl;
        cout << "Due Date   : "
             << record.dueDate
             << endl;
    }
    void returnBook()
    {
        int bookId;
        cout << "===============RETURN BOOK===============\n";
        cout << "Enter Book ID: ";
        cin >> bookId;
        for (IssueRecord& record : records)
        {
            if (record.bookId == bookId &&
                !record.returned)
            {
                record.returned = true;
                for (Book& book : books)
                {
                    if (book.getId() == bookId)
                    {
                        book.setIssued(false);
                        break;
                    }
                }
                saveBooks();
                saveRecords();
                cout << "\nBook returned successfully!\n";
                return;
            }
        }
        cout << "\nNo active issue record found.\n";
    }
    void displayIssuedBooks() const
    {
        cout << "===============ISSUED BOOKS===============\n";
        bool found = false;
        cout << "Book ID\tMember ID\tIssue Date\tDue Date\n";
        cout << "------------------------------------------------------------\n";
        for (const IssueRecord& record : records)
        {
            if (!record.returned)
            {
                cout << record.bookId
                     << "\t"
                     << record.memberId
                     << "\t\t"
                     << record.issueDate
                     << "\t"
                     << record.dueDate
                     << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No books are currently issued.\n";
        }
    }

    void report()
    {
        int issuedBooks = 0;
        for (const Book& book : books)
        {
            if (book.isIssued())
            {
                issuedBooks++;
            }
        }
        int totalBooks = books.size();
        int availableBooks =
            totalBooks - issuedBooks;
        cout << "==============LIBRARY REPORT\n";
        cout << "Total Books        : "
             << totalBooks
             << endl;
        cout << "Available Books    : "
             << availableBooks
             << endl;
        cout << "Issued Books       : "
             << issuedBooks
             << endl;
        cout << "Total Members      : "
             << members.size()
             << endl;
        cout << "Total Transactions : "
             << records.size()
             << endl;
        cout << "====================================\n";
    }
};

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
    cin.get();
}
int main()
{
    Library library;
    int choice;
    do
    {
        cout << "\n\n";
        cout << "============================================\n";
        cout << "        LIBRARY MANAGEMENT SYSTEM\n";
        cout << "             OOCP MINI PROJECT\n";
        cout << "============================================\n";

        cout << "1.  Add Book\n";
        cout << "2.  Display All Books\n";
        cout << "3.  Search Book\n";
        cout << "4.  Update Book\n";
        cout << "5.  Delete Book\n";
        cout << "6.  Add Member\n";
        cout << "7.  Display All Members\n";
        cout << "8.  Update Member\n";
        cout << "9.  Delete Member\n";
        cout << "10. Issue Book\n";
        cout << "11. Return Book\n";
        cout << "12. Display Issued Books\n";
        cout << "13. Library Report\n";
        cout << "0.  Exit\n";
        cout << "--------------------------------------------\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );
            cout << "\nInvalid input!\n";
            continue;
        }
        switch (choice)
        {
            case 1:
                library.addBook();
                break;
            case 2:
                library.displayBooks();
                break;
            case 3:
                library.searchBook();
                break;
            case 4:
                library.updateBook();
                break;
            case 5:
                library.deleteBook();
                break;
            case 6:
                library.addMember();
                break;
            case 7:
                library.displayMembers();
                break;
            case 8:
                library.updateMember();
                break;
            case 9:
                library.deleteMember();
                break;
            case 10:
                library.issueBook();
                break;
            case 11:
                library.returnBook();
                break;
            case 12:
                library.displayIssuedBooks();
                break;
            case 13:
                library.report();
                break;
            case 0:
                cout << "\n====================================\n";
                cout << " Thank you for using Library System!\n";
                cout << "====================================\n";
                break;
            default:
                cout << "\nInvalid choice!";
                cout << " Please select 0-13.\n";
        }
        if (choice != 0)
        {
            pauseScreen();
        }
    }
    while (choice != 0);
    return 0;
}