#include "WorkerWindow.hpp"
#include <QTabWidget>
#include <QHeaderView>
#include <QSplitter>

WorkerWindow::WorkerWindow(ApiClient* client, QWidget* parent)
    : QWidget(parent)
    , client_(client)
{
    setupUi();
}

void WorkerWindow::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);

    auto* topRow   = new QHBoxLayout;
    auto* backBtn  = new QPushButton("← Назад");
    backBtn->setMaximumWidth(120);
    connect(backBtn, &QPushButton::clicked, this, &WorkerWindow::backRequested);
    topRow->addWidget(backBtn);
    topRow->addStretch();
    mainLayout->addLayout(topRow);

    auto* tabs = new QTabWidget;
    mainLayout->addWidget(tabs);

    auto* profileTab   = new QWidget;
    auto* jobsTab      = new QWidget;
    auto* employersTab = new QWidget;

    tabs->addTab(profileTab,   "Мой профиль");
    tabs->addTab(jobsTab,      "Поиск вакансий");
    tabs->addTab(employersTab, "Работодатели");

    setupProfileTab(profileTab);
    setupJobsTab(jobsTab);
    setupEmployersTab(employersTab);
}

void WorkerWindow::setupProfileTab(QWidget* tab) {
    auto* layout = new QVBoxLayout(tab);
    auto* form   = new QFormLayout;

    nameEdit_       = new QLineEdit;
    emailEdit_      = new QLineEdit;
    skillsEdit_     = new QLineEdit;
    skillsEdit_->setPlaceholderText("C++, Python, Qt (через запятую)");
    experienceSpin_ = new QSpinBox;
    experienceSpin_->setRange(0, 50);
    cityEdit_       = new QLineEdit;
    resumeEdit_     = new QTextEdit;
    resumeEdit_->setMaximumHeight(100);

    form->addRow("Имя:",          nameEdit_);
    form->addRow("Email:",        emailEdit_);
    form->addRow("Навыки:",       skillsEdit_);
    form->addRow("Опыт (лет):",   experienceSpin_);
    form->addRow("Город:",        cityEdit_);
    form->addRow("Резюме:",       resumeEdit_);

    createProfileBtn_   = new QPushButton("Создать профиль");
    profileStatusLabel_ = new QLabel;
    profileStatusLabel_->setWordWrap(true);

    connect(createProfileBtn_, &QPushButton::clicked, this, &WorkerWindow::onCreateProfile);

    layout->addLayout(form);
    layout->addWidget(createProfileBtn_);
    layout->addWidget(profileStatusLabel_);
    layout->addStretch();
}

void WorkerWindow::setupJobsTab(QWidget* tab) {
    auto* layout = new QVBoxLayout(tab);

    auto* filterGroup  = new QGroupBox("Фильтры");
    auto* filterLayout = new QFormLayout(filterGroup);

    filterCityEdit_       = new QLineEdit;
    filterProfessionEdit_ = new QLineEdit;
    filterMinSalarySpin_  = new QSpinBox;
    filterMinSalarySpin_->setRange(0, 10000000);
    filterMinSalarySpin_->setSingleStep(10000);
    filterMaxSalarySpin_  = new QSpinBox;
    filterMaxSalarySpin_->setRange(0, 10000000);
    filterMaxSalarySpin_->setSingleStep(10000);
    filterRemoteCheck_    = new QCheckBox("Только удалённая работа");

    filterLayout->addRow("Город:",         filterCityEdit_);
    filterLayout->addRow("Профессия:",     filterProfessionEdit_);
    filterLayout->addRow("Зарплата от:",   filterMinSalarySpin_);
    filterLayout->addRow("Зарплата до:",   filterMaxSalarySpin_);
    filterLayout->addRow("",               filterRemoteCheck_);

    searchBtn_ = new QPushButton("🔍 Найти вакансии");
    connect(searchBtn_, &QPushButton::clicked, this, &WorkerWindow::onSearchJobs);

    layout->addWidget(filterGroup);
    layout->addWidget(searchBtn_);

    jobsTable_ = new QTableWidget(0, 7);
    jobsTable_->setHorizontalHeaderLabels(
        {"ID", "Название", "Профессия", "Зарплата", "Город", "Удалённо", "Дата"});
    jobsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    jobsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    jobsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(jobsTable_, &QTableWidget::itemSelectionChanged,
            this, &WorkerWindow::onJobSelectionChanged);

    replyBtn_ = new QPushButton("📩 Откликнуться на вакансию");
    replyBtn_->setEnabled(false);
    connect(replyBtn_, &QPushButton::clicked, this, &WorkerWindow::onReplyToJob);

    layout->addWidget(jobsTable_);
    layout->addWidget(replyBtn_);
}

void WorkerWindow::setupEmployersTab(QWidget* tab) {
    auto* layout = new QVBoxLayout(tab);

    loadEmployersBtn_ = new QPushButton("🔄 Загрузить работодателей");
    connect(loadEmployersBtn_, &QPushButton::clicked, this, &WorkerWindow::onLoadEmployers);

    employersTable_ = new QTableWidget(0, 4);
    employersTable_->setHorizontalHeaderLabels({"ID", "Компания", "Email", "Город"});
    employersTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    employersTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    employersTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    viewEmployerJobsBtn_ = new QPushButton("📋 Вакансии работодателя");
    viewEmployerJobsBtn_->setEnabled(false);
    connect(viewEmployerJobsBtn_, &QPushButton::clicked, this, &WorkerWindow::onViewEmployerJobs);
    connect(employersTable_, &QTableWidget::itemSelectionChanged, this, [this]() {
        viewEmployerJobsBtn_->setEnabled(!employersTable_->selectedItems().isEmpty());
    });

    employerJobsTable_ = new QTableWidget(0, 6);
    employerJobsTable_->setHorizontalHeaderLabels(
        {"ID", "Название", "Профессия", "Зарплата", "Город", "Удалённо"});
    employerJobsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    employerJobsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(loadEmployersBtn_);
    layout->addWidget(employersTable_);
    layout->addWidget(viewEmployerJobsBtn_);
    layout->addWidget(new QLabel("Вакансии выбранного работодателя:"));
    layout->addWidget(employerJobsTable_);
}

void WorkerWindow::onCreateProfile() {
    Models::Worker w;
    w.name       = nameEdit_->text().trimmed();
    w.email      = emailEdit_->text().trimmed();
    w.experience = experienceSpin_->value();
    w.city       = cityEdit_->text().trimmed();
    w.resume     = resumeEdit_->toPlainText().trimmed();

    const QString skillsRaw = skillsEdit_->text();
    for (const QString& s : skillsRaw.split(',', Qt::SkipEmptyParts)) {
        w.skills << s.trimmed();
    }

    if (w.name.isEmpty() || w.email.isEmpty() || w.city.isEmpty()) {
        profileStatusLabel_->setText("❌ Заполните обязательные поля: имя, email, город.");
        return;
    }

    createProfileBtn_->setEnabled(false);
    client_->createWorker(w, [this](bool ok, size_t id, const QString& err) {
        createProfileBtn_->setEnabled(true);
        if (ok) {
            currentWorkerId_ = id;
            profileStatusLabel_->setText(
                QString("✅ Профиль создан. ID работника: %1").arg(id));
        } else {
            profileStatusLabel_->setText("❌ Ошибка: " + err);
        }
    });
}

void WorkerWindow::onSearchJobs() {
    Models::JobFilter filter;
    filter.city       = filterCityEdit_->text().trimmed();
    filter.profession = filterProfessionEdit_->text().trimmed();
    filter.minSalary  = filterMinSalarySpin_->value();
    filter.maxSalary  = filterMaxSalarySpin_->value();
    filter.remoteOnly = filterRemoteCheck_->isChecked();

    searchBtn_->setEnabled(false);
    client_->searchJobPostings(filter, [this](bool ok, QVector<Models::JobPosting> jobs, const QString& err) {
        searchBtn_->setEnabled(true);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", err);
            return;
        }
        jobsTable_->setRowCount(0);
        for (const auto& job : jobs) {
            int row = jobsTable_->rowCount();
            jobsTable_->insertRow(row);
            jobsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(job.id)));
            jobsTable_->setItem(row, 1, new QTableWidgetItem(job.title));
            jobsTable_->setItem(row, 2, new QTableWidgetItem(job.profession));
            jobsTable_->setItem(row, 3, new QTableWidgetItem(QString::number(job.salary)));
            jobsTable_->setItem(row, 4, new QTableWidgetItem(job.city));
            jobsTable_->setItem(row, 5, new QTableWidgetItem(job.isRemote ? "Да" : "Нет"));
            jobsTable_->setItem(row, 6, new QTableWidgetItem(job.publishDate));
        }
    });
}

void WorkerWindow::onJobSelectionChanged() {
    replyBtn_->setEnabled(!jobsTable_->selectedItems().isEmpty());
}

void WorkerWindow::onReplyToJob() {
    if (currentWorkerId_ == 0) {
        QMessageBox::warning(this, "Профиль не создан",
                             "Сначала создайте профиль работника на вкладке «Мой профиль».");
        return;
    }
    const int row = jobsTable_->currentRow();
    if (row < 0) return;

    size_t jobId = static_cast<size_t>(jobsTable_->item(row, 0)->text().toULongLong());

    replyBtn_->setEnabled(false);
    client_->createReply(jobId, currentWorkerId_, [this](bool ok, size_t id, const QString& err) {
        replyBtn_->setEnabled(true);
        if (ok) {
            QMessageBox::information(this, "Успех",
                                     QString("Отклик отправлен. ID отклика: %1").arg(id));
        } else {
            QMessageBox::warning(this, "Ошибка", err);
        }
    });
}

void WorkerWindow::onLoadEmployers() {
    loadEmployersBtn_->setEnabled(false);
    client_->getAllEmployers([this](bool ok, QVector<Models::Employer> employers, const QString& err) {
        loadEmployersBtn_->setEnabled(true);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", err);
            return;
        }
        employersTable_->setRowCount(0);
        for (const auto& emp : employers) {
            int row = employersTable_->rowCount();
            employersTable_->insertRow(row);
            employersTable_->setItem(row, 0, new QTableWidgetItem(QString::number(emp.id)));
            employersTable_->setItem(row, 1, new QTableWidgetItem(emp.companyName));
            employersTable_->setItem(row, 2, new QTableWidgetItem(emp.email));
            employersTable_->setItem(row, 3, new QTableWidgetItem(emp.city));
        }
    });
}

void WorkerWindow::onViewEmployerJobs() {
    const int row = employersTable_->currentRow();
    if (row < 0) return;

    size_t empId = static_cast<size_t>(employersTable_->item(row, 0)->text().toULongLong());

    viewEmployerJobsBtn_->setEnabled(false);
    client_->getJobPostingsByEmployer(empId, [this](bool ok, QVector<Models::JobPosting> jobs, const QString& err) {
        viewEmployerJobsBtn_->setEnabled(true);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", err);
            return;
        }
        employerJobsTable_->setRowCount(0);
        for (const auto& job : jobs) {
            int r = employerJobsTable_->rowCount();
            employerJobsTable_->insertRow(r);
            employerJobsTable_->setItem(r, 0, new QTableWidgetItem(QString::number(job.id)));
            employerJobsTable_->setItem(r, 1, new QTableWidgetItem(job.title));
            employerJobsTable_->setItem(r, 2, new QTableWidgetItem(job.profession));
            employerJobsTable_->setItem(r, 3, new QTableWidgetItem(QString::number(job.salary)));
            employerJobsTable_->setItem(r, 4, new QTableWidgetItem(job.city));
            employerJobsTable_->setItem(r, 5, new QTableWidgetItem(job.isRemote ? "Да" : "Нет"));
        }
    });
}
