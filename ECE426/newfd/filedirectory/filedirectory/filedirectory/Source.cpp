#include <iostream>
#include "FileDirectory.h"
using namespace std;

int main()
{
	FileDirectory f1 = FileDirectory();
	//1.	create and write a file; file1;  of 40 bytes;
	f1.create("file1000", 40);
	f1.write("file1000", 40, "data1", 2017, 1, 17, 13, 0, 0);
	cout << endl;

	//2.	create and write a file; file2;  of 200 bytes;
	f1.create("file2000", 200);
	f1.write("file2000", 200, "data2", 2017, 1, 17, 13, 0, 0);
	cout << endl;

	//3.	create and write a file; file3;  of 300 bytes;
	f1.create("file3000", 300);

	f1.write("file3000", 300, "data3", 2017, 1, 17, 13, 0, 0);
	cout << endl;
	
	//4.	create and write a file; file4;  of 500 bytes.
	if(f1.create("file4000", 500))
		f1.write("file4000", 500, "data4", 2017, 1, 17, 13, 0, 0);
	cout << endl;

	//Try to write a file the same name
	f1.create("file1000", 100);
	cout << endl;
	
	//5.	delete file2.
	f1.dlete("file2000");
	cout << endl;

	//Print the directory
	cout << "Directory:" << endl;
	f1.printDirectory();
	//Print file ata and clusters
	cout << "file1000 data:" << endl;
	f1.printData("file1000");
	cout << "file1000 clusters: " << endl;
	f1.printClusters("file1000");
	cout << endl;

	cout << "file2000 data:" << endl;
	f1.printData("file2000");
	cout << "file2000 clusters: " << endl;
	f1.printClusters("file2000");
	cout << endl;

	cout << "file3000 data:" << endl;
	f1.printData("file3000");
	cout << "file3000 clusters: " << endl;
	f1.printClusters("file3000");
	cout << endl;

	cout << "file4000 data:" << endl;
	f1.printData("file4000");
	cout << "file4000 clusters: " << endl;
	f1.printClusters("file4000");
	cout << endl;
	
	return 0;
}