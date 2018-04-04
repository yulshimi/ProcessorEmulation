//Name: Phillip Jo
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
pair<string, string> inspect_string(string input, bool& is_branch);
string generate_code(string input, bool is_seti);
int main (int argc, char* argv[])
{
  if(argc != 3)
  {
    cout << "./assembler input_file output_file" << endl;
  }
  pair<string, string> my_pair;
  string line;
  string sub_line;
  string int_string;
  int integer;
  ofstream outs;
  ifstream ins;
  outs.open(argv[2]);
  ins.open(argv[1]);
  int string_size = 0;
  bool is_branch = false;
  if(ins.is_open())
  {
    while(getline(ins, line))
    {
      my_pair = inspect_string(line, is_branch);
      outs << my_pair.first;
      if(is_branch == true)
      {
        outs << my_pair.second << endl;
        is_branch = false;
      }
      else
      {  
        if(my_pair.second != " ")
        {
          if(my_pair.first.length() == 1)
          {
            int_string = generate_code(my_pair.second, true);
          }
          else
          {
            int_string = generate_code(my_pair.second, false);
          }
          outs << int_string << endl;
        }
      }
    }  
  }
  ins.close();
  outs.close();
  return 0;
}

pair<string, string> inspect_string(string input, bool& is_branch)
{
  pair<string, string> output_pair;
  string instruction = input.substr(0, input.find(' '));
  if(instruction == "seti")
  {
    output_pair.first = "1";
    output_pair.second = input.substr(5, input.find(';'));
  }
  else if(instruction == "bnez" || instruction == "bez")
  {
    is_branch = true;
    if(instruction == "bnez")
    {
      output_pair.first = "00100";
      output_pair.second = input.substr(5, 4);
    }  
    else
    {
      output_pair.first = "00011";
      output_pair.second = input.substr(4, 4); 
    }
  }
  else if(instruction == "halt;")
  {
    output_pair.first = "011111111";
    output_pair.second = " ";
  }
  else
  {
    if(instruction == "set")
    {
      output_pair.first = "00000";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "lw")
    {
      output_pair.first = "00001";
      output_pair.second = input.substr(4, input.find(';'));    
    }
    else if(instruction == "add")
    {
      output_pair.first = "00010";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "slo")
    {
      output_pair.first = "00101";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "slg")
    {
      output_pair.first = "00110";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "addone")
    {
      output_pair.first = "00111";
      output_pair.second = input.substr(8, input.find(';'));
    }
    else if(instruction == "srg")
    {
      output_pair.first = "01000";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "mov")
    {
      output_pair.first = "01001";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "sub")
    {
      output_pair.first = "01010";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "flip")
    {
      output_pair.first = "01011";
      output_pair.second = input.substr(6, input.find(';'));
    }
    else if(instruction == "and")
    {
      output_pair.first = "01100";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "xor")
    {
      output_pair.first = "01101";
      output_pair.second = input.substr(5, input.find(';'));
    }
    else if(instruction == "sw")
    {
      output_pair.first = "01110";
      output_pair.second = input.substr(4, input.find(';'));
    }
  }
  return output_pair;
}

string generate_code(string input, bool is_seti)
{
  int my_int = stoi(input);
  char input_char = (char)my_int;
  unsigned char bit_mask = 128;
  char consequence = 0;
  string output_string = "        ";
  for(int i=0; i < 8; ++i)
  {
    consequence = input_char & bit_mask;
    if(consequence == 0)
    {
      output_string[i] = '0';
    }
    else
    {
      output_string[i] = '1';
    }
    input_char = input_char << 1;
  }
  if(is_seti == false)
  {
    string s_trimmed = output_string.substr(4, 7);
    return s_trimmed;
  }
  return output_string;
}
















