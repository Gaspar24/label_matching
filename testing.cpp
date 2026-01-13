#include <string>
#include <vector>
#include <iostream>

struct Company {
    std::string description;
    std::vector<std::string> business_tags;
    std::string sector;
    std::string category;
    std::string niche;
};

enum class Field {
    DESCRIPTION,
    BUSINESS_TAGS,
    SECTOR,
    CATEGORY,
    NICHE
};

struct Rule {
    Field field;
    std::vector<std::string> keywords;
};

struct LabelDefinition {
    std::string label_name;
    std::vector<Rule> positive_rules;
    std::vector<Rule> negative_rules; // disqualifiers
};


bool containsAny(const std::string& text, const std::vector<std::string>& keywords) {
    for (const auto& k : keywords) {
        if (text.find(k) != std::string::npos)
            return true;
    }
    return false;
}

bool ruleMatches(const Company& c, const Rule& rule) {
    switch (rule.field) {
        case Field::DESCRIPTION:
            return containsAny(c.description, rule.keywords);
        case Field::CATEGORY:
            return containsAny(c.category, rule.keywords);
        case Field::SECTOR:
            return containsAny(c.sector, rule.keywords);
        case Field::NICHE:
            return containsAny(c.niche, rule.keywords);
        case Field::BUSINESS_TAGS:
            for (const auto& tag : c.business_tags) {
                if (containsAny(tag, rule.keywords))
                    return true;
            }
            return false;
    }
    return false;
}


std::vector<std::string> classifyCompany(
    const Company& company,
    const std::vector<LabelDefinition>& labels
) {
    std::vector<std::string> matched;

    for (const auto& label : labels) {
        bool positive_match = false;

        // At least one positive rule must match
        for (const auto& rule : label.positive_rules) {
            if (ruleMatches(company, rule)) {
                positive_match = true;
                break;
            }
        }

        if (!positive_match)
            continue;

        // No negative rule must match
        bool disqualified = false;
        for (const auto& rule : label.negative_rules) {
            if (ruleMatches(company, rule)) {
                disqualified = true;
                break;
            }
        }

        if (!disqualified)
            matched.push_back(label.label_name);
    }

    return matched;
}