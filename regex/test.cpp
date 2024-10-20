#include <iostream>
#include <regex/reTree.hpp>
#include <regex/state_machine.hpp>
#include <regex/state_machine_export.hpp>

int main(int argc, const char *argv[]) {

  std::string input = "3";

  std::string re = "[0-9[aaa]][a-c]*b";
  // re = "^[aaa[33]]*[0-9]";
  // re = "^((a)(-^-)[-0-]-[[][]])$";
  // re = " .....$";
  re = "abc[1-5rty]b";
  // unsigned count = 0;
  // auto bracket = regex::evaluateBracket(count, re);
  // std::cout << " Res : " << bracket.eval(input[0]) << '\n';
  // auto result = regex(input, re);
  std::cout << re << '\n';
  regex::Tree tree = regex::parseToTree(re);

  regex::printReTree(tree);

  // auto new_tree = regex::alignTree(tree);

  auto engine = regex::buildStateMachineFromTree(tree);

  std::cout << " Save : "
            << regex::writeMachineDescriptionToFile("engine.txt", engine)
            << '\n';

  std::cout << " Run : " << regex::runStateMachine(engine, "qwertabc12345b")
            << '\n';

  return 0;
}