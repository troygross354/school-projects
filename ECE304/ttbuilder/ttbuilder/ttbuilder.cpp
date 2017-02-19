//TTBuilder
#include <iostream>
#include <fstream>
#include <algorithm> //For string transform function
#include <stack>
#include <bitset>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

struct Gate
{
	int number;
	string name = "";
	bool out = false;
	//Inputs with mac fan-in of 8
	int numInputs;
	char inputs[8];
	string operation;
};

//Function to increment bitset sequentially
bitset<26> increment(std::bitset<26> in) {
	//  add 1 to each value, and if it was 1 already, carry the 1 to the next.
	for (size_t i = 0; i < 26; ++i) {
		if (in[i] == 0) {  // There will be no carry
			in[i] = 1;
			break;
		}
		in[i] = 0;  // This entry was 1; set to zero and carry the 1
	}
	return in;
}

// Reads a line from the .ldf and parses all relevant data into a Gate struct
Gate parseLine(string line)
{
	//NUMBER->NAME->OPERATION->INPUTS
	unsigned int i = 0, inputIndex = 0;
	Gate newGate;
	while (i < line.length())
	{
		//Get gate number
		if(line[i] == '\t') i++;
		while (line[i] != '\t')
		{
			newGate.number = (int)(line[i] - '0');
			i++;
		}
		
		//Name
		if(line[i] == '\t') i++;
		while (line[i] != '\t')
		{
			newGate.name.append(1, line[i]);
			i++;
		}
		//Convert name to all uppercase
		transform(newGate.name.begin(), newGate.name.end(), newGate.name.begin(), ::toupper);

		if (line[i] == '\t') i++;
		//Operation
		while (line[i] != '\t')
		{
			newGate.operation.append(1, line[i]);
			i++;
		}
		//Convert operation to all uppercase
		transform(newGate.operation.begin(), newGate.operation.end(), newGate.operation.begin(), ::toupper);

		if (line[i] == '\t') i++;
		//Inputs
		while (i < line.length())
		{
			newGate.inputs[inputIndex++] = toupper(line[i]);
			if (line[i] >= '0' && line[i] <= '9') newGate.out = true;
			i += 2; //Skip the tab in between inputs
		}
		newGate.numInputs = inputIndex;
	}

	return newGate;
}

//Finds the post fix representation of a particular output
string getPostFix(Gate currentGate, vector<Gate> circuit)
{
	string post_fix = "";

	for (int i = 0; i < currentGate.numInputs; i++)
	{
		if (i != 0 && i % 2 == 0)//Append an operator for every two operands
		{
			if (!currentGate.operation.compare("NOT"))
				post_fix.append("!");
			else if (!currentGate.operation.compare("AND"))
				post_fix.append("&");
			else if (!currentGate.operation.compare("OR"))
				post_fix.append("|");
			else if (!currentGate.operation.compare("XOR"))
				post_fix.append("^");
			else if (!currentGate.operation.compare("NAND"))
				post_fix.append("&");
			else if (!currentGate.operation.compare("NOR"))
				post_fix.append("|");
			else if (!currentGate.operation.compare("XNOR"))
				post_fix.append("^");
			else
				cout << "Operation " << currentGate.operation << " not supported." << endl;
		}

		//Append operands
		if (currentGate.inputs[i] >= '0' && currentGate.inputs[i] <= '9')
		{
			post_fix.append(getPostFix(circuit[currentGate.inputs[i] - '0' - 1], circuit));
		}
		else
		{
			post_fix += currentGate.inputs[i];
		}
		//Check if two operands have been inserted(every two inputs)
		
	}

	//Append operator second
	if (!currentGate.operation.compare("NOT"))
		post_fix.append("!");
	else if (!currentGate.operation.compare("AND"))
		post_fix.append("&");
	else if (!currentGate.operation.compare("OR"))
		post_fix.append("|");
	else if (!currentGate.operation.compare("XOR"))
		post_fix.append("^");
	else if (!currentGate.operation.compare("NAND"))
		post_fix.append("&!");
	else if (!currentGate.operation.compare("NOR"))
		post_fix.append("|!");
	else if (!currentGate.operation.compare("XNOR"))
		post_fix.append("^!");
	else
		cout << "Operation " << currentGate.operation << " not supported." << endl;

	return post_fix;
}

void sortGates(vector<Gate>& circuit)
{
	//Bubble sort by gate number provided in the .ldf file
	Gate tempGate;
	bool swapped = true;
	int j = 0;
	while (swapped) {
		swapped = false;
		j++;
		for (unsigned int i = 0; i < circuit.size() - j; i++) {
			if (circuit[i].number > circuit[i + 1].number) {
				tempGate = circuit[i];
				circuit[i] = circuit[i + 1];
				circuit[i + 1] = tempGate;
				swapped = true;
			}
		}
	}
}

//Evaluate an output given post fix representation
bool evalOutput(bitset<26> input, string post_fix, int inputNum)
{
	stack<char> s;
	unsigned int i = 0, operand1, operand2;

	//Push first operand on to stack
	s.push(input.test(inputNum - 1 - (post_fix.at(i++) - 'A')) & 0x00000001);
	while (i < post_fix.length())
	{
		if (!(post_fix.at(i) >= 'A' && post_fix.at(i) <= 'Z'))
		{
			// The next char in the post_fix string is an operator
			if (post_fix.at(i) == '!')
			{
				// Negate (only takes one operand)
				operand1 = s.top() ^ 0x00000001;
				s.pop();
				s.push(operand1);
			}
			else
			{
				//The other three operators (&, |, ^) take two operands
				operand1 = s.top() & 0x00000001;
				s.pop();
				operand2 = s.top() & 0x00000001;
				s.pop();

				if (post_fix.at(i) == '&')
					s.push(operand1 & operand2);
				else if (post_fix.at(i) == '|')
					s.push(operand1 | operand2);
				else if (post_fix.at(i) == '^')
					s.push(operand1 ^ operand2);
				else
					cout << "Operator not found" << endl;

			}
			i++;
		}
		else
		{
			// New values to be calculated from the input
			s.push(input.test(inputNum - 1 - (post_fix.at(i++) - 'A')) & 0x00000001);
		}
	}
	return s.top();
}

void printTable(int numInputs, vector<string> post_fix, vector<string> gateNames)
{
	bitset<26> input_bits;
	int numLines = pow(2, numInputs);
	int outputNum = post_fix.size();
	int fieldWidth = 7;

	// Print table header
	for (int i = 0; i < numInputs; i++)
	{
		cout << setw(fieldWidth) << left << (char)('A' + i);
	}
	cout << '|';
	for (int i = 0; i < outputNum; i++)
	{
		cout << setw(fieldWidth) << left << gateNames.at(i);
	}
	cout << endl;
	cout << string(fieldWidth * (numInputs + outputNum), '-') << endl;
	
	//Print table line
	for (int i = 0; i < numLines; i++)
	{
		for (int j = numInputs - 1; j >= 0; j--)
		{
			cout << setw(fieldWidth) << left << input_bits.test(j);
		}
		cout << '|';
		for (int j = 0; j < outputNum; j++)
		{
			cout << setw(fieldWidth) << left << evalOutput(input_bits, post_fix.at(j), numInputs);
		}
		cout << endl;
		// Increment bitset
		input_bits = increment(input_bits);
	}
	cout << endl;
}

void printGate(Gate gateIn)
{
	//Prints all of the data members of the logic gate

	cout << "Name: " << gateIn.name << endl;
	cout << "Number: " << gateIn.number << endl;
	cout << "Number of inputs: " << gateIn.numInputs << endl;
	cout << "Operation: " << gateIn.operation << endl;
	cout << "Output " << gateIn.out << endl;
	cout << "Inputs: ";
	for (int i = 0; i < gateIn.numInputs; i++)
		cout << gateIn.inputs[i] << " ";
	cout << endl << endl;
}

//Finds the amount of unique inputsfor a particular circuit
int getInputNum(vector<Gate> circuit)
{
	bitset<26> ins;
	int count = 0;

	//Find all of the unique inputs
	for (unsigned int i = 0; i < circuit.size(); i++)
	{
		for (int j = 0; j < circuit[i].numInputs; j++)
		{
			if (circuit[i].inputs[j] >= 'A' && circuit[i].inputs[j] <= 'Z')
			{
				ins.set(circuit[i].inputs[j] - 'A');
			}
		}
	}

	//Count how many are checked
	for(int i = 0; i < 26; i++)
		if (ins.test(i) == 1) count++;

	return count;
}

/*Checks to see if the name of the gate contains
  a common logic gate name, if so, return false because
  it is assumed to be only an intermediate circuit gate
*/
bool validOutput(string name)
{
	if (name.find("NOT") != -1 ||
		name.find("AND") != -1 || 
		name.find("OR") != -1)
	{
		return false;
	}
	else
		return true;
}

int main()
{
	string line;
	Gate tempGate;
	vector<Gate> gateList;
	vector<char> fileLine;
	vector<string> outputPF;
	vector<string> outputNames;

	//Input description from file
	ifstream ifile("test4.ldf");

	cout << "The Logic Description File Input: " << endl << endl;
	//Separate the file entries by lines
	while (getline(ifile, line))
	{
		cout << line << endl;
		gateList.push_back(parseLine(line));
	}
	ifile.close();
	cout << endl;

	//Sort the circuit by ascending gate number
	sortGates(gateList);

	int inputNum = getInputNum(gateList);

	//Get post-fix for all gates
	bool* output = new bool[gateList.size()];
	for (unsigned int i = 0; i < gateList.size(); i++)
	{
		if (validOutput(gateList[i].name))
		{
			outputPF.push_back(getPostFix(gateList[i], gateList));
			outputNames.push_back(gateList[i].name);

			cout << outputPF.back() << endl;
		}
	}

	//Print table
	cout << "Truth Table for assumed outputs: " << endl;
	printTable(inputNum, outputPF, outputNames);
	return 0;
}