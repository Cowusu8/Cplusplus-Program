#include <Python.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

/*
Description:
	Prints out menu options
Example:
	menu_options()
Output:
	Will print a list of options with newlines
Return:
	None
*/
void menu_options()
{
	cout << "Please select an option:" << endl;
	cout << "1. List all items purchased Today" << endl;
	cout << "2. Find out how many times an item was purchased Today" << endl;
	cout << "3. Show me the purchases as a histogram" << endl;
	cout << "4. Exit program" << endl;
}

/*
Description:
	Allow users to pick from a known list of menu options
Example:
	menu_selection()
Output:
	Returns int value of the selected menu option 
Return:
	menu option as integer
*/
int menu_selection()
{
	string option;
	string validOptions[4] = { "1", "2", "3", "4" };
	menu_options();
	cin >> option;

	bool exists = std::find(std::begin(validOptions), std::end(validOptions), option) != std::end(validOptions);

	if (exists)
	{
		cout << "You have chosen option: " << option << "\n" << endl;
		return stoi(option);
	}
	else
	{
		cout << "Unrecognised option!\n\n";
		menu_selection();
	}
}

/*
Description:
	Provides sub-menu for additional options
Example:
	displayPurchaseCount()
Output:
	Outputs sub-menu options 
Return:
	none
*/
void displayPurchaseCount()
{
	string word;
	cout << "Please enter item name:" << endl;
	cin >> word;
	cout << callIntFunc("purchaseCount", word);
}

/*
Description:
	Data pair to handle frequency.dat file lines
Example:
	DataPair line = { "Yams", 5 }
Output:
	none
Return:
	DataPair
*/
struct DataPair
{
	string key;
	string value;
};

/*
Description:
	Custom splitter function
Example:
	splitPair("Yams 5", " ")
Output:
	Outputs key and value of pair
Return:
	{ "Yams", 5 }
*/
DataPair splitPair(string line, string delimiter)
{
	string key = line.substr(0, line.find(delimiter));
	size_t pos = line.find(delimiter);
	string value = line.substr(pos + 1);

	DataPair pair = { key, value };
	return pair;
};

/*
Description:
	Forms the graphical representation of the filler elements
Example:
	grapher(5) => "*****"
	grapher(3) => "***"
Output:
	Outputs graphical representation
Return:
	complete build-up with rep. elements 
*/
string grapher(int frequency)
{
	string str = "";

	for (int i = 0; i < frequency; i++) {
		str = str + "*";
	}
	return str;
}

/*
Description:
	Reads data file and prints histogram representation
Example:
    ```
	file: frequency.dat
	Yams  5
	Limes 3
	```

	fileOperation()
	-----
	Yams "*****"
	Limes "***"
Output:
	Outputs graphical representation for the frequency.dat file
Return:
	complete build-up with rep. elements
*/
void fileOperation()
{
	string line;
	ifstream frequencyData ("frequency.dat");
	if (frequencyData.is_open())
	{
		while (getline(frequencyData, line))
		{
			string itemName = splitPair(line, " ").key;
			string frequency = splitPair(line, " ").value;

			cout << itemName << " " << grapher(stoi(frequency)) << "\n";
		}
		frequencyData.close();
	}

	else cout << "Unable to open file";
}

/*
Description:
	handlesmenu selections and calls appropriate action
Example:
	handleSelection(3)
Output:
	switch call out based on selected option 
Return:
	none
*/
void handleSelection(int option)
{
	switch (option)
	{
		case 1:
			CallProcedure("printGroceryListCount");
			cout << "\n";
			break;
		case 2:
			displayPurchaseCount();
			cout << "\n";
			break;
		case 3:
			CallProcedure("createDataFile");
			cout << "\n";
			fileOperation();
			break;
	}
}

int main()
{
	system("Color E4");
	int option = menu_selection();

	while (option != 4)
	{
		handleSelection(option);
		option = menu_selection();
	}

	cout << "Exiting program!";
	return 0;
}