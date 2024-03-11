#include "avl.hpp"
#include <vector>
#include <chrono>
#include <set>

namespace main_details {

bool is_int_num(std::string str) {
    for (auto it = str.begin(), ite = str.end(); it != ite; ++it) {
        if ((*it != '-') && (!std::isdigit(*it))) return false;
    }
    return true;
}


enum class COMANDS {
    KCMD = 1,
    QCMD = 0
};

} // namespace main_details


int main() {
    using namespace main_details;

    std::vector<int> commands;
    std::vector<int> numbers;

    std::string cmd;

    while ((std::cin >> cmd)) {
        if      (!cmd.compare("k")) { commands.push_back(static_cast<int>(COMANDS::KCMD)); }
        else if (!cmd.compare("q")) { commands.push_back(static_cast<int>(COMANDS::QCMD)); }
        else if (is_int_num(cmd))   { numbers. push_back(std::stoi(cmd)); }
        else {
            std::cerr << "ERROR: bad comand or number\n";
            std::cout << cmd << std::endl;
        }
    }

#ifdef COMPARE
    std::fstream out;
    out.open("test/tmp.txt", std::ios::app);
    if (!out.is_open()) {
        std::cerr << "Cannot open tmp file. Programm shutdown\n";
        return 1;
    }

    std::set<int> test_set;
    std::vector<int> ans_set;
    auto jt = numbers.begin();

    auto start_set = std::chrono::high_resolution_clock::now();

    for (auto it = commands.begin(), ite = commands.end(); it != ite; ++it) {
        if (*it) {
            test_set.insert(*jt);
            ++jt;
        }
        else {
            int lwbnd = *jt;
            auto fst = test_set.lower_bound(lwbnd); ++jt;
            int upbnd = *jt;
            auto snd = test_set.upper_bound(upbnd); ++jt;

            if (upbnd <= lwbnd) ans_set.push_back(0);
            else ans_set.push_back(std::distance(fst, snd));
        }
    }

    auto end_set = std::chrono::high_resolution_clock::now();
    std::cout << "std::set time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_set - start_set).count() << std::endl;
    out << std::chrono::duration_cast<std::chrono::nanoseconds>(end_set - start_set).count() << std::endl;
#endif

    AVL::avl_tree_t tree;
    std::vector<int> ans_tree;

    auto jtt = numbers.begin();

#ifdef COMPARE
    auto start_tree = std::chrono::high_resolution_clock::now();
#endif

    for (auto it = commands.begin(), ite = commands.end(); it != ite; ++it) {
        if (*it) {
            tree.insert(*jtt);
            ++jtt;
        }
        else {
            auto fst = *jtt; ++jtt;
            auto snd = *jtt; ++jtt;
            ans_tree.push_back(AVL::get_tree_distance(tree, fst, snd));
        }
    }

#ifdef COMPARE
    auto end_tree = std::chrono::high_resolution_clock::now();
    std::cout << "avl_tree time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_tree - start_tree).count() << std::endl;
    out << std::chrono::duration_cast<std::chrono::nanoseconds>(end_tree - start_tree).count() << std::endl;
#endif

#ifndef COMPARE
    std::copy(ans_tree.begin(), ans_tree.end(), std::ostream_iterator<int>(std::cout, " "));
#endif

#ifdef COMPARE
    if (ans_set.size() == ans_tree.size()) {
        bool flag = true;
        for (auto it = ans_set.begin(), jt = ans_tree.begin(), ite = ans_set.end(); it != ite; ++it, ++jt) {
            if ((*it) != (*jt)) {
                flag = false;
            }
            //std::cout << (*it) << "-" << (*jt) << std::endl;
        }

        if (flag) std::cout << "worked equally\n";
        else std::cerr << "answer error\n";
        return 0;
    }
    std::cerr << "answer error\n";
#endif
}
