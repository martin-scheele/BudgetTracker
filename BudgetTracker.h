#pragma once

#include "User.h"

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class BudgetTracker;
}
/**
 * @brief The BudgetTracker class
 *        Transaction plot/table viewer and manipulator.
 *
 *        Shows plot and table of user transactions, allowing
 *        for manipulation of transaction data and plot/table
 *        filtration.
 */
class BudgetTracker : public QWidget
{
    Q_OBJECT

public:
    // constructors
    explicit BudgetTracker(std::shared_ptr<User> user,
                           QWidget *parent = nullptr);
    // destructors
    ~BudgetTracker();

private slots:
    // entry-related slots
    void addEntry();
    void verifyEntry();
    void removeEntry();

    // table-related slots
    void filterTable();
    void verifyTableFilter();
    void clearTableFilter();

    // plot-related slots
    void filterPlot();
    void verifyPlotFilter();
    void clearPlotFilter();

private:
    Ui::BudgetTracker *ui;
    QSqlQueryModel *transactionModel;       // model for transactionTableView
    std::shared_ptr<User> m_user;           // current user

    QString m_currentPlotCategory = "";     // current plot category filter string
    QString m_currentPlotSubcategory = "";  // current plot subcategory filter string
    QString m_currentTableCategory = "";    // current table category filter string
    QString m_currentTableSubcategory = ""; // current table subcategory filter string

    // non-slot functions
    void setupDatabase(const std::shared_ptr<const User> user);
    void initializeTable();
    void drawTable();
    void initializePlot();
    void drawPlot();
};
