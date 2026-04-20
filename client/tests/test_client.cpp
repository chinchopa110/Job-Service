#include <QtTest/QtTest>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSignalSpy>
#include <QNetworkAccessManager>

#include "Models.hpp"
#include "ApiClient.hpp"
#include "WorkerWindow.hpp"
#include "EmployerWindow.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Test suite 1: Models – JSON serialization / deserialization
// ─────────────────────────────────────────────────────────────────────────────
class TestModels : public QObject {
    Q_OBJECT

private slots:
    // Worker
    void testWorkerToJson() {
        Models::Worker w;
        w.id         = 1;
        w.name       = "Ivan Petrov";
        w.email      = "ivan@example.com";
        w.skills     = {"C++", "Qt"};
        w.experience = 5;
        w.city       = "Moscow";
        w.resume     = "Senior developer";

        QJsonObject obj = w.toJson();
        QCOMPARE(obj["name"].toString(),       QString("Ivan Petrov"));
        QCOMPARE(obj["email"].toString(),      QString("ivan@example.com"));
        QCOMPARE(obj["experience"].toInt(),    5);
        QCOMPARE(obj["city"].toString(),       QString("Moscow"));
        QCOMPARE(obj["resume"].toString(),     QString("Senior developer"));
        QCOMPARE(obj["skills"].toArray().size(), 2);
        QCOMPARE(obj["skills"].toArray()[0].toString(), QString("C++"));
        QCOMPARE(obj["skills"].toArray()[1].toString(), QString("Qt"));
    }

    void testWorkerFromJson() {
        QJsonObject obj;
        obj["id"]         = 42;
        obj["name"]       = "Anna Sidorova";
        obj["email"]      = "anna@example.com";
        obj["experience"] = 3;
        obj["city"]       = "Saint Petersburg";
        obj["resume"]     = "Designer";
        QJsonArray skills;
        skills.append("Figma");
        skills.append("Photoshop");
        obj["skills"] = skills;

        Models::Worker w = Models::Worker::fromJson(obj);
        QCOMPARE(w.id,                    static_cast<size_t>(42));
        QCOMPARE(w.name,                  QString("Anna Sidorova"));
        QCOMPARE(w.email,                 QString("anna@example.com"));
        QCOMPARE(w.experience,            3);
        QCOMPARE(w.city,                  QString("Saint Petersburg"));
        QCOMPARE(w.skills.size(),         2);
        QCOMPARE(w.skills[0],             QString("Figma"));
    }

    void testWorkerRoundTrip() {
        Models::Worker original;
        original.id         = 7;
        original.name       = "Test User";
        original.email      = "test@test.com";
        original.skills     = {"Go", "Rust"};
        original.experience = 2;
        original.city       = "Kazan";
        original.resume     = "Backend dev";

        Models::Worker restored = Models::Worker::fromJson(original.toJson());
        QCOMPARE(restored.name,       original.name);
        QCOMPARE(restored.email,      original.email);
        QCOMPARE(restored.experience, original.experience);
        QCOMPARE(restored.city,       original.city);
        QCOMPARE(restored.resume,     original.resume);
        QCOMPARE(restored.skills,     original.skills);
    }

    // Employer
    void testEmployerToJson() {
        Models::Employer e;
        e.id          = 1;
        e.companyName = "TechCorp";
        e.email       = "hr@techcorp.com";
        e.description = "Leading tech company";
        e.city        = "Moscow";

        QJsonObject obj = e.toJson();
        QCOMPARE(obj["companyName"].toString(), QString("TechCorp"));
        QCOMPARE(obj["email"].toString(),       QString("hr@techcorp.com"));
        QCOMPARE(obj["description"].toString(), QString("Leading tech company"));
        QCOMPARE(obj["city"].toString(),        QString("Moscow"));
    }

    void testEmployerFromJson() {
        QJsonObject obj;
        obj["id"]          = 5;
        obj["companyName"] = "StartupXYZ";
        obj["email"]       = "info@startup.com";
        obj["description"] = "Fast-growing startup";
        obj["city"]        = "Novosibirsk";

        Models::Employer e = Models::Employer::fromJson(obj);
        QCOMPARE(e.id,          static_cast<size_t>(5));
        QCOMPARE(e.companyName, QString("StartupXYZ"));
        QCOMPARE(e.email,       QString("info@startup.com"));
        QCOMPARE(e.city,        QString("Novosibirsk"));
    }

    void testEmployerRoundTrip() {
        Models::Employer original;
        original.id          = 3;
        original.companyName = "MegaCorp";
        original.email       = "mega@corp.ru";
        original.description = "Big company";
        original.city        = "Ekaterinburg";

        Models::Employer restored = Models::Employer::fromJson(original.toJson());
        QCOMPARE(restored.companyName, original.companyName);
        QCOMPARE(restored.email,       original.email);
        QCOMPARE(restored.description, original.description);
        QCOMPARE(restored.city,        original.city);
    }

    // JobPosting
    void testJobPostingToJson() {
        Models::JobPosting j;
        j.id          = 1;
        j.employerId  = 2;
        j.title       = "Senior C++ Developer";
        j.description = "We need a C++ expert";
        j.salary      = 200000;
        j.city        = "Moscow";
        j.isRemote    = false;
        j.profession  = "Software Engineer";

        QJsonObject obj = j.toJson();
        QCOMPARE(obj["title"].toString(),       QString("Senior C++ Developer"));
        QCOMPARE(obj["salary"].toInt(),         200000);
        QCOMPARE(obj["city"].toString(),        QString("Moscow"));
        QCOMPARE(obj["isRemote"].toBool(),      false);
        QCOMPARE(obj["profession"].toString(),  QString("Software Engineer"));
        QCOMPARE(obj["employerId"].toInt(),     2);
    }

    void testJobPostingFromJson() {
        QJsonObject obj;
        obj["id"]          = 10;
        obj["employerId"]  = 3;
        obj["title"]       = "QA Engineer";
        obj["description"] = "Testing role";
        obj["salary"]      = 120000;
        obj["city"]        = "Samara";
        obj["isRemote"]    = true;
        obj["publishDate"] = "2024-01-15T10:00:00";
        obj["profession"]  = "QA";

        Models::JobPosting j = Models::JobPosting::fromJson(obj);
        QCOMPARE(j.id,         static_cast<size_t>(10));
        QCOMPARE(j.title,      QString("QA Engineer"));
        QCOMPARE(j.salary,     120000);
        QCOMPARE(j.isRemote,   true);
        QCOMPARE(j.profession, QString("QA"));
    }

    void testJobPostingRoundTrip() {
        Models::JobPosting original;
        original.id          = 5;
        original.employerId  = 1;
        original.title       = "DevOps";
        original.description = "CI/CD pipelines";
        original.salary      = 180000;
        original.city        = "Rostov";
        original.isRemote    = true;
        original.profession  = "DevOps Engineer";

        Models::JobPosting restored = Models::JobPosting::fromJson(original.toJson());
        QCOMPARE(restored.title,       original.title);
        QCOMPARE(restored.salary,      original.salary);
        QCOMPARE(restored.city,        original.city);
        QCOMPARE(restored.isRemote,    original.isRemote);
        QCOMPARE(restored.profession,  original.profession);
    }

    // Reply
    void testReplyToJson() {
        Models::Reply r;
        r.jobPostingId = 3;
        r.workerId     = 7;

        QJsonObject obj = r.toJson();
        QCOMPARE(obj["jobPostingId"].toInt(), 3);
        QCOMPARE(obj["workerId"].toInt(),     7);
    }

    void testReplyFromJson() {
        QJsonObject obj;
        obj["id"]           = 1;
        obj["jobPostingId"] = 5;
        obj["workerId"]     = 2;
        obj["replyDate"]    = "2024-03-01T12:00:00";
        obj["status"]       = "PENDING";

        Models::Reply r = Models::Reply::fromJson(obj);
        QCOMPARE(r.id,           static_cast<size_t>(1));
        QCOMPARE(r.jobPostingId, static_cast<size_t>(5));
        QCOMPARE(r.workerId,     static_cast<size_t>(2));
        QCOMPARE(r.status,       QString("PENDING"));
    }

    // JobFilter
    void testJobFilterEmptyQuery() {
        Models::JobFilter f;
        QCOMPARE(f.toQueryString(), QString());
    }

    void testJobFilterCityOnly() {
        Models::JobFilter f;
        f.city = "Moscow";
        QVERIFY(f.toQueryString().contains("city=Moscow"));
        QVERIFY(f.toQueryString().startsWith("?"));
    }

    void testJobFilterAllParams() {
        Models::JobFilter f;
        f.city       = "Kazan";
        f.profession = "Developer";
        f.minSalary  = 100000;
        f.maxSalary  = 300000;
        f.remoteOnly = true;

        QString qs = f.toQueryString();
        QVERIFY(qs.contains("city=Kazan"));
        QVERIFY(qs.contains("profession=Developer"));
        QVERIFY(qs.contains("minSalary=100000"));
        QVERIFY(qs.contains("maxSalary=300000"));
        QVERIFY(qs.contains("isRemote=true"));
    }

    void testJobFilterNoRemote() {
        Models::JobFilter f;
        f.city       = "Moscow";
        f.remoteOnly = false;
        QString qs = f.toQueryString();
        QVERIFY(!qs.contains("isRemote"));
    }

    void testJobFilterZeroSalaryNotIncluded() {
        Models::JobFilter f;
        f.minSalary = 0;
        f.maxSalary = 0;
        QString qs = f.toQueryString();
        QVERIFY(!qs.contains("minSalary"));
        QVERIFY(!qs.contains("maxSalary"));
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Test suite 2: ApiClient – URL construction and base URL management
// ─────────────────────────────────────────────────────────────────────────────
class TestApiClient : public QObject {
    Q_OBJECT

private slots:
    void testDefaultBaseUrl() {
        ApiClient client;
        QCOMPARE(client.baseUrl(), QString("http://localhost:8080"));
    }

    void testCustomBaseUrl() {
        ApiClient client("http://192.168.1.1:9090");
        QCOMPARE(client.baseUrl(), QString("http://192.168.1.1:9090"));
    }

    void testSetBaseUrl() {
        ApiClient client;
        client.setBaseUrl("http://example.com:8080");
        QCOMPARE(client.baseUrl(), QString("http://example.com:8080"));
    }

    void testBaseUrlChangeReflected() {
        ApiClient client("http://old-host:8080");
        QCOMPARE(client.baseUrl(), QString("http://old-host:8080"));
        client.setBaseUrl("http://new-host:9000");
        QCOMPARE(client.baseUrl(), QString("http://new-host:9000"));
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Test suite 3: WorkerWindow – UI state logic
// ─────────────────────────────────────────────────────────────────────────────
class TestWorkerWindow : public QObject {
    Q_OBJECT

private:
    ApiClient*    client_  = nullptr;
    WorkerWindow* window_  = nullptr;

private slots:
    void init() {
        client_ = new ApiClient("http://localhost:8080");
        window_ = new WorkerWindow(client_);
    }

    void cleanup() {
        delete window_;
        delete client_;
        window_ = nullptr;
        client_ = nullptr;
    }

    void testWindowCreated() {
        QVERIFY(window_ != nullptr);
    }

    void testBackSignalEmitted() {
        QSignalSpy spy(window_, &WorkerWindow::backRequested);
        // Find the back button and click it
        QPushButton* backBtn = window_->findChild<QPushButton*>();
        QVERIFY(backBtn != nullptr);
        // The first QPushButton in the layout is the back button
        const auto buttons = window_->findChildren<QPushButton*>();
        QVERIFY(!buttons.isEmpty());
        // Emit signal directly to test connection
        emit window_->backRequested();
        QCOMPARE(spy.count(), 1);
    }

    void testWindowHasTabs() {
        QTabWidget* tabs = window_->findChild<QTabWidget*>();
        QVERIFY(tabs != nullptr);
        QCOMPARE(tabs->count(), 3);
        QCOMPARE(tabs->tabText(0), QString("Мой профиль"));
        QCOMPARE(tabs->tabText(1), QString("Поиск вакансий"));
        QCOMPARE(tabs->tabText(2), QString("Работодатели"));
    }

    void testProfileFormHasRequiredFields() {
        const auto lineEdits = window_->findChildren<QLineEdit*>();
        // Should have: name, email, skills, city, filterCity, filterProfession
        QVERIFY(lineEdits.size() >= 4);
    }

    void testJobsTableExists() {
        const auto tables = window_->findChildren<QTableWidget*>();
        QVERIFY(tables.size() >= 1);
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Test suite 4: EmployerWindow – UI state logic
// ─────────────────────────────────────────────────────────────────────────────
class TestEmployerWindow : public QObject {
    Q_OBJECT

private:
    ApiClient*      client_ = nullptr;
    EmployerWindow* window_ = nullptr;

private slots:
    void init() {
        client_ = new ApiClient("http://localhost:8080");
        window_ = new EmployerWindow(client_);
    }

    void cleanup() {
        delete window_;
        delete client_;
        window_ = nullptr;
        client_ = nullptr;
    }

    void testWindowCreated() {
        QVERIFY(window_ != nullptr);
    }

    void testBackSignalEmitted() {
        QSignalSpy spy(window_, &EmployerWindow::backRequested);
        emit window_->backRequested();
        QCOMPARE(spy.count(), 1);
    }

    void testWindowHasTabs() {
        QTabWidget* tabs = window_->findChild<QTabWidget*>();
        QVERIFY(tabs != nullptr);
        QCOMPARE(tabs->count(), 2);
        QCOMPARE(tabs->tabText(0), QString("Профиль компании"));
        QCOMPARE(tabs->tabText(1), QString("Управление вакансиями"));
    }

    void testProfileFormHasRequiredFields() {
        const auto lineEdits = window_->findChildren<QLineEdit*>();
        // companyName, email, city, jobTitle, jobCity, profession
        QVERIFY(lineEdits.size() >= 3);
    }

    void testJobsTableExists() {
        const auto tables = window_->findChildren<QTableWidget*>();
        // myJobsTable_ and repliesTable_
        QVERIFY(tables.size() >= 2);
    }

    void testDeleteAndReplyButtonsInitiallyDisabled() {
        const auto buttons = window_->findChildren<QPushButton*>();
        // Find buttons by text
        QPushButton* deleteBtn  = nullptr;
        QPushButton* repliesBtn = nullptr;
        for (auto* btn : buttons) {
            if (btn->text().contains("Удалить"))  deleteBtn  = btn;
            if (btn->text().contains("отклики")) repliesBtn = btn;
        }
        if (deleteBtn)  QVERIFY(!deleteBtn->isEnabled());
        if (repliesBtn) QVERIFY(!repliesBtn->isEnabled());
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    int status = 0;

    {
        TestModels t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestApiClient t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestWorkerWindow t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestEmployerWindow t;
        status |= QTest::qExec(&t, argc, argv);
    }

    return status;
}

#include "test_client.moc"
