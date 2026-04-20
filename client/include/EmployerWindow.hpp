#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QMessageBox>
#include "ApiClient.hpp"

class EmployerWindow : public QWidget {
    Q_OBJECT
public:
    explicit EmployerWindow(ApiClient* client, QWidget* parent = nullptr);

signals:
    void backRequested();

private slots:
    void onCreateProfile();
    void onPostJob();
    void onDeleteJob();
    void onViewReplies();
    void onJobSelectionChanged();

private:
    void setupUi();
    void setupProfileTab(QWidget* tab);
    void setupJobsTab(QWidget* tab);

    ApiClient* client_;
    size_t     currentEmployerId_ = 0;

    // Profile tab
    QLineEdit*   companyNameEdit_;
    QLineEdit*   emailEdit_;
    QTextEdit*   descriptionEdit_;
    QLineEdit*   cityEdit_;
    QPushButton* createProfileBtn_;
    QLabel*      profileStatusLabel_;

    // Jobs tab
    QLineEdit*   jobTitleEdit_;
    QTextEdit*   jobDescEdit_;
    QSpinBox*    salarySpin_;
    QLineEdit*   jobCityEdit_;
    QCheckBox*   remoteCheck_;
    QLineEdit*   professionEdit_;
    QPushButton* postJobBtn_;

    QTableWidget* myJobsTable_;
    QPushButton*  deleteJobBtn_;
    QPushButton*  viewRepliesBtn_;

    QTableWidget* repliesTable_;
};
