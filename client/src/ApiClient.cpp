#include "ApiClient.hpp"
#include <QNetworkRequest>
#include <QJsonDocument>

ApiClient::ApiClient(const QString& baseUrl, QObject* parent)
    : QObject(parent)
    , manager_(new QNetworkAccessManager(this))
    , baseUrl_(baseUrl)
{}

void ApiClient::setBaseUrl(const QString& url) { baseUrl_ = url; }
QString ApiClient::baseUrl() const { return baseUrl_; }

QNetworkReply* ApiClient::postJson(const QString& path, const QJsonObject& body) {
    QNetworkRequest req(QUrl(baseUrl_ + path));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return manager_->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

QNetworkReply* ApiClient::getRequest(const QString& path) {
    QNetworkRequest req(QUrl(baseUrl_ + path));
    return manager_->get(req);
}

QNetworkReply* ApiClient::deleteRequest(const QString& path) {
    QNetworkRequest req(QUrl(baseUrl_ + path));
    return manager_->deleteResource(req);
}

void ApiClient::connectReply(QNetworkReply* reply, JsonCallback cb) {
    connect(reply, &QNetworkReply::finished, this, [reply, cb]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            cb(false, QJsonDocument(), reply->errorString());
            return;
        }
        QJsonParseError parseErr;
        auto doc = QJsonDocument::fromJson(reply->readAll(), &parseErr);
        if (parseErr.error != QJsonParseError::NoError) {
            cb(false, QJsonDocument(), parseErr.errorString());
            return;
        }
        cb(true, doc, QString());
    });
}

QVector<Models::Employer> ApiClient::parseEmployers(const QJsonArray& arr) {
    QVector<Models::Employer> result;
    result.reserve(arr.size());
    for (const auto& v : arr) result.push_back(Models::Employer::fromJson(v.toObject()));
    return result;
}

QVector<Models::JobPosting> ApiClient::parseJobPostings(const QJsonArray& arr) {
    QVector<Models::JobPosting> result;
    result.reserve(arr.size());
    for (const auto& v : arr) result.push_back(Models::JobPosting::fromJson(v.toObject()));
    return result;
}

QVector<Models::Reply> ApiClient::parseReplies(const QJsonArray& arr) {
    QVector<Models::Reply> result;
    result.reserve(arr.size());
    for (const auto& v : arr) result.push_back(Models::Reply::fromJson(v.toObject()));
    return result;
}

void ApiClient::createWorker(const Models::Worker& worker, IdCallback cb) {
    auto* reply = postJson("/api/workers", worker.toJson());
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, 0, err); return; }
        size_t id = static_cast<size_t>(doc.object()["id"].toInt());
        cb(true, id, QString());
    });
}

void ApiClient::createEmployer(const Models::Employer& employer, IdCallback cb) {
    auto* reply = postJson("/api/employers", employer.toJson());
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, 0, err); return; }
        size_t id = static_cast<size_t>(doc.object()["id"].toInt());
        cb(true, id, QString());
    });
}

void ApiClient::getAllEmployers(std::function<void(bool, QVector<Models::Employer>, QString)> cb) {
    auto* reply = getRequest("/api/employers");
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, {}, err); return; }
        cb(true, parseEmployers(doc.array()), QString());
    });
}

void ApiClient::createJobPosting(const Models::JobPosting& job, IdCallback cb) {
    auto* reply = postJson("/api/jobs", job.toJson());
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, 0, err); return; }
        size_t id = static_cast<size_t>(doc.object()["id"].toInt());
        cb(true, id, QString());
    });
}

void ApiClient::deleteJobPosting(size_t jobId, SuccessCallback cb) {
    auto* reply = deleteRequest("/api/jobs/" + QString::number(jobId));
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, err); return; }
        bool success = doc.object()["success"].toBool();
        cb(success, success ? QString() : "Server returned failure");
    });
}

void ApiClient::searchJobPostings(const Models::JobFilter& filter,
                                   std::function<void(bool, QVector<Models::JobPosting>, QString)> cb) {
    auto* reply = getRequest("/api/jobs/search" + filter.toQueryString());
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, {}, err); return; }
        cb(true, parseJobPostings(doc.array()), QString());
    });
}

void ApiClient::getJobPostingsByEmployer(size_t employerId,
                                          std::function<void(bool, QVector<Models::JobPosting>, QString)> cb) {
    auto* reply = getRequest("/api/jobs/employer/" + QString::number(employerId));
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, {}, err); return; }
        cb(true, parseJobPostings(doc.array()), QString());
    });
}

void ApiClient::createReply(size_t jobPostingId, size_t workerId, IdCallback cb) {
    Models::Reply r;
    r.jobPostingId = jobPostingId;
    r.workerId     = workerId;
    auto* reply = postJson("/api/replies", r.toJson());
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, 0, err); return; }
        size_t id = static_cast<size_t>(doc.object()["id"].toInt());
        cb(true, id, QString());
    });
}

void ApiClient::getRepliesByJob(size_t jobId,
                                 std::function<void(bool, QVector<Models::Reply>, QString)> cb) {
    auto* reply = getRequest("/api/replies/job/" + QString::number(jobId));
    connectReply(reply, [cb](bool ok, const QJsonDocument& doc, const QString& err) {
        if (!ok) { cb(false, {}, err); return; }
        cb(true, parseReplies(doc.array()), QString());
    });
}
