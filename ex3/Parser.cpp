//
// Created by ori294 on 12/11/19.
//

#include "Parser.h"

/**
 * get the next command and returns a pair containing the command and the list of args for it.
 * @return pair with command and list of args (as strings).
 */
std::pair<Command*,std::list<std::string>> Parser::getNextCommand() {
  if (!isEnded()) { //check that there're more commands in the queue
    Command* tempCommand;
    std::list<std::string> tempList;
    auto mapIterator = commandMap.find(*listIterator); //find the right command
    if (mapIterator != commandMap.end()) {
      cout << "found " << *listIterator << endl;
      tempCommand = mapIterator->second; //save the command
        listIterator++;
        cout << "number of arguments: " << tempCommand->get_num_of_arg() << " for " << mapIterator->first  << endl;
      for (int i = 0; i < tempCommand->get_num_of_arg(); i++) { //get the arguments for the command
        tempList.emplace_back(*listIterator);
        cout << "argumnet " << i+1 << " " << *listIterator << endl;
        listIterator++;
      }
      std::pair<Command*,std::list<std::string>> tempPair(tempCommand,tempList);
      return tempPair;

    } else if (*listIterator == "while" || *listIterator == "if") {
      std::string loopType = *listIterator;
      cout << "Found while of if case: " << endl;
      listIterator++;
      while (*listIterator != "{") {
        tempList.emplace_back(*listIterator);
        listIterator++;
      }
      tempList.emplace_back(*listIterator);
      listIterator++;
      int numberOfLeftBrackets = 1;
      int numberOfRightBrackets = 0;
      while (numberOfLeftBrackets > numberOfRightBrackets) {
        cout << "emplacing " << *listIterator << endl;

        if (*listIterator == "}") {
          numberOfRightBrackets++;
          cout << "found right bracket" << endl;
          tempList.emplace_back(*listIterator);
        } else if (*listIterator == "{"){
          numberOfLeftBrackets++;
          cout << "found left bracket" << endl;
          tempList.emplace_back(*listIterator);
        } else {
          tempList.emplace_back(*listIterator);
        }
        listIterator++;
      }

      cout << "debug while and if commands:" << endl;
      auto iterate = tempList.begin();
      while (iterate != tempList.end()) {
        cout << *iterate << ";";
        iterate++;
      }
      cout << endl;

      if (loopType == "while") {
        LoopCommand* loop_command = new LoopCommand(&tempList);
        conditionalCommands.emplace_back(loop_command);
        return std::pair<Command*,std::list<std::string>>(loop_command, tempList);
      } else {
        IfCommand* if_command = new IfCommand(&tempList);
        conditionalCommands.emplace_back(if_command);
        return std::pair<Command*,std::list<std::string>>(if_command, tempList);
      }
    }
    cout << "null command: " << *listIterator << endl;
    NullCommand* null_command;
    listIterator++;
    return std::pair<Command*,std::list<std::string>>(null_command, std::list<std::string>());
  }

}

/**
 * check if there're more commands to perform
 * @return
 */
bool Parser::isEnded() {
  if (listIterator == commandList.end()) {
    return true;
  } else {
    return false;
  }
}
/**
 * Parser constructor
 * @param strList
 */
Parser::Parser(std::list<std::string> strList) {
  commandList = strList;
  listIterator = commandList.begin();

  //Add open server command to the map
  OpenServerCommand *ops = new OpenServerCommand();
  commandMap.insert({"openDataServer", ops});

  //Add open client command to the map
  connectControlClient* cnc = new connectControlClient();
  commandMap.insert({"connectControlClient", cnc});

  //Add assign var command to the map
  VarAssignCommand* vac = new VarAssignCommand();
  commandMap.insert({"=", vac});

  //Add define var command to the map
  DefineVarCommand* dvc = new DefineVarCommand();
  commandMap.insert({"simvar", dvc});

  //Add local var command to the map
  DefineLocalVarCommand* dlvc = new DefineLocalVarCommand();
  commandMap.insert({"var", dlvc});

  //Add print command to the map
  PrintCommand *print = new PrintCommand();
  commandMap.insert({"Print", print});

  //Add sleep command to the map
  SleepCommand *sleep = new SleepCommand();
  commandMap.insert({"Sleep", sleep});
}
