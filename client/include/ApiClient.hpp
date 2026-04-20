#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <functional>
#include "Models.hpp"

using JsonCallback    = std::function<void(bool ok, const QJsonDocument& doc, const QString& error)>;
using IdCallback      = std::function<void(bool ok, size_t id,              const QString& error)>;
using SuccessCallback = std::function<void(bool ok,                          const QString& error)>;

class ApiClient : public QObject {
    Q_OBJECT
public:
    explicit ApiClient(const QString& baseUrl = "http://localhost:8080", QObject* parent = nullptr);

    void setBaseUrl(const QString& url);
    QString baseUrl() const;

    void createWorker(const Models::Worker& worker, IdCallback cb);

    void createEmployer(const Models::Employer& employer, IdCallback cb);
    void getAllEmployers(std::function<void(bool, QVector<Models::Employer>, QString)> cb);

    void createJobPosting(const Models::JobPosting& job, IdCallback cb);
    void deleteJobPosting(size_t jobId, SuccessCallback cb);
    void searchJobPostings(const Models::JobFilter& filter,
                           std::function<void(bool, QVector<Models::JobPosting>, QString)> cb);
    void getJobPostingsByEmployer(size_t employerId,
                                  std::function<void(bool, QVector<Models::JobPosting>, QString)> cb);

    void createReply(size_t jobPostingId, size_t workerId, IdCallback cb);
    void getRepliesByJob(size_t jobId,
                         std::function<void(bool, QVector<Models::Reply>, QString)> cb);

private:
    QNetworkReply* postJson(const QString& path, const QJsonObject& body);
    QNetworkReply* getRequest(const QString& path);
    QNetworkReply* deleteRequest(const QString& path);

    void connectReply(QNetworkReply* reply, JsonCallback cb);

    static QVector<Models::Employer>   parseEmployers(const QJsonArray& arr);
    static QVector<Models::JobPosting> parseJobPostings(const QJsonArray& arr);
    static QVector<Models::Reply>      parseReplies(const QJsonArray& arr);

    QNetworkAccessManager* manager_;
    QString baseUrl_;
};
