#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Configuration.h"

class Parser {

public:   
  Parser();
  virtual ~Parser();
  
  void initialize();
  void printout();
  
  void addCommand(std::string input);

  void parseLine(std::string line);
  void parse();
  
  void printVector();

  void validateCommand(std::string input);

  string getPart(std::string line);
  string getLabel(std::string line);

  string getValue(std::string line);

  void makeConfiguration();
  vector<double> getParamValues(std::string value);
  
  double getBracketValue(std::string value);
  void parseTextFile(std::string textFile);

  std::vector<std::string> _command;

  std::vector<Configuration*> _config;
  
  static std::string getFinderName(std::string value);
  static std::vector<std::string> getFinderVector(std::string value);
  static std::string getParenthesesValue(std::string value);
  static std::vector<double> getValueVector(std::string value);
  static std::string parseBracketValue(std::string value);
 
};

#endif
  
  
  


