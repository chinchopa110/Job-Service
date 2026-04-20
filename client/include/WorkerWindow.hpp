#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QMessageBox>
#include "ApiClient.hpp"

class WorkerWindow : public QWidget {
    Q_OBJECT
public:
    explicit WorkerWindow(ApiClient* client, QWidget* parent = nullptr);

signals:
    void backRequested();

private slots:
    void onCreateProfile();
    void onSearchJobs();
    void onLoadEmployers();
    void onReplyToJob();
    void onJobSelectionChanged();
    void onViewEmployerJobs();

private:
    void setupUi();
    void setupProfileTab(QWidget* tab);
    void setupJobsTab(QWidget* tab);
    void setupEmployersTab(QWidget* tab);

    ApiClient* client_;
    size_t     currentWorkerId_ = 0;

    // Profile tab
    QLineEdit* nameEdit_;
    QLineEdit* emailEdit_;
    QLineEdit* skillsEdit_;   // comma-separated
    QSpinBox*  experienceSpin_;
    QLineEdit* cityEdit_;
    QTextEdit* resumeEdit_;
    QPushButton* createProfileBtn_;
    QLabel*    profileStatusLabel_;

    // Jobs tab
    QLineEdit* filterCityEdit_;
    QLineEdit* filterProfessionEdit_;
    QSpinBox*  filterMinSalarySpin_;
    QSpinBox*  filterMaxSalarySpin_;
    QCheckBox* filterRemoteCheck_;
    QPushButton* searchBtn_;
    QTableWidget* jobsTable_;
    QPushButton* replyBtn_;

    // Employers tab
    QPushButton*  loadEmployersBtn_;
    QTableWidget* employersTable_;
    QPushButton*  viewEmployerJobsBtn_;
    QTableWidget* employerJobsTable_;
};
