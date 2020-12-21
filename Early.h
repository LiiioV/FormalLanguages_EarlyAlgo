#ifndef FORMALLANGUAGEPRACTICE1_EARLY_H
#define FORMALLANGUAGEPRACTICE1_EARLY_H

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Rule {
    char left;
    std::string right;

    Rule(char left, std::string right) : left(left), right(std::move(right)) {}
    bool operator==(const Rule &other) const {
        return left == other.left && right == other.right;
    }
    struct RuleHash {
        size_t operator()(const Rule &rule) const {
            std::size_t h1 = std::hash<char>{}(rule.left);
            std::size_t h2 = std::hash<std::string>{}(rule.right);
            return h1 ^ (h2 << 1);
        }
    };
};

struct Event : Rule {
    int startPos;
    int pointIndex;

    Event(char left, const std::string &right, int start, int index = 0)
        : Rule(left, right), startPos(start), pointIndex(index) {}
    Event(const Rule &rule, int start, int index = 0) : Rule(rule), startPos(start), pointIndex(index) {}

    bool operator==(const Event &other) const {
        return dynamic_cast<const Rule &>(*this) == dynamic_cast<const Rule &>(other)
            && startPos == other.startPos && pointIndex == other.pointIndex;
    }
    struct EventHash {
        size_t operator()(const Event &event) const {
            std::size_t h1 = std::hash<char>{}(event.left);
            std::size_t h2 = std::hash<std::string>{}(event.right);
            std::size_t h3 = std::hash<int>{}(event.startPos);
            std::size_t h4 = std::hash<int>{}(event.pointIndex);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
};

using un_set_rule = std::unordered_set<Rule, Rule::RuleHash>;
using un_set_event = std::unordered_set<Event, Event::EventHash>;

void predict(std::unordered_map<char, un_set_rule> &ruleSet, std::vector<un_set_event> &configurations, int configIndex) {
    un_set_event configSet;
    for (const auto &curEvent : configurations[configIndex]) {
        if (curEvent.pointIndex >= curEvent.right.size())
            continue;
        char pointFrontSymbol = curEvent.right[curEvent.pointIndex];
        if (ruleSet.find(pointFrontSymbol) != ruleSet.end())
            for (const auto &curRule : ruleSet[pointFrontSymbol])
                configSet.emplace(curRule, configIndex, 0);
    }
    configurations[configIndex].merge(configSet);
}

void scan(std::vector<un_set_event> &configurations, char next, int configIndex) {
    un_set_event configSet;
    for (const auto &curEvent : configurations[configIndex])
        if (curEvent.pointIndex < curEvent.right.size() && curEvent.right[curEvent.pointIndex] == next)
            configSet.insert(Event(curEvent.left, curEvent.right, curEvent.startPos, curEvent.pointIndex + 1));
    configurations.emplace_back(configSet);
}

void complete(std::vector<un_set_event> &configurations, int configIndex) {
    un_set_event configSet;
    for (const auto &curEvent : configurations[configIndex]) {
        if (curEvent.pointIndex < curEvent.right.size())
            continue;
        for (const auto &config : configurations[curEvent.startPos])
            if (config.pointIndex < config.right.size() && config.right[config.pointIndex] == curEvent.left)
                configSet.emplace(config.left, config.right, config.startPos, config.pointIndex + 1);
    }
    configurations[configIndex].merge(configSet);
}
void getRuleSet(std::unordered_map<char, un_set_rule> &ruleSet, const std::vector<Rule> &rules) {
    un_set_rule tmpRuleSet;
    for (const auto &curRule : rules) {
        if (ruleSet.find(curRule.left) != ruleSet.end()) {
            ruleSet[curRule.left].insert(curRule);
            continue;
        }
        tmpRuleSet = un_set_rule();
        tmpRuleSet.insert(curRule);
        ruleSet.emplace(curRule.left, tmpRuleSet);
    }
    tmpRuleSet = un_set_rule();
    tmpRuleSet.emplace('\'', std::string(1, 'S'));
    ruleSet.emplace( '\'', tmpRuleSet);
}


class EarlyAlgorithm {
    char startSymbol_;
    std::unordered_map<char, un_set_rule> ruleSet_;
    std::vector<un_set_event> configurations_;
   public:
    EarlyAlgorithm(const std::vector<Rule> &rules, char start) : startSymbol_(start) {
        getRuleSet(ruleSet_, rules);
        un_set_event tmpConfigSet;
        tmpConfigSet.insert(Event('\'', std::string(1, startSymbol_), 0, 0));
        configurations_.emplace_back(tmpConfigSet);
    }
    bool isContainsWord(const std::string &word) {
        configurations_.resize(1);
        return isRunAlgo_(startSymbol_, word);
    }
private:
    bool isRunAlgo_(char start, const std::string &word) {
        predictComplete_(0);
        for (size_t j = 1; j <= word.size(); ++j) {
            scan(configurations_, word[j - 1], j - 1);
            predictComplete_(j);
        }
        Event foundEvent('\'', std::string(1, startSymbol_), 0, 1);
        return configurations_[word.size()].find(foundEvent) != configurations_[word.size()].end();
    }
    void predictComplete_(int configIndex) {
        int oldSize = -1;
        int newSize = configurations_[configIndex].size();
        while (newSize != oldSize) {
            predict(ruleSet_, configurations_, configIndex);
            complete(configurations_, configIndex);
            oldSize = newSize;
            newSize = configurations_[configIndex].size();
        };
    }
};

#endif
