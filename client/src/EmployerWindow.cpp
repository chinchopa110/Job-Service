#include "EmployerWindow.hpp"
#include <QTabWidget>
#include <QHeaderView>
#include <QGroupBox>

EmployerWindow::EmployerWindow(ApiClient* client, QWidget* parent)
    : QWidget(parent)
    , client_(client)
{
    setupUi();
}

void EmployerWindow::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);

    auto* topRow  = new QHBoxLayout;
    auto* backBtn = new QPushButton("← Назад");
    backBtn->setMaximumWidth(120);
    connect(backBtn, &QPushButton::clicked, this, &EmployerWindow::backRequested);
    topRow->addWidget(backBtn);
    topRow->addStretch();
    mainLayout->addLayout(topRow);

    auto* tabs = new QTabWidget;
    mainLayout->addWidget(tabs);

    auto* profileTab = new QWidget;
    auto* jobsTab    = new QWidget;

    tabs->addTab(profileTab, "Профиль компании");
    tabs->addTab(jobsTab,    "Управление вакансиями");

    setupProfileTab(profileTab);
    setupJobsTab(jobsTab);
}

void EmployerWindow::setupProfileTab(QWidget* tab) {
    auto* layout = new QVBoxLayout(tab);
    auto* form   = new QFormLayout;

    companyNameEdit_  = new QLineEdit;
    emailEdit_        = new QLineEdit;
    descriptionEdit_  = new QTextEdit;
    descriptionEdit_->setMaximumHeight(100);
    cityEdit_         = new QLineEdit;

    form->addRow("Название компании:", companyNameEdit_);
    form->addRow("Email:",             emailEdit_);
    form->addRow("Описание:",          descriptionEdit_);
    form->addRow("Город:",             cityEdit_);

    createProfileBtn_   = new QPushButton("Создать профиль компании");
    profileStatusLabel_ = new QLabel;
    profileStatusLabel_->setWordWrap(true);

    connect(createProfileBtn_, &QPushButton::clicked, this, &EmployerWindow::onCreateProfile);

    layout->addLayout(form);
    layout->addWidget(createProfileBtn_);
    layout->addWidget(profileStatusLabel_);
    layout->addStretch();
}

void EmployerWindow::setupJobsTab(QWidget* tab) {
    auto* layout = new QVBoxLayout(tab);

    auto* postGroup  = new QGroupBox("Опубликовать вакансию");
    auto* postLayout = new QFormLayout(postGroup);

    jobTitleEdit_  = new QLineEdit;
    jobDescEdit_   = new QTextEdit;
    jobDescEdit_->setMaximumHeight(80);
    salarySpin_    = new QSpinBox;
    salarySpin_->setRange(0, 10000000);
    salarySpin_->setSingleStep(10000);
    jobCityEdit_   = new QLineEdit;
    remoteCheck_   = new QCheckBox("Удалённая работа");
    professionEdit_ = new QLineEdit;

    postLayout->addRow("Название:",   jobTitleEdit_);
    postLayout->addRow("Описание:",   jobDescEdit_);
    postLayout->addRow("Зарплата:",   salarySpin_);
    postLayout->addRow("Город:",      jobCityEdit_);
    postLayout->addRow("",            remoteCheck_);
    postLayout->addRow("Профессия:",  professionEdit_);

    postJobBtn_ = new QPushButton("📢 Опубликовать вакансию");
    connect(postJobBtn_, &QPushButton::clicked, this, &EmployerWindow::onPostJob);

    layout->addWidget(postGroup);
    layout->addWidget(postJobBtn_);

    layout->addWidget(new QLabel("Мои вакансии:"));
    myJobsTable_ = new QTableWidget(0, 5);
    myJobsTable_->setHorizontalHeaderLabels(
        {"ID", "Название", "Профессия", "Зарплата", "Город"});
    myJobsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myJobsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    myJobsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(myJobsTable_, &QTableWidget::itemSelectionChanged,
            this, &EmployerWindow::onJobSelectionChanged);

    auto* btnRow      = new QHBoxLayout;
    deleteJobBtn_     = new QPushButton("🗑 Удалить вакансию");
    viewRepliesBtn_   = new QPushButton("📬 Просмотреть отклики");
    deleteJobBtn_->setEnabled(false);
    viewRepliesBtn_->setEnabled(false);
    connect(deleteJobBtn_,   &QPushButton::clicked, this, &EmployerWindow::onDeleteJob);
    connect(viewRepliesBtn_, &QPushButton::clicked, this, &EmployerWindow::onViewReplies);
    btnRow->addWidget(deleteJobBtn_);
    btnRow->addWidget(viewRepliesBtn_);

    layout->addWidget(myJobsTable_);
    layout->addLayout(btnRow);

    layout->addWidget(new QLabel("Отклики на выбранную вакансию:"));
    repliesTable_ = new QTableWidget(0, 4);
    repliesTable_->setHorizontalHeaderLabels(
        {"ID отклика", "ID работника", "Дата", "Статус"});
    repliesTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    repliesTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(repliesTable_);
}

void EmployerWindow::onCreateProfile() {
    Models::Employer e;
    e.companyName = companyNameEdit_->text().trimmed();
    e.email       = emailEdit_->text().trimmed();
    e.description = descriptionEdit_->toPlainText().trimmed();
    e.city        = cityEdit_->text().trimmed();

    if (e.companyName.isEmpty() || e.email.isEmpty() || e.city.isEmpty()) {
        profileStatusLabel_->setText("❌ Заполните обязательные поля: название, email, город.");
        return;
    }

    createProfileBtn_->setEnabled(false);
    client_->createEmployer(e, [this](bool ok, size_t id, const QString& err) {
        createProfileBtn_->setEnabled(true);
        if (ok) {
            currentEmployerId_ = id;
            profileStatusLabel_->setText(
                QString("✅ Профиль создан. ID работодателя: %1").arg(id));
        } else {
            profileStatusLabel_->setText("❌ Ошибка: " + err);
        }
    });
}

void EmployerWindow::onPostJob() {
    if (currentEmployerId_ == 0) {
        QMessageBox::warning(this, "Профиль не создан",
                             "Сначала создайте профиль компании на вкладке «Профиль компании».");
        return;
    }

    Models::JobPosting job;
    job.employerId  = currentEmployerId_;
    job.title       = jobTitleEdit_->text().trimmed();
    job.description = jobDescEdit_->toPlainText().trimmed();
    job.salary      = salarySpin_->value();
    job.city        = jobCityEdit_->text().trimmed();
    job.isRemote    = remoteCheck_->isChecked();
    job.profession  = professionEdit_->text().trimmed();

    if (job.title.isEmpty() || job.profession.isEmpty() || job.city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните название, профессию и город.");
        return;
    }

    postJobBtn_->setEnabled(false);
    client_->createJobPosting(job, [this](bool ok, size_t id, const QString& err) {
        postJobBtn_->setEnabled(true);
        if (ok) {
            QMessageBox::information(this, "Успех",
                                     QString("Вакансия опубликована. ID: %1").arg(id));
            client_->getJobPostingsByEmployer(currentEmployerId_,
                [this](bool ok2, QVector<Models::JobPosting> jobs, const QString&) {
                    if (!ok2) return;
                    myJobsTable_->setRowCount(0);
                    for (const auto& j : jobs) {
                        int r = myJobsTable_->rowCount();
                        myJobsTable_->insertRow(r);
                        myJobsTable_->setItem(r, 0, new QTableWidgetItem(QString::number(j.id)));
                        myJobsTable_->setItem(r, 1, new QTableWidgetItem(j.title));
                        myJobsTable_->setItem(r, 2, new QTableWidgetItem(j.profession));
                        myJobsTable_->setItem(r, 3, new QTableWidgetItem(QString::number(j.salary)));
                        myJobsTable_->setItem(r, 4, new QTableWidgetItem(j.city));
                    }
                });
        } else {
            QMessageBox::warning(this, "Ошибка", err);
        }
    });
}

void EmployerWindow::onJobSelectionChanged() {
    bool hasSelection = !myJobsTable_->selectedItems().isEmpty();
    deleteJobBtn_->setEnabled(hasSelection);
    viewRepliesBtn_->setEnabled(hasSelection);
}

void EmployerWindow::onDeleteJob() {
    const int row = myJobsTable_->currentRow();
    if (row < 0) return;

    size_t jobId = static_cast<size_t>(myJobsTable_->item(row, 0)->text().toULongLong());

    auto answer = QMessageBox::question(this, "Подтверждение",
                                        QString("Удалить вакансию ID %1?").arg(jobId));
    if (answer != QMessageBox::Yes) return;

    deleteJobBtn_->setEnabled(false);
    client_->deleteJobPosting(jobId, [this, row](bool ok, const QString& err) {
        deleteJobBtn_->setEnabled(true);
        if (ok) {
            myJobsTable_->removeRow(row);
            repliesTable_->setRowCount(0);
            QMessageBox::information(this, "Успех", "Вакансия удалена.");
        } else {
            QMessageBox::warning(this, "Ошибка", err);
        }
    });
}

void EmployerWindow::onViewReplies() {
    const int row = myJobsTable_->currentRow();
    if (row < 0) return;

    size_t jobId = static_cast<size_t>(myJobsTable_->item(row, 0)->text().toULongLong());

    viewRepliesBtn_->setEnabled(false);
    client_->getRepliesByJob(jobId, [this](bool ok, QVector<Models::Reply> replies, const QString& err) {
        viewRepliesBtn_->setEnabled(true);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", err);
            return;
        }
        repliesTable_->setRowCount(0);
        for (const auto& r : replies) {
            int row = repliesTable_->rowCount();
            repliesTable_->insertRow(row);
            repliesTable_->setItem(row, 0, new QTableWidgetItem(QString::number(r.id)));
            repliesTable_->setItem(row, 1, new QTableWidgetItem(QString::number(r.workerId)));
            repliesTable_->setItem(row, 2, new QTableWidgetItem(r.replyDate));
            repliesTable_->setItem(row, 3, new QTableWidgetItem(r.status));
        }
    });
}
