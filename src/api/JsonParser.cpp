#include "api/JsonParser.hpp"
#include <sstream>
#include <algorithm>

std::string JsonParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string JsonParser::unescapeJson(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            switch (str[i + 1]) {
                case '"': result += '"'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                case '/': result += '/'; ++i; break;
                case 'b': result += '\b'; ++i; break;
                case 'f': result += '\f'; ++i; break;
                case 'n': result += '\n'; ++i; break;
                case 'r': result += '\r'; ++i; break;
                case 't': result += '\t'; ++i; break;
                default: result += str[i];
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

std::map<std::string, std::string> JsonParser::parseObject(const std::string& json) {
    std::map<std::string, std::string> result;
    std::string trimmed = trim(json);
    
    if (trimmed.empty() || trimmed.front() != '{' || trimmed.back() != '}') {
        return result;
    }
    
    trimmed = trimmed.substr(1, trimmed.size() - 2);
    
    size_t pos = 0;
    while (pos < trimmed.size()) {
        size_t keyStart = trimmed.find('"', pos);
        if (keyStart == std::string::npos) break;
        
        size_t keyEnd = trimmed.find('"', keyStart + 1);
        if (keyEnd == std::string::npos) break;
        
        std::string key = trimmed.substr(keyStart + 1, keyEnd - keyStart - 1);
        
        size_t colonPos = trimmed.find(':', keyEnd);
        if (colonPos == std::string::npos) break;
        
        pos = colonPos + 1;
        while (pos < trimmed.size() && std::isspace(trimmed[pos])) ++pos;
        
        std::string value;
        if (trimmed[pos] == '"') {
            size_t valueStart = pos;
            size_t valueEnd = trimmed.find('"', valueStart + 1);
            while (valueEnd != std::string::npos && trimmed[valueEnd - 1] == '\\') {
                valueEnd = trimmed.find('"', valueEnd + 1);
            }
            if (valueEnd == std::string::npos) break;
            value = trimmed.substr(valueStart, valueEnd - valueStart + 1);
            pos = valueEnd + 1;
        } else if (trimmed[pos] == '[') {
            int depth = 1;
            size_t valueStart = pos;
            ++pos;
            while (pos < trimmed.size() && depth > 0) {
                if (trimmed[pos] == '[') ++depth;
                else if (trimmed[pos] == ']') --depth;
                ++pos;
            }
            value = trimmed.substr(valueStart, pos - valueStart);
        } else if (trimmed[pos] == '{') {
            int depth = 1;
            size_t valueStart = pos;
            ++pos;
            while (pos < trimmed.size() && depth > 0) {
                if (trimmed[pos] == '{') ++depth;
                else if (trimmed[pos] == '}') --depth;
                ++pos;
            }
            value = trimmed.substr(valueStart, pos - valueStart);
        } else {
            size_t valueEnd = trimmed.find(',', pos);
            if (valueEnd == std::string::npos) {
                value = trimmed.substr(pos);
                pos = trimmed.size();
            } else {
                value = trimmed.substr(pos, valueEnd - pos);
                pos = valueEnd;
            }
            value = trim(value);
        }
        
        result[key] = value;
        
        size_t commaPos = trimmed.find(',', pos);
        if (commaPos == std::string::npos) break;
        pos = commaPos + 1;
    }
    
    return result;
}

std::vector<std::string> JsonParser::parseArray(const std::string& json) {
    std::vector<std::string> result;
    std::string trimmed = trim(json);
    
    if (trimmed.empty() || trimmed.front() != '[' || trimmed.back() != ']') {
        return result;
    }
    
    trimmed = trimmed.substr(1, trimmed.size() - 2);
    
    size_t pos = 0;
    while (pos < trimmed.size()) {
        while (pos < trimmed.size() && std::isspace(trimmed[pos])) ++pos;
        if (pos >= trimmed.size()) break;
        
        std::string element;
        if (trimmed[pos] == '"') {
            size_t start = pos + 1;
            size_t end = trimmed.find('"', start);
            while (end != std::string::npos && trimmed[end - 1] == '\\') {
                end = trimmed.find('"', end + 1);
            }
            if (end == std::string::npos) break;
            element = unescapeJson(trimmed.substr(start, end - start));
            pos = end + 1;
        } else {
            size_t end = trimmed.find(',', pos);
            if (end == std::string::npos) {
                element = trim(trimmed.substr(pos));
                pos = trimmed.size();
            } else {
                element = trim(trimmed.substr(pos, end - pos));
                pos = end;
            }
        }
        
        result.push_back(element);
        
        size_t commaPos = trimmed.find(',', pos);
        if (commaPos == std::string::npos) break;
        pos = commaPos + 1;
    }
    
    return result;
}

std::optional<std::string> JsonParser::getString(const std::map<std::string, std::string>& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    
    std::string value = trim(it->second);
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        return unescapeJson(value.substr(1, value.size() - 2));
    }
    return std::nullopt;
}

std::optional<int> JsonParser::getInt(const std::map<std::string, std::string>& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    
    try {
        return std::stoi(trim(it->second));
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<size_t> JsonParser::getSizeT(const std::map<std::string, std::string>& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    
    try {
        return std::stoull(trim(it->second));
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool> JsonParser::getBool(const std::map<std::string, std::string>& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    
    std::string value = trim(it->second);
    if (value == "true") return true;
    if (value == "false") return false;
    return std::nullopt;
}

std::vector<std::string> JsonParser::getStringArray(const std::map<std::string, std::string>& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end()) return {};
    
    return parseArray(it->second);
}
