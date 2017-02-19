#include "FileDirectory.h"
#include <iostream>
using namespace std;
FileDirectory::FileDirectory() //initialize all entries in the fileDirectory and FAT16 to be 0, i.e.safe values.
{
	for (int i = 0; i < 256; i++) FAT16[i] = 0;
	FAT16[255] = 0xFFFF;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			fileDirectory[i][j] = '0';
}
bool FileDirectory::create(char filename[], unsigned short int numberBytes)
{
	unsigned short int i, j, sCLIndex = 0;
	unsigned short int numClusters = numberBytes / 4;//4 bytes per cluster
	unsigned short int storeClusterLocations[256];
	bool emptyFound = false;
	//(1)	to check if there is an unused entry in the File Directory;  (i.e.the first character of the file name in the File Directory is zero).Return false if not true.

	//Check if filename already exists in the directory
	for (i = 0; i < 4; i++)
	{
		//Check if current filename is empty
		if (fileDirectory[i][0] == '0')
		{
			//Found empty location
			break;
		}
		for (j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//Filename already exists
					cout << "Can't create " << filename << ", filename already exists." << endl;
					return false;
				}
			}
			else
			{
				//Filename doesn't match, check next file
				break;
			}
		}
	}

	//Find unused clusters
	cout << "Searching for open clusters for " << filename << ", number needed: " << dec << numClusters << endl;
	for (j = 0; (j < 256) && (sCLIndex < numClusters); j++)
	{
		if (FAT16[j] == 0x0000)
		{
			//Store next available FAT address
			storeClusterLocations[sCLIndex++] = j;
		}
		if(j == 255) {
			//(2)	to check if there are enough unused clusters to store the file with the numberBytes.Return false if not true.
			//Not enough clsuters to store num bytes
			cout << "Not enough clusters to store " << filename << endl;
			return false;
		}
	}
	//Set first cluster address in file directory
	fileDirectory[i][27] = (storeClusterLocations[0] & 0xFF00);
	fileDirectory[i][26] = (storeClusterLocations[0] & 0x00FF);
	cout << filename << " location: " << hex << (unsigned short) fileDirectory[i][27] << (unsigned short)fileDirectory[i][26] << endl;

	//Fill the FAT table with the appropriate linked list addresses
	for (i = 0; i < numClusters - 1; i++)
	{
		FAT16[storeClusterLocations[i]] = storeClusterLocations[i + 1];
	}
	//Denote end of file
	FAT16[storeClusterLocations[i]] = 0xFFFF;

	cout << filename << " successfully created " << "with first cluster at: " << hex << storeClusterLocations[0] << " and the last at " << storeClusterLocations[i] <<  endl;
	return true;
}

bool FileDirectory::dlete(char filename[])
{
	//(0)	to check if the file to be deleted; filename[]; is in the Directory.If not; return false.
	unsigned short  i, j, firstClusterAddress, nextClusterAddress, file;
	bool foundFile = false;
	//Search for file in directory
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//(1) Match found, set first cluster address
					firstClusterAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];
					file = i;
					foundFile = true;
				}
				continue;
			}
			else
			{
				//Filename doesn't match, check next file
				if (i == 3)
				{
					//No match found
					return false;
				}
				break;
			}
		}
		if (foundFile) break;
	}

	//(1) change first character of the file name in the File Directory to be zero;
	fileDirectory[file][0] = '0';
	//(2)	change all entries of the clusters of this file in the FAT to 1; i.e.; unused.
	for(i = firstClusterAddress; i < 0xFFF8; i = nextClusterAddress)
	{
		nextClusterAddress = FAT16[i];
		FAT16[i] = 0x0000;
	}
	
	cout << filename << " successfully deleted." << endl;
	return true;
}

bool FileDirectory::read(char filename[])
{
	unsigned short int firstClusterAddress, clusterAddress, numClusters, p = 0;
	unsigned short int storeClusterLocation[256];
	unsigned char fileData[1024];
	/*purpose: to read  of data from data[] array from the file with the specified file name.
	(0)	to check if the file to be deleted; filename[]; is in the Directory.If not; return false.*/
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//(1) Match found, set first cluster address
					firstClusterAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];
				}
				continue;
			}
			else
			{
				//Filename doesn't match, check next file
				if (i == 3)
				{
					//No match found
					return false;
				}
				break;
			}
		}
	}
	//(2)	use the first cluster address to get all the cluster addresses of this file from the FAT - 16;
	clusterAddress = firstClusterAddress;
	for(numClusters = 0; numClusters < 256 && clusterAddress < 0xfff8; numClusters++)
	{
		storeClusterLocation[numClusters] = clusterAddress;
		clusterAddress = FAT16[clusterAddress];

	}

	/*
	(2)	use the first cluster address to get all the cluster addresses of this file from the FAT - 16;
	(3)	use all the cluster addresses to read the data from the disk / flash memory.*/
	for (int i = 0; i <= numClusters; i++)
	{
		for (int j = 0; j < 4; j++)//Each cluster has 4 bytes
		{
			fileData[p] = data[(storeClusterLocation[i] << 2) + j]; // data[cluster *4bytes + offset inside of cluster
			++p;//File data index index increment
		}
	}
	return true;
}


bool FileDirectory::write(char filename[], unsigned int numberBytes, char inputData[], unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short second)
{
	/*purpose: to write numberBytes bytes of data from data[] array into the file with the specified file name
	(0)	to look for the first unused entry(0 or 1) in the FAT - 16; and use it as the First Cluster Address.*/
	unsigned short i, freeClusterAddress;
	unsigned int allocatedBytes = 0, numClusters = numberBytes / 4;
	
	//Find filname in directory
	unsigned short empty;
	for (int i = 0; i < 4; i++)
	{
		if (fileDirectory[i][0] == '0')
		{
			empty = i;
			break;
		}
		if (i == 3) return false; //File not found
	}
	//Write filename into the ith entry of the file directory
	for(int i = 0; i < 8; i++) fileDirectory[empty][i] = filename[i];

	// Write date into directory[24:25]
	fileDirectory[empty][24] = day + (month << 5);
	fileDirectory[empty][25] = (month >> 4) + ((year - 1980) << 1);

	//TODO: Write time in to directory[23:22]
	fileDirectory[empty][22] = (second / 2) + (minute << 5);
	fileDirectory[empty][23] = (minute >> 3) + (hour << 3);

	//Get first cluster address from the created file
	freeClusterAddress = (fileDirectory[empty][27] << 8) + fileDirectory[empty][26];

	//Write file length into directory
	fileDirectory[empty][31] = numberBytes >> 24;
	fileDirectory[empty][31] = numberBytes >> 16;
	fileDirectory[empty][31] = numberBytes >> 8;
	fileDirectory[empty][31] = numberBytes;

	//Write data
	unsigned int p = 0;
	while (freeClusterAddress < 0xFFF7)
	{
		for (unsigned short j = 0; j < 4; j++)//Each cluster has 4 bytes
		{
			data[(freeClusterAddress << 2) + j] = inputData[p % 5]; // data[cluster * 4] + offset inside of cluster
			++p;//File data index index increment
		}
		freeClusterAddress = FAT16[freeClusterAddress];
	}
	return true;
}

//purpose: to print all the clusters of a file.
void FileDirectory::printClusters(char filename[])
{
	unsigned short int firstClusterAddress;
	//(1)	to check if the file to be printed; filename[]; is in the Directory.If not; return false.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//File exists
					//Store first cluster address
					firstClusterAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];

					//Print all cluster addresses in the file
					for (unsigned short i = firstClusterAddress; i < 0xFFF8; i = FAT16[i])
						cout << std::hex << i << " ";
					cout << endl;
				}
				continue;
			}
			else
			{
				//Filename doesn't match, check next file
				if (i == 3)
				{
					//No match found
					cout << filename << " does not exist in the directory." << endl;
				}
				break;
			}
		}
	}
}

//prints all the  files of the directory.
void FileDirectory::printDirectory()
{
	unsigned short int date, time;
	//prints all the  files of the directory.
	for (int i = 0; i < 4; i++)
	{
		//(1) If the file name is valid, print the filename, '.', the file extension
		//A valid filename doesn't begin with 0 (NULL)
		if (fileDirectory[i][0] != '0')
		{
			//Valid filename
			for (int j = 0; j < 8; j++)
			{
				cout << fileDirectory[i][j];
			}
			cout << '.';
			for (int j = 8; j < 3; j++)
			{
				cout << fileDirectory[i][j];
			}
			cout << "\t";

			//(2) Print date(DECODE)
			date = (fileDirectory[i][25] << 8) + fileDirectory[i][24];
			cout << (1980 + ((date & 0xFE00) >> 9)) << " ";
			//Print month
			cout << ((date & 0x01E0) >> 5) << " ";
			//Print day
			cout << (date & 0x001F);

			//Print time
			time = (fileDirectory[i][23] << 8) + fileDirectory[i][22];

			//Hour
			cout << ((time & 0xF800) >> 11) <<":";

			//Minute
			cout << ((time & 0x07E0) >> 5) << ":";

			//Second
			cout << ((time & 0x001F) << 1);
			cout << endl;
		}
	}
	cout << endl;
}



//prints the data of a file.
void FileDirectory::printData(char filename[])
{
	unsigned short int firstSectorAddress;
	bool fileFound = false;
	//Search for file in directory
	for (int i = 0; i < 4; i++)
	{
		//Check if name matches
		for (int j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//Match found
					//(1)use the file name to get the file information from the File Directory; including the first cluster address;
					firstSectorAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];
					fileFound = true;
				}
				continue;
			}
			else
			{
				if (i == 3)
				{
					//File not found in directory
					cout << filename << " does not exist in the directory." << endl;
					return;
				}
				//Filename doesn't match, check next file
				break;
			}
		}
		if (fileFound) break;
	}

	//Print the data from the file
	//(2)use the first cluster address to get all cluster addresses from the FAT - 16;
	//(3)use cluster address to read the data of the file.Use the file length to print these data in hexadecimal format.* /
	for (unsigned short i = firstSectorAddress; i < 0xFFF8; i = FAT16[i])
	{
		for (unsigned short j = 0; j < 4; j++)
		{
			cout << data[(i << 2) + j] << " ";
		}
	}
	cout << endl;
}

void FileDirectory::printFAT()
{
	//Additional function ot print the contents of the FAT table
	for (int i = 0; i < 256; i++)
	{
		cout << FAT16[i] << " ";
	}
}
