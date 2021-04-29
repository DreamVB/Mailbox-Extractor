// Mail message extractor
// Extract messages from inbox files such as Thunderbird or UseNet
// by DreamVB
// Last update 19:54 29/04/2021
// Version 1.2

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using std::cout;
using std::endl;

string Dec2Str(int value){
	ostringstream os;
	os << value;
	return os.str();
}

string UnixToDos(std::string src){
	size_t i = 0;
	std::string buffer;

	while (i < src.length()){
		if (src[i] == '\n'){
			//Check for \r
			if (src[i] != '\r'){
				buffer += "\r\n";
			}
			else{
				buffer += "\n";
			}
		}
		else{
			buffer += src[i];
		}
		i++;
	}
	return buffer;
}

int main(int argc, char *argv[]){

	fstream fs;
	fstream fout;
	std::string StrLine;
	std::string PrevStr;
	std::string sOut;
	std::string sOutFile;
	std::string sMessage;
	size_t x = 0;

	if (argc < 2){
		cout << "Usage: " << argv[0] << " Source Output-Folder" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Get inbox filename.
	sOut = argv[2];

	//Display message.
	std::cout << "Reading source file." << std::endl;

	fs.open(argv[1], std::ios::in | std::ios::binary);

	//Check if file is open.
	if (!fs.is_open()){
		std::cout << "Cannot read the source file." << std::endl;
		std::cout << argv[1] << std::endl;
		exit(EXIT_FAILURE);
	}

	//Read the inbox file and extract the messages.
	while (getline(fs,StrLine)){
		//Check for line length
		if (StrLine.length() > 0){
			//Check for the string From this is were the message begins
			if (StrLine.substr(0, 5) == "From "){
				//Display message.
				std::cout << "Extracting: " << StrLine.c_str() << std::endl;
				//If StrLine not equal Prev String then output message data
				if (StrLine != PrevStr){
					//Check the length of the message.
					if (sMessage.length() > 0){
						//Set output file
						sOutFile = sOut + "Message_" + Dec2Str(x) + ".txt";
						//Open the file for output
						fout.open(sOutFile, std::ios::out | std::ios::binary);
						//Check that the file was open if not exit.
						if (!fout.good()){
							std::cout << "Cannot create output file: " << std::endl;
							std::cout << sOutFile.c_str() << std::endl;
							break;
						}
						sMessage = UnixToDos(sMessage);
						//Write data to the file.
						fout.write(sMessage.c_str(), sMessage.length());
						//Close file
						fout.close();
						//Clear message.
					}
				}
				//INC counter
				x++;
				sMessage.clear();
			}
			//Build the message.
			sMessage += StrLine + "\n";
			//Store prev string
			PrevStr = StrLine;
		}
	}
	
	//Output last one
	sMessage = UnixToDos(sMessage);
	sOutFile = sOut + "Message_" + Dec2Str(x) + ".txt";
	fout.open(sOutFile, std::ios::out | std::ios::binary);
	fout.write(sMessage.c_str(), sMessage.length());
	//Close file
	fs.close();

	//Display messages found.
	std::cout << std::endl << "Finished " << x << " messages extracted." << std::endl;

	//Clear up.
	sMessage.clear();
	StrLine.clear();

	return EXIT_SUCCESS;
}
