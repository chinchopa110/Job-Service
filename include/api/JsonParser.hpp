#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

class JsonParser {
public:
    static std::map<std::string, std::string> parseObject(const std::string& json);
    static std::vector<std::string> parseArray(const std::string& json);
    
    static std::optional<std::string> getString(const std::map<std::string, std::string>& obj, const std::string& key);
    static std::optional<int> getInt(const std::map<std::string, std::string>& obj, const std::string& key);
    static std::optional<size_t> getSizeT(const std::map<std::string, std::string>& obj, const std::string& key);
    static std::optional<bool> getBool(const std::map<std::string, std::string>& obj, const std::string& key);
    static std::vector<std::string> getStringArray(const std::map<std::string, std::string>& obj, const std::string& key);
    
private:
    static std::string trim(const std::string& str);
    static std::string unescapeJson(const std::string& str);
};
