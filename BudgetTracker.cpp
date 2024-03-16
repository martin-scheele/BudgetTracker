#include "BudgetTracker.h"
#include "ui_BudgetTracker.h"

#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>

/**
 * @brief BudgetTracker::BudgetTracker
 *        Sets up UI and connects signals and slots.
 *
 *        Sets up database and initializes transaction table and plot.
 * @param user logged in user
 * @param parent pointer to QWiget parent object
 */
BudgetTracker::BudgetTracker(std::shared_ptr<User> user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BudgetTracker)
    , transactionModel(new QSqlQueryModel(this))
    , m_user(user)
{
    ui->setupUi(this);

    // manual ui setup
    ui->entryDateDateEdit->setDate(QDate::currentDate());
    this->setWindowTitle(QString("BudgetTracker | Username: %1 | userID: %2")
                             .arg(user->getUsername(), QString::number(user->getUserID())));

    // database, table, plot initialization
    setupDatabase(user);
    initializeTable();
    initializePlot();

    // entry connections
    connect(ui->entryCategoryLineEdit, &QLineEdit::textChanged,
            this, &BudgetTracker::verifyEntry);
    connect(ui->entrySubcategoryLineEdit, &QLineEdit::textChanged,
            this, &BudgetTracker::verifyEntry);
    connect(ui->entryAmountLineEdit, &QLineEdit::textChanged,
            this, &BudgetTracker::verifyEntry);
    connect(ui->entryAddButton, &QPushButton::clicked,
            this, &BudgetTracker::addEntry);
    connect(ui->entryRemoveButton, &QPushButton::clicked,
            this, &BudgetTracker::removeEntry);

    // plot connections
    connect(ui->plotFilterCategoryLineEdit, &QLineEdit::textChanged,
            this, &BudgetTracker::verifyPlotFilter);
    connect(ui->plotFilterFilterButton, &QPushButton::clicked,
            this, &BudgetTracker::filterPlot);
    connect(ui->plotFilterClearButton, &QPushButton::clicked,
            this, &BudgetTracker::clearPlotFilter);
    connect(ui->transactionPlot, &QCustomPlot::mouseDoubleClick,
            this, &BudgetTracker::drawPlot);

    // table connections
    connect(ui->tableFilterCategoryLineEdit, &QLineEdit::textChanged,
            this, &BudgetTracker::verifyTableFilter);
    connect(ui->tableFilterFilterButton, &QPushButton::clicked,
            this, &BudgetTracker::filterTable);
    connect(ui->tableFilterClearButton, &QPushButton::clicked,
            this, &BudgetTracker::clearTableFilter);

}

/**
 * @brief BudgetTracker::~BudgetTracker
 *        Deallocates UI memory.
 */
BudgetTracker::~BudgetTracker()
{
    delete ui;
}

/**
 * @brief BudgetTracker::setupDatabase
 *        Sets up connection to SQLite user database,
 *        creates table for current user.
 * @param user current user whose database should be opened
 */
void BudgetTracker::setupDatabase(const std::shared_ptr<const User> user)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    db.setDatabaseName(path + QDir::separator() + QString("%1.sqlite")
                                                      .arg(user->getUsername()));
    db.open();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS budget ("
               "transactionID INTEGER PRIMARY KEY, "
               "date VARCHAR(20), "
               "category VARCHAR(20), "
               "subcategory VARCHAR(20),"
               "amount DOUBLE)");
}

/**
 * @brief BudgetTracker::initializeTable
 *        Initializes table view. Calls drawTable
 *        with no filter.
 */
void BudgetTracker::initializeTable()
{
    ui->transactionTableView->setModel(transactionModel);
    drawTable();
    ui->transactionTableView->horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief BudgetTracker::drawTable
 *        Draws table based on current table category and subcategory criteria.
 */
void BudgetTracker::drawTable()
{
    QSqlQuery query;
    // if currentTableCategory is empty, show all transactions
    if (m_currentTableCategory == "") {
        transactionModel->setQuery("SELECT transactionID, date, category, subcategory, amount, SUM(amount) "
                                   "OVER (ORDER BY date, transactionID ROWS UNBOUNDED PRECEDING) "
                                   "AS balance FROM budget ORDER BY date, transactionID");
        transactionModel->setHeaderData(0, Qt::Horizontal, "transactionID");
        transactionModel->setHeaderData(1, Qt::Horizontal, "Date");
        transactionModel->setHeaderData(2, Qt::Horizontal, "Category");
        transactionModel->setHeaderData(3, Qt::Horizontal, "Subcategory");
        transactionModel->setHeaderData(4, Qt::Horizontal, "Amount");
        transactionModel->setHeaderData(5, Qt::Horizontal, "Balance");
        ui->transactionGroupBox->setTitle(QString("Table: All Transactions"));
    // else if currentTableSubcategory is empty, show all transactions matching category filter
    } else if (m_currentTableSubcategory == ""){
        query.prepare("SELECT transactionID, date, subcategory, amount, SUM(amount) "
                      "OVER (ORDER BY date, transactionID ROWS UNBOUNDED PRECEDING) "
                      "AS balance FROM budget "
                      "WHERE category = ? "
                      "ORDER BY date, transactionID" );
        query.bindValue(0, m_currentTableCategory);
        query.exec();
        transactionModel->setQuery(std::move(query));
        transactionModel->setHeaderData(0, Qt::Horizontal, "transactionID");
        transactionModel->setHeaderData(1, Qt::Horizontal, "Date");
        transactionModel->setHeaderData(2, Qt::Horizontal, "Subcategory");
        transactionModel->setHeaderData(3, Qt::Horizontal, "Amount");
        transactionModel->setHeaderData(4, Qt::Horizontal, "Balance");
        ui->transactionGroupBox->setTitle(QString("Table: %1 Transactions")
                                              .arg(m_currentTableCategory));
    // else show all transactions matching category and subcategory filters
    } else {
        query.prepare("SELECT transactionID, date, amount, SUM(amount) "
                      "OVER (ORDER BY date, transactionID ROWS UNBOUNDED PRECEDING) "
                      "AS balance FROM budget "
                      "WHERE category = ? "
                      "AND subcategory = ? "
                      "ORDER BY date, transactionID" );
        query.bindValue(0, m_currentTableCategory);
        query.bindValue(1, m_currentTableSubcategory);
        query.exec();
        transactionModel->setQuery(std::move(query));
        transactionModel->setHeaderData(0, Qt::Horizontal, "transactionID");
        transactionModel->setHeaderData(1, Qt::Horizontal, "Date");
        transactionModel->setHeaderData(2, Qt::Horizontal, "Amount");
        transactionModel->setHeaderData(3, Qt::Horizontal, "Balance");
        ui->transactionGroupBox->setTitle(QString("Table: %1 - %2 Transactions")
                                              .arg(m_currentTableCategory, m_currentTableSubcategory));
    }
    ui->transactionTableView->resizeColumnsToContents();
}

/**
 * @brief BudgetTracker::filterTable
 *        Updates table category and subcategory filters,
 *        and redraws table.
 */
void BudgetTracker::filterTable()
{
    m_currentTableCategory = ui->tableFilterCategoryLineEdit->text();
    m_currentTableSubcategory = ui->tableFilterSubcategoryLineEdit->text();
    drawTable();
    ui->tableFilterClearButton->setEnabled(true);
}

/**
 * @brief BudgetTracker::verifyTableFilter
 *        Enables table filter button if category field is not empty,
 *        disables button if empty.
 *
 *        Connected to table filter LineEdits textChanged signals.
 */
void BudgetTracker::verifyTableFilter()
{
    QString category = ui->tableFilterCategoryLineEdit->text();

    if (category != "") {
        ui->tableFilterFilterButton->setEnabled(true);
    } else {
        ui->tableFilterFilterButton->setEnabled(false);
    }
}

/**
 * @brief BudgetTracker::clearTableFilter
 *        Sets table category and subcategory filters to empty string,
 *        and redraws table (showing all transactions).
 */
void BudgetTracker::clearTableFilter()
{
    m_currentTableCategory = "";
    m_currentTableSubcategory = "";
    drawTable();
    // for some reason this needs to be called again
    /// BUG: columns are resized, but last column header section does not
    /// stretch (as set in initializeTable()) until any header has its size
    /// adjusted (only happens on clearTableFilter())
    ui->transactionTableView->resizeColumnsToContents();

    ui->tableFilterCategoryLineEdit->clear();
    ui->tableFilterSubcategoryLineEdit->clear();
    ui->tableFilterClearButton->setEnabled(false);
    ui->tableFilterCategoryLineEdit->setFocus();
}

/**
 * @brief BudgetTracker::initializePlot
 *        Sets up transaction plot axis information and graph settings,
 *        then calls drawPlot with no filters.
 */
void BudgetTracker::initializePlot()
{
    // create x-axis ticker
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("MMM dd\nyyyy");
    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);
    dateTimeTicker->setTickOrigin(QDateTime(QDate::currentDate(), QTime(0, 0)));

    // setup axes
    ui->transactionPlot->xAxis->setLabel("Date");
    ui->transactionPlot->xAxis->setTicker(dateTimeTicker);
    ui->transactionPlot->yAxis->setLabel("Amount");

    // initialize plot
    ui->transactionPlot->addGraph();
    ui->transactionPlot->graph()->setLineStyle(QCPGraph::lsNone);
    ui->transactionPlot->graph()->setScatterStyle(QCPScatterStyle::ScatterShape::ssDisc);
    ui->transactionPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    drawPlot();
}

/**
 * @brief BudgetTracker::drawPlot
 *        Draws plot base on current plot category and subcategory filters.
 */
void BudgetTracker::drawPlot()
{
    QSqlQuery query;
    // if currentPlotCategory is empty, plot all transactions
    if (m_currentPlotCategory == "") {
        query.prepare("SELECT date, amount "
                      "FROM budget "
                      "ORDER BY date, transactionID");
        ui->plotGroupBox->setTitle(QString("Plot: All Transactions"));
    // else if currentPlotSubcategory is empty, plot transactions matching category filter
    } else if (m_currentPlotSubcategory == ""){
        query.prepare("SELECT date, amount "
                      "FROM budget "
                      "WHERE category = ? "
                      "ORDER BY date, transactionID");
        query.bindValue(0, m_currentPlotCategory);
        ui->plotGroupBox->setTitle(QString("Plot: %1 Transactions").arg(m_currentPlotCategory));
    // else plot transactions matching category and subcategory filters
    } else {
        query.prepare("SELECT date, amount "
                      "FROM budget "
                      "WHERE category = ? "
                      "AND subcategory = ? "
                      "ORDER BY date, transactionID");
        query.bindValue(0, m_currentPlotCategory);
        query.bindValue(1, m_currentPlotSubcategory);
        ui->plotGroupBox->setTitle(QString("Plot: %1 - %2 Transactions").arg(m_currentPlotCategory, m_currentPlotSubcategory));
    }
    query.exec();

    // populate data vectors with relevant sql info
    QVector<double> dates;
    QVector<double> amounts;
    while (query.next()) {
        dates.push_back(QCPAxisTickerDateTime::dateTimeToKey(QDateTime::fromString(query.value(0).toString(), "yyyy/MM/dd")));
        amounts.push_back(query.value(1).toDouble());
    }

    // determine ranges for x (date) and y (amount)
    if (m_currentPlotCategory == "") {
        query.prepare("SELECT MIN(date), MAX(date), MIN(amount), MAX(amount) "
                      "FROM budget");
    } else if (m_currentPlotSubcategory == ""){
        query.prepare("SELECT MIN(date), MAX(date), MIN(amount), MAX(amount) "
                      "FROM budget "
                      "WHERE category = ?");
        query.bindValue(0, m_currentPlotCategory);
    } else {
        query.prepare("SELECT MIN(date), MAX(date), MIN(amount), MAX(amount) "
                      "FROM budget "
                      "WHERE category = ? "
                      "AND subcategory = ?");
        query.bindValue(0, m_currentPlotCategory);
        query.bindValue(1, m_currentPlotSubcategory);
    }
    query.exec();
    query.next();
    QDateTime minDate = QDateTime::fromString(query.value(0).toString(), "yyyy/MM/dd");
    QDateTime maxDate = QDateTime::fromString(query.value(1).toString(), "yyyy/MM/dd");
    double minAmount = query.value(2).toDouble();
    double maxAmount = query.value(3).toDouble();

    // create margin around X range
    double minRangeX;
    double maxRangeX;
    /// if min and max dates match, offset them
    /// (this is necessary to properly intitialize x-axis in this situation)
    if (minDate == maxDate) {
        minDate = minDate.addDays(-1);
        maxDate = maxDate.addDays(1);
    }
    minRangeX = (QCPAxisTickerDateTime::dateTimeToKey(minDate)
                        - (QCPAxisTickerDateTime::dateTimeToKey(maxDate)
                           - QCPAxisTickerDateTime::dateTimeToKey(minDate))/10);
    maxRangeX = (QCPAxisTickerDateTime::dateTimeToKey(maxDate)
                        + (QCPAxisTickerDateTime::dateTimeToKey(maxDate)
                           - QCPAxisTickerDateTime::dateTimeToKey(minDate))/10);
    ui->transactionPlot->xAxis->setRange(minRangeX, maxRangeX);

    // create margin around Y range
    double minRangeY;
    double maxRangeY;
    /// if min and max amounts match, offset them
    /// this is necessary to properly initialize y-axis in this situation
    if (minAmount == maxAmount) {
        minAmount -= 100;
        maxAmount += 100;
    }
    minRangeY = (minAmount - (maxAmount - minAmount)/10);
    maxRangeY = (maxAmount + (maxAmount -minAmount)/10);
    ui->transactionPlot->yAxis->setRange(minRangeY, maxRangeY);

    // plot data
    ui->transactionPlot->graph()->setData(dates, amounts);
    ui->transactionPlot->replot();
}

/**
 * @brief BudgetTracker::filterPlot
 *        Updates plot category and subcategory filters,
 *        then redraws plot.
 */
void BudgetTracker::filterPlot()
{
    m_currentPlotCategory = ui->plotFilterCategoryLineEdit->text();
    m_currentPlotSubcategory = ui->plotFilterSubcategoryLineEdit->text();
    drawPlot();
    ui->plotFilterClearButton->setEnabled(true);
}

/**
 * @brief BudgetTracker::verifyPlotFilter
 *        Enables plot filter button if category field is not empty,
 *        disables button if empty.
 *
 *        Connected to plot filter LineEdits textChanged signals.
 */
void BudgetTracker::verifyPlotFilter()
{
    QString category = ui->plotFilterCategoryLineEdit->text();

    if (category != "") {
        ui->plotFilterFilterButton->setEnabled(true);
    } else {
        ui->plotFilterFilterButton->setEnabled(false);
    }
}

/**
 * @brief BudgetTracker::clearPlotFilter
 *        Sets plot category and subcategory filters to empty string,
 *        and redraws plot (showing all transactions).
 */
void BudgetTracker::clearPlotFilter()
{
    m_currentPlotCategory = "";
    m_currentPlotSubcategory = "";
    drawPlot();
    ui->plotFilterCategoryLineEdit->clear();
    ui->plotFilterSubcategoryLineEdit->clear();
    ui->plotFilterClearButton->setEnabled(false);
    ui->plotFilterCategoryLineEdit->setFocus();
}

/**
 * @brief BudgetTracker::addEntry
 *        Adds new entry to budget database, updating table and plot.
 */
void BudgetTracker::addEntry()
{
    QString date = ui->entryDateDateEdit->date().toString("yyyy/MM/dd");
    QString category = ui->entryCategoryLineEdit->text();
    QString subcategory = ui->entrySubcategoryLineEdit->text();
    double amount = ui->entryAmountLineEdit->text().toDouble();

    QSqlQuery query;
    query.prepare("INSERT INTO budget "
                  "(transactionID, date, category, subcategory, amount) "
                  "VALUES (NULL, ?, ?, ?, ?)");
    query.bindValue(0, date);
    query.bindValue(1, category);
    query.bindValue(2, subcategory);
    query.bindValue(3, amount);
    query.exec();

    drawTable();
    drawPlot();

    ui->entryCategoryLineEdit->clear();
    ui->entrySubcategoryLineEdit->clear();
    ui->entryAmountLineEdit->clear();
    ui->entryAddButton->setEnabled(false);
    ui->entryDateDateEdit->setFocus();
}

/**
 * @brief BudgetTracker::verifyEntry
 *        Enables entry add button if category and subcategory are not empty
 *        and amount is not 0, disables button otherwise.
 *
 *        Connected to entry LineEdits textChanged signals.
 */
void BudgetTracker::verifyEntry()
{
    QString category = ui->entryCategoryLineEdit->text();
    QString subcategory = ui->entrySubcategoryLineEdit->text();
    double amount = ui->entryAmountLineEdit->text().toDouble();

    if (category != "" && subcategory != "" && amount != 0) {
        ui->entryAddButton->setEnabled(true);
    } else {
        ui->entryAddButton->setEnabled(false);
    }
}

/**
 * @brief BudgetTracker::removeEntry
 *        Removes selected entry from database.
 *
 *        Stub.
 */
void BudgetTracker::removeEntry()
{

}
