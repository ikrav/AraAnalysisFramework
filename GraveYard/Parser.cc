#include "Parser.h"

Parser::Parser(){
 
  initialize();
}

Parser::~Parser(){

  for(int i=0; i<_config.size(); i++){
    if(_config[i]) delete _config[i];
    _config[i] = 0;
  }
  _config.clear();
}

void Parser::initialize(){

  _command.clear();
  _config.clear();
  
}

void Parser::printout(){

  for(int i=0; i< _config.size(); i++){
    _config[i]->printout();
  }
}

void Parser::parseTextFile(std::string textFile){

  ifstream configFile;
  configFile.open(textFile.c_str());
  
  bool commandOpen= false;
  std::string line;
  std::string command ="";
  while(!configFile.eof()){
    if(configFile.eof() || !configFile.good()) break;
    
    configFile >> line;
    std::cout << __PRETTY_FUNCTION__ <<" line: "<<  line << std::endl;
    if(line[0] == '%' && commandOpen == false){
      commandOpen = true;
      command = "";
    }
    else if(line[0] == '%' && commandOpen == true){
      std::cout << __PRETTY_FUNCTION__  <<  command << std::endl;
      command[command.size()-1] = ""[0]; // to remove last comma
      addCommand(command);
      commandOpen = false;
      command = "";
    }
    else{
      line += ',';
      command += line;
    }
  }
}

void Parser::addCommand(std::string input){

  parseLine(input);
  validateCommand(input);
  makeConfiguration();
  _command.clear();
}

void Parser::printVector(){

  for(int i=0; i < _command.size(); i++){
    std::cout  << " i = " << i << "\t "<< _command[i] << std::endl;
  }
}

void Parser::parseLine(std::string input){
  
  std::vector<std::string> command;
  std::string singleLetter;
  int openBracket = false;

  for(int i=0; i<input.size(); i++){

    input[i] = toupper(input[i]);
    if(input[i] == '('){
      openBracket = true;

      }
    if(input[i] == ')'){
      openBracket = false;

    }
    if(input[i] != ','){     
      if(!isspace(input[i])) 
	singleLetter += input[i];
    }

    if(input[i] == ',') {
      if( openBracket == false){
	_command.push_back(singleLetter);
	singleLetter ="";
      }// open bracket
      else {
	//remove space
       	if(!isspace(input[i]))
	  singleLetter += input[i];	
      }
    }
    
  }
  _command.push_back(singleLetter);

}

void Parser::validateCommand(std::string input){

  //Checks if missing brackets, missing commas, etc.
  int nOpenParentheses = 0;
  int nCloseParentheses = 0;
  int nOpenBrackets =0;
  int nCloseBrackets =0;

  for(int i=0; i<input.size(); i++){
    
    if(input[i] == '(') nOpenParentheses++;
    else if(input[i] == ')') nCloseParentheses++;
    else if(input[i] == '[') nOpenBrackets++;
    else if(input[i] == ']') nCloseBrackets++;
    else continue;
  }
    
  if(nOpenParentheses != nCloseParentheses) 
    std::cerr <<"Parentheses missing "<< std::endl;
  if(nOpenBrackets != nCloseBrackets)
    std::cerr<<"Bracket missing "<< std::endl;
    
}

string Parser::getPart(std::string line){

  //  return line.substr(line.find_first_of(",")+1).str();
  return "";
}

string Parser::getLabel(std::string line) {
  
  return line.substr(0, line.find_first_of("="));
}

string Parser::getValue(std::string line) {

  return line.substr(line.find_first_of("=")+1).c_str();
}

void Parser::makeConfiguration(){
  
  //every time we call this is to make a new configuration
  _config.push_back(new Configuration());

  for(int i=0; i<_command.size(); i++){
    
    if(getLabel(_command[i]) == "LABEL"){
      _config.back()->setLabel(getValue(_command[i]));
      
    }

    else if(getLabel(_command[i]) == "POL"){

      _config.back()->setPol(getValue(_command[i]));

    }
    else if(getLabel(_command[i]) == "TIME"){

      std::string name = Parser::getFinderName(getValue(_command[i]));
      std::string bracketValue = Parser::parseBracketValue(getValue(_command[i]));
      std::string parenthesesValues = Parser::getParenthesesValue(getValue(_command[i]));
      std::vector<double> vals = Parser::getValueVector(parenthesesValues);
      _config.back()->setTimeFinder(name);
      _config.back()->setTimeFinderParameters(vals[0], vals[1], vals[2]);
      _config.back()->setDelta(bracketValue);
      
    }

    else if(getLabel(_command[i]) == "VERTEX"){

      std::vector<std::string> commands;
      commands = Parser::getFinderVector(getValue(_command[i]));
      
      for(int i=0; i<commands.size(); i++){
	

	std::string recoName = Parser::getFinderName(commands[i]);

	std::string parenthesesValues = Parser::getParenthesesValue(getValue(commands[i]));
	std::vector<double> vals = Parser::getValueVector(parenthesesValues);


	std::string coord = Parser::parseBracketValue(commands[i]);

	_config.back()->addVertex(new ConfigVertex(recoName, vals[0], vals[1], vals[2], coord));

	if(commands[i][0] == '>' && i != 0){
	  //has previous
	  _config.back()->getVertices().back()->setPreviousFinder(_config.back()->getVertices().at(_config.back()->getVertices().size()-2));

	}
	else  {
	  _config.back()->getVertices().back()->setPreviousFinder(0);
	}
      }//for
      
    }//else if

    else if(getLabel(_command[i]) == "SAVE"){

    }
    
    else if(getLabel(_command[i]) == "PRINT"){

    }

    else {
      std::cout <<  getLabel(_command[i]) <<" "<< getValue(_command[i]) << std::endl;
      std::cerr <<"UNKWNON COMMAND :" << _command[i] << " SKIPPED" << std::endl;
      if(_config.back()) delete _config.back();
      _config.pop_back();
      //      exit(1);
    }
  }
}

double Parser::getBracketValue(string line){


}

string Parser::getFinderName(std::string value){

  std::string name;
  for(int i=0; i<value.size(); i++){
    if(value[i] == '>' || value[i] == '|') continue;
    if(value[i] == '(' || value[i] == '[') break;
    name += value[i];
  }
  return name;
}

std::vector<std::string> Parser::getFinderVector(std::string value){

  //returns vector of vertexFinding method            
  std::string command;
  std::vector<std::string> vec_command;
  for(int i=0; i< value.size(); i++){

    if(value[i] == '>'){
      vec_command.push_back(command);
      command = "";
      command += '>';
    }
    else if(value[i] == '|'){
      vec_command.push_back(command);
      command = "";
      command += '|';

    }
    else {

      command += value[i];
    }
  }
  vec_command.push_back(command);
  return vec_command;
}

string Parser::getParenthesesValue(std::string value){

  std::string command;
  bool parentheses= false;
  for(int i=0; i<value.size(); i++){

    if(value[i] == '('){
      parentheses = true;
      continue;
    }
    if(value[i] == ')'){
      parentheses = false;
      break;
    }
    if(parentheses == true){
      command += value[i];
    }
  }
  return command;
}

std::vector<double> Parser::getValueVector(std::string value){

  std::vector<double> vals;
  vals.clear();
  vals.reserve(3);
  double a = std::strtod((value.substr(0, value.find_first_of(','))).c_str(),0);
  std::string bb = value.substr(value.find_first_of(',')+1);
  double b = std::strtod((bb.substr(0,bb.find_first_of(','))).c_str(),0);
  std::string cc = bb.substr(bb.find_first_of(',')+1);
  double c = std::strtod((cc.substr(0, cc.size())).c_str(),0);

  vals.push_back(a);
  vals.push_back(b);
  vals.push_back(c);

  return vals;
}

std::string Parser::parseBracketValue(std::string value){

  std::string command;
  bool bracket = false;
  for(int i=0; i<value.size(); i++){

    if(value[i] == '['){

      bracket = true;
      continue;
    }
    if(value[i] == ']'){
      bracket = false;
      break;
    }
    if(bracket == true){

      command += value[i];
    }
  }
  return command;
}
