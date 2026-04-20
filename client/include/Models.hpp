#pragma once

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>

namespace Models {

struct Worker {
    size_t id = 0;
    QString name;
    QString email;
    QStringList skills;
    int experience = 0;
    QString city;
    QString resume;

    static Worker fromJson(const QJsonObject& obj) {
        Worker w;
        w.id         = static_cast<size_t>(obj["id"].toInt());
        w.name       = obj["name"].toString();
        w.email      = obj["email"].toString();
        w.experience = obj["experience"].toInt();
        w.city       = obj["city"].toString();
        w.resume     = obj["resume"].toString();
        const QJsonArray arr = obj["skills"].toArray();
        for (const auto& v : arr) {
            w.skills << v.toString();
        }
        return w;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        if (id != 0) obj["id"] = static_cast<int>(id);
        obj["name"]       = name;
        obj["email"]      = email;
        obj["experience"] = experience;
        obj["city"]       = city;
        obj["resume"]     = resume;
        QJsonArray arr;
        for (const auto& s : skills) arr.append(s);
        obj["skills"] = arr;
        return obj;
    }
};

struct Employer {
    size_t id = 0;
    QString companyName;
    QString email;
    QString description;
    QString city;

    static Employer fromJson(const QJsonObject& obj) {
        Employer e;
        e.id          = static_cast<size_t>(obj["id"].toInt());
        e.companyName = obj["companyName"].toString();
        e.email       = obj["email"].toString();
        e.description = obj["description"].toString();
        e.city        = obj["city"].toString();
        return e;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        if (id != 0) obj["id"] = static_cast<int>(id);
        obj["companyName"] = companyName;
        obj["email"]       = email;
        obj["description"] = description;
        obj["city"]        = city;
        return obj;
    }
};

struct JobPosting {
    size_t id = 0;
    size_t employerId = 0;
    QString title;
    QString description;
    int salary = 0;
    QString city;
    bool isRemote = false;
    QString publishDate;
    QString profession;

    static JobPosting fromJson(const QJsonObject& obj) {
        JobPosting j;
        j.id          = static_cast<size_t>(obj["id"].toInt());
        j.employerId  = static_cast<size_t>(obj["employerId"].toInt());
        j.title       = obj["title"].toString();
        j.description = obj["description"].toString();
        j.salary      = obj["salary"].toInt();
        j.city        = obj["city"].toString();
        j.isRemote    = obj["isRemote"].toBool();
        j.publishDate = obj["publishDate"].toString();
        j.profession  = obj["profession"].toString();
        return j;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        if (id != 0)         obj["id"]         = static_cast<int>(id);
        if (employerId != 0) obj["employerId"]  = static_cast<int>(employerId);
        obj["title"]       = title;
        obj["description"] = description;
        obj["salary"]      = salary;
        obj["city"]        = city;
        obj["isRemote"]    = isRemote;
        obj["profession"]  = profession;
        return obj;
    }
};

struct Reply {
    size_t id = 0;
    size_t jobPostingId = 0;
    size_t workerId = 0;
    QString replyDate;
    QString status;

    static Reply fromJson(const QJsonObject& obj) {
        Reply r;
        r.id           = static_cast<size_t>(obj["id"].toInt());
        r.jobPostingId = static_cast<size_t>(obj["jobPostingId"].toInt());
        r.workerId     = static_cast<size_t>(obj["workerId"].toInt());
        r.replyDate    = obj["replyDate"].toString();
        r.status       = obj["status"].toString();
        return r;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["jobPostingId"] = static_cast<int>(jobPostingId);
        obj["workerId"]     = static_cast<int>(workerId);
        return obj;
    }
};

struct JobFilter {
    QString city;
    QString profession;
    int minSalary = 0;
    int maxSalary = 0;
    bool remoteOnly = false;

    QString toQueryString() const {
        QStringList params;
        if (!city.isEmpty())       params << "city=" + city;
        if (!profession.isEmpty()) params << "profession=" + profession;
        if (minSalary > 0)         params << "minSalary=" + QString::number(minSalary);
        if (maxSalary > 0)         params << "maxSalary=" + QString::number(maxSalary);
        if (remoteOnly)            params << "isRemote=true";
        return params.isEmpty() ? QString() : "?" + params.join("&");
    }
};

}
