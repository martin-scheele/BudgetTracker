# BudgetTracker
---
GUI budget tracking application made with Qt6.

Final Project for CSC-284 - Advanced C++/OOP, Fall 2023

Uploaded retroactively to familiarize myself with git
---

# Changelog 

---
---
## Final Project - Final Submission - 12/15/23

---
### Current Status:

- The main transaction functionality has been consolidated to one screen / widget (BudgetTracker).
- UI has been remade using the Qt UI designer.
- User cannot remove entries. BudgetTracker::removeEntry() is a stub. I think this would require either returning to using QSqlTableModel (and figuring out how to calculate the balance correctly) or figuring out how to retrieve the transactionID of a selected row. The former might be preferable, as subclassing QSqlTableModel would allow for better control of editing functionality as well. 
- Plots have been implemented. User can filter the plot based on category and subcategory.
- Table can be filtered by category and subcategory as well. 
- Basic input validation has been done via the UI.
- User login has been rewritten using a database manager class to abstract the SQLite queries away from the business logic. BudgetTracker does not have an equivalent database manager.
- User login has been simplified to use only a user class. Admin privileges were stripped away and unimplemented for the time being.
- "Forgot login" mechanism is rudimentary. 
- Stored passwords and transaction database entries are unencrypted.

---
### TODO:

- Implement BudgetTracker::removeEntry().
- Implement budget database manager.
- Change Budget database to accept QDateTime instead of QDate, to allow better plotting of transactions that occur on the same day.
- Subclass QSqlTableModel (instead of QSqlQueryModel) to allow editing and easier removal of entries. 
- Change individual transaction plottables to bar graph to better visualize positive and negative transaction amounts.
- Display total/balance plot along with individual transactions bars.
- Show plottable information on hover (exact date and amount) if possible (or some equivalent functionality).
- Consolidate filter "widgets" into one, using one button for filtering table and one button for filtering plot (or both at once?).
- Revamp SQL queries and table (NOT NULL, etc.).
- Table should load only a portion of the database and not all of it at once. User should be able to navigate through table pages (not sure how this would work for the plot).  
- Fully implement user login functionality (first name, last name, privileges, etc.). Implement admin privileges.
- Hash passwords and transaction database entries.
- Revamp UI - scalability, aesthetics, etc.
- Set focus on relevant buttons when filling out respective information fields.
- Use Qt Installer Framework to create an actual installer for the program (statically linked?). 

---
### Bugs

- When clearing the table filter, the last section of the header does not stretch properly unless the header column size is adjusted.
- When changing the transactionTableView headers, a white rectangle extends from the header outside of the group box to the right.

---
--- 
## Final Project - Status Report 2 - 12/3/23

---
### Current Status:

- User login has been implemented.
- Transition to SQLite database has been made. Each user has a separate database file.
- User can add entries to database but cannot remove them yet. The remove function (TableViewer::remove()) is a stub.
- Running balance calculates correctly. QSqlQueryModel was used, as I could not figure out how to accomplish this with QSqlTableModel. I tried to create a trigger but it didn't work correctly. QSqlQueryModel is read-only, so I will have  to figure out how to remove entries and if I can allow entries to be edited.  Otherwise, I will have to figure out how to get the running balance working  with QSqlTableModel.
- User can filter entries based on category.
- Plots are not yet implemented.
- Entries do not have any input validation.

---
### TODO:

- Implement remove function.
- Filter by subcategory as well as category.
- Implement plots based on filtration.
- Create documentation, video demonstration, installation manual, and user manual.

### IF I HAVE TIME:

- Rewrite code to make use of a database manager, as well as user and userlogin  classes (or combine User and UserLogin into one class for the purpose of this  program). At first I didn't see the necessity of the classes, but after working  with this program some more I see the value in separating the database logic  (SQL queries, etc) from the UI logic. While it will probably be marginally less efficient, creating and passing User/UserLogin objects looks cleaner. TableViewer can have a User/UserLogin class as a member for easier access to data. There can  be a manager for the login database as well as the budget table databases.
- Now that I have more experience with how the UI setup works under the hood, I  want to transition to using the UI designer. Ideally, the whole program would  be remade using QMainWindow, and have most (if not all) of the functionality available from the main window or toolbars.

---
### Timeline:

- 12/03 - 12/10: Implement as much as possible, clean up code and class names,  finalize program.
- 12/11 - 12/15: Create documentation, video demonstration, installation manual, and user manual.

---
---
## Final Project - Status Report 1  - 11/19/23

---
### Current Status:

- Program is completely functional as it currently stands, with a few important missing features. 
- User can add new entries to database, load entries from a file and save entries to a file.  Database is saved in a single file, and not user-defined. Stub.
- Users cannot remove entries from database yet. 
- Entries can be inserted out of order, and database automatically sorts them in order. Running balance is fully functional and updates on each entry insertion. 
- User defined subviews have not yet been implemented, as the SQL implementation works very differently from the implementation with QAbstractTableModel and QSortFilterProxyModel, which are significantly more involved.
- Custom plots have not been implemented. 

---
### Notes on plan and transition to SQL implementation:

- Program must be rewritten with SQLite implementation. SQLTableModel replaces custom QAbstractTableModel - from the UserLogin project, this seems easier to work with. However, the running balance has to be solved again - this will likely prove to be the source of most difficulty. On the other hand, having user-defined views for specific (sub)categories, date ranges, etc., will be easier with SQLTableModel filtering instead of QSortFilterProxyModel. Removal of entries should be drastically simplified.
- The login screen from the UserLogin assignment must be adapted, which will likely be trivial - each user will have their own table, and that table will be loaded on login. 
- After all basic table functionality is complete, plots based on the user-defined subviews will be implemented with QCustomPlot - not sure how that will interface with SQLite queries, or if an intermediary container will be necessary.
- Once all required aspects are complete, UI layout will be refined, and additional quality of life functionality will be added - for example, entries should be editable directly within table view, new entries should be cached and only  saved on manual submit, a limited number of entries should be shown at one time, running balance implementation should  be optimized to whatever extent possible, UI should be persistent window with login from toolbar and user should be able to logout, user login information and databases should be encrypted (even if naively). Additional modifications will surely arise. 
- Source code will be fully formally commented, and once fully complete, doxygen documentation will be generated.

---
### Timeline:

- 11/19 - 12/3: Complete SQL rewrite with user login. Subviews will likely be implemented as well, and QCustomPlot if there are no unexpected hiccups.
- 12/3 - 12/15: Further refinement and modifications. Documentation, video walkthrough, and reflection will be completed during the final week. 
---
--- 
