#include "Early.h"

const std::string Passed = "Passed\n";
const std::string NotPassed = "Not Passed\n";

std::string TestContainWord(EarlyAlgorithm &parser, const std::string& word, bool isContains = true) {
    return parser.isContainsWord(word) ^ !isContains ? Passed : NotPassed;
}

void TestCorrectBracket() {
    std::vector<Rule> rules;
    rules.emplace_back(Rule('P', "P(P)"));
    rules.emplace_back(Rule('P', ""));

    EarlyAlgorithm parser(rules, 'P');
    std::cout << TestContainWord(parser, "()())(()", false);
    std::cout << TestContainWord(parser, "(", false);
    std::cout << TestContainWord(parser, "()()()[]", false);
    std::cout << TestContainWord(parser, "((((())()))))", false);

    std::cout << TestContainWord(parser, "(())()()(()(())())");
    std::cout << TestContainWord(parser, "((()))");
    std::cout << TestContainWord(parser, "");

}

void TestMathExpression() {
    std::vector<Rule> rules;
    rules.emplace_back('S', "S+B");
    rules.emplace_back('S', "B");
    rules.emplace_back('B', "A*B");
    rules.emplace_back('B', "A");
    rules.emplace_back('A', "(S)");
    rules.emplace_back('A', "x");
    rules.emplace_back('A', "y");
    rules.emplace_back('A', "z");

    EarlyAlgorithm parser(rules, 'S');

    std::cout << TestContainWord(parser, "x+xxy+(z+z)", false);
    std::cout << TestContainWord(parser, "", false);
    std::cout << TestContainWord(parser, "(x", false);
    std::cout << TestContainWord(parser, "xx", false);
    std::cout << TestContainWord(parser, "ax+bx^2+cx^3", false);

    std::cout << TestContainWord(parser, "(z+y)*(z+x*x)+(y*y+x*z)");
    std::cout << TestContainWord(parser, "(((((z+z+z)))))");
    std::cout << TestContainWord(parser, "x*z*z+y*x+((x+z+z)*y+x*x)");
}

void TestFromMyKR() {
    std::vector<Rule> rules;
    rules.emplace_back(Rule('S', "T"));
    rules.emplace_back(Rule('S', "a"));
    rules.emplace_back(Rule('T', "aUba"));
    rules.emplace_back(Rule('T', "b"));
    rules.emplace_back(Rule('U', "bSa"));
    rules.emplace_back(Rule('U', ""));

    EarlyAlgorithm parser(rules, 'S');

    std::cout << TestContainWord(parser, "aabbaba", false);
    std::cout << TestContainWord(parser, "bbb", false);
    std::cout << TestContainWord(parser, "ababaabc", false);

    std::cout << TestContainWord(parser, "abbaba");
    std::cout << TestContainWord(parser, "ababaabaaba");
    std::cout << TestContainWord(parser, "ababaaba");
}

std::string TestScan1() {
    std::vector<un_set_event> configurations;
    un_set_event tmpSet;
    tmpSet.emplace('T', "(S)", 0, 0);
    tmpSet.emplace('T', "x", 0, 0);
    tmpSet.emplace('T', "y", 0, 0);
    tmpSet.emplace('T', "z", 0, 0);
    configurations.emplace_back(tmpSet);

    scan(configurations, '(', 0);
    if (configurations[1].size() != 1)
        return NotPassed;
    if (configurations[1].find(Event('T', "(S)", 0, 1)) == configurations[1].end())
        return NotPassed;
    return Passed;
}

std::string TestScan2() {
    std::vector<un_set_event> configurations;
    un_set_event tmpSet;
    tmpSet.emplace('S', "P", 1, 1);
    tmpSet.emplace('P', "P*T", 1, 1);
    tmpSet.emplace('T', "(S)", 0, 2);
    tmpSet.emplace('S', "S+P", 1, 1);
    configurations.emplace_back(tmpSet);

    scan(configurations, '+', 0);
    if (configurations[1].size() != 1)
        return NotPassed;
    if (configurations[1].find(Event('S', "S+P", 1, 2)) == configurations[1].end())
        return NotPassed;
    return Passed;
}

std::string TestScan3() {
    std::vector<un_set_event> configurations;
    un_set_event tmpSet;
    tmpSet.emplace('T', "x", 6, 0);
    tmpSet.emplace('T', "y", 6, 0);
    tmpSet.emplace('T', "z", 6, 0);
    configurations.emplace_back(tmpSet);

    scan(configurations, 'z', 0);
    if (configurations[1].size() != 1)
        return NotPassed;
    if (configurations[1].find(Event('T', "z", 6, 1)) == configurations[1].end())
        return NotPassed;
    return Passed;
}

void TestScanFunction() {
    std::cout << "Scan" << std::endl;
    std::cout << TestScan1();
    std::cout << TestScan2();
    std::cout << TestScan3();
}

std::string TestPredict1(std::unordered_map<char, un_set_rule> &ruleSet) {
    std::vector<un_set_event> configurations;
    un_set_event tmpSet;
    tmpSet.emplace('P', "T", 0, 0);
    configurations.emplace_back(tmpSet);

    predict(ruleSet, configurations, 0);
    if (configurations[0].size() != 5)
        return NotPassed;
    if (configurations[0].find(Event('T', "(S)", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "x", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "y", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "z", 0, 0)) == configurations[0].end())
        return NotPassed;
    return Passed;
}

std::string TestPredict2(std::unordered_map<char, un_set_rule> &ruleSet) {
    std::vector<un_set_event> configurations;
    un_set_event tmpSet;
    tmpSet.emplace('S', "S+P", 0, 2);
    configurations.emplace_back(tmpSet);

    predict(ruleSet, configurations, 0);
    if (configurations[0].size() != 3)
        return NotPassed;
    if (configurations[0].find(Event('P', "P*T", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('P', "T", 0, 0)) == configurations[0].end())
        return NotPassed;
    return Passed;
}

std::string TestPredict3(std::unordered_map<char, un_set_rule> &ruleSet) {
    std::vector<un_set_event> configurations;
    un_set_event tmp_set;
    tmp_set.emplace('P', "P*T", 0, 2);
    configurations.emplace_back(tmp_set);

    predict(ruleSet, configurations, 0);
    if (configurations[0].size() != 5)
        return NotPassed;
    if (configurations[0].find(Event('T', "(S)", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "x", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "y", 0, 0)) == configurations[0].end())
        return NotPassed;
    if (configurations[0].find(Event('T', "z", 0, 0)) == configurations[0].end())
        return NotPassed;
    return Passed;
}

void TestPredictFunction() {
    std::vector<Rule> rules;
    rules.emplace_back('S', "S+P");
    rules.emplace_back('S', "P");
    rules.emplace_back('P', "P*T");
    rules.emplace_back('P', "T");
    rules.emplace_back('T', "(S)");
    rules.emplace_back('T', "x");
    rules.emplace_back('T', "y");
    rules.emplace_back('T', "z");

    std::unordered_map<char, un_set_rule> ruleSet;
    getRuleSet(ruleSet, rules);

    std::cout << "Predict\n";
    std::cout << TestPredict1(ruleSet);
    std::cout << TestPredict2(ruleSet);
    std::cout << TestPredict3(ruleSet);
}

std::string TestMath2Complete() {
    un_set_event tmpSet;
    std::vector<un_set_event> configurations(3, tmpSet);
    configurations[1].emplace('S', "S+P", 1, 0);
    configurations[1].emplace('S', "P", 1, 0);
    configurations[1].emplace('T', "(S)", 0, 1);
    configurations[1].emplace('P', "P*T", 1, 0);
    configurations[1].emplace('P', "T", 1, 0);
    configurations[2].emplace('P', "T", 1, 1);

    complete(configurations, 2);
    complete(configurations, 2);

    if (configurations[2].size() != 5)
        return NotPassed;
    if (configurations[2].find(Event('S', "P", 1, 1)) == configurations[2].end())
        return NotPassed;
    if (configurations[2].find(Event('P', "P*T", 1, 1)) == configurations[2].end())
        return NotPassed;
    if (configurations[2].find(Event('T', "(S)", 0, 2)) == configurations[2].end())
        return NotPassed;
    if (configurations[2].find(Event('S', "S+P", 1, 1)) == configurations[2].end())
        return NotPassed;
    return Passed;
}


std::string TestMathExpressionComplete() {
    un_set_event tmpSet;
    std::vector<un_set_event> configurations(5, tmpSet);
    configurations[4].emplace('T', "y", 3, 1);
    configurations[3].emplace('P', "T", 3, 0);
    configurations[3].emplace('S', "S+P", 1, 2);
    configurations[3].emplace('P', "P*T", 3, 0);
    configurations[1].emplace('S', "S+P", 1, 0);
    configurations[1].emplace('T', "(S)", 0, 1);

    complete(configurations, 4);
    complete(configurations, 4);
    complete(configurations, 4);

    if (configurations[4].size() != 6)
        return NotPassed;
    if (configurations[4].find(Event('P', "T", 3, 1)) == configurations[4].end())
        return NotPassed;
    if (configurations[4].find(Event('P', "P*T", 3, 1)) == configurations[4].end())
        return NotPassed;
    if (configurations[4].find(Event('T', "(S)", 0, 2)) == configurations[4].end())
        return NotPassed;
    if (configurations[4].find(Event('S', "S+P", 1, 1)) == configurations[4].end())
        return NotPassed;
    if (configurations[4].find(Event('S', "S+P", 1, 3)) == configurations[4].end())
        return NotPassed;
    return Passed;
}

std::string TestOnlySumComplete() {
    un_set_event tmpSet;
    std::vector<un_set_event> configurations(5, tmpSet);
    configurations[1].emplace('T', "(S)", 0, 1);
    configurations[1].emplace('S', "S+P", 1, 0);
    configurations[2].emplace('S', "P", 1, 1);

    complete(configurations, 2);

    if (configurations[2].size() != 3)
        return NotPassed;
    if (configurations[2].find(Event('T', "(S)", 0, 2)) == configurations[2].end())
        return NotPassed;
    if (configurations[2].find(Event('S', "S+P", 1, 1)) == configurations[2].end())
        return NotPassed;
    return Passed;
}

void TestCompleteFunction() {
    std::cout << "Complete\n";
    std::cout << TestMathExpressionComplete();
    std::cout << TestMath2Complete();
    std::cout << TestOnlySumComplete();
}

int main() {
    TestCorrectBracket();
    TestMathExpression();
    TestFromMyKR();
    TestScanFunction();
    TestPredictFunction();
    TestCompleteFunction();
    return 0;
}