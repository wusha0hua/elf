#include<iostream>
#include<elf.h>
#include<string>
#include<fstream>
#include<string.h>
#include<map>

using namespace std;

int main(int argc, char **argv)
{
	string MAGIC = string("") + char(0x7f) + 'E' + 'L' + 'F';
	
	unsigned char Class;
	map<unsigned char, string> CLASS;	
	CLASS[0] = "None";
	CLASS[1] = "ELF32";
	CLASS[2] = "ELF64";

	unsigned char Data;
	map<unsigned char, string> DATA;
	DATA[1] = "little endian";
	DATA[2] = "big endian";

	unsigned char Version;

	unsigned char Osabi;
	map<unsigned char, string> OSABI;
	OSABI[0] = "Unix - System V";

	unsigned short Type;
	map<unsigned char, string> TYPE;
	TYPE[1] = "REL";
	TYPE[2] = "EXEC";
	TYPE[3] = "DNY";
	TYPE[4] = "CORE";
	TYPE[5] = "NUM";

	unsigned short Machine;
	map<unsigned short, string> MACHINE;
	MACHINE[1] = "AT&T WE R3000";
	MACHINE[3] = "Intel 80386";
	MACHINE[7] = "Intel 80860";
	MACHINE[8] = "Mips R3000 big-endian";

	unsigned int e_Version;
	map<unsigned int, string> VERSION;
	VERSION[1] = "Current";

	unsigned long EntryPointAdress;
	unsigned long HeaderOffset;
	unsigned long SectionHeaderOffset;
	unsigned int Flags;
	unsigned short HeaderSize;
	unsigned short HeaderTableEntrySize;
	unsigned short HeaderTableEntryNumber;
	unsigned short SectionHeaderTableEntrySize;
	unsigned short SectionHeaderTableEntryNumber;
	unsigned short SectionHeaderStringTableIndex;
	
	if(argc != 2)
	{
		cout << "input elf file name" << endl;
	}		

	string path = argv[1];
		
	ifstream ifs(path, ios::in | ios::binary);
	if(!ifs)
	{
		cout << "open " + path + "error" << endl;
		return -1;
	}
	
	char *buf = new char[8];	
	memset(buf, 0, 8);	
	unsigned char c;

	if(!ifs.read(buf, 4))
	{
		cout << "read e_indent error : " + string(buf) << endl;
		return -1;
	}
	if(!(MAGIC == string(buf)))
	{
		cout << "MAGIC: " + string(buf) + "error" << endl;
	}

	if(!ifs.read((char*)&Class, 1))	
	{
		cout << "read e_indent error"<< endl;
		return -1;
	}

	if(!ifs.read((char*)&Data, 1))
	{
		cout << "read e_indent error" << endl;
		return -1;
	}

	if(!ifs.read((char*)&Version, 1))
	{
		cout << "read e_indent error" << endl;
		return -1;
	}

	if(!ifs.read((char*)&Osabi, 1))
	{
		cout << "read e_indent error" <<endl;
		return -1;
	}
	
	if(!ifs.read(buf, 8))
	{
		cout << "read e_indent error" << endl;
		return -1;
	}

	if(!ifs.read((char*)&Type, 2))
	{
		cout << "read type error" << endl;
		return -1;
	}

	if(!ifs.read((char*)&Machine, 2))
	{
		cout << "read machine error" << endl;
		return -1;
	}

	if(!ifs.read((char*)&e_Version, 4))
	{
		cout << "read version error" << endl;
		return -1;
	}

	if(Class == 1)
	{
		ifs.read((char*)&EntryPointAdress, 4);
		ifs.read((char*)&HeaderOffset, 4);
		ifs.read((char*)&SectionHeaderOffset, 4);
	}
	else if(Class == 2)
	{
		ifs.read((char*)&EntryPointAdress,8);
		ifs.read((char*)&HeaderOffset, 8);
		ifs.read((char*)&SectionHeaderOffset, 8);
	}
	else
	{
		cout << "Class error" << endl;	
		return -1;
	}

	ifs.read((char*)&Flags, 4);
	ifs.read((char*)&HeaderSize, 2);
	ifs.read((char*)&HeaderTableEntrySize, 2);
	ifs.read((char*)&HeaderTableEntryNumber, 2);
	ifs.read((char*)&SectionHeaderTableEntrySize, 2);
	ifs.read((char*)&SectionHeaderTableEntryNumber, 2);
	ifs.read((char*)&SectionHeaderStringTableIndex, 2);

	cout << "Magic : " + MAGIC << endl;
	cout << "Class : " + CLASS[Class]  << endl;
	cout << "Data : " + DATA[Data] << endl;
	cout << "OS/ABI : " + OSABI[Osabi]  << endl;
	cout << "Type : " + TYPE[Type]  << endl;
	cout << "Machine : " + MACHINE[Machine]  << endl;
	cout << "Version : " + VERSION[Version] << endl;
	cout << "Entry point adress : 0x" << hex << EntryPointAdress << endl;
	cout << "Start of pogram headers : " << dec << HeaderOffset << endl;
	cout << "Start of section headers : " << SectionHeaderOffset << endl;
	cout << "Flags : " << Flags << endl;
	cout << "Size of this header : " << HeaderSize << endl;
	cout << "Size of program headers : " << HeaderTableEntrySize << endl;
	cout << "Number of program headers : " << HeaderTableEntryNumber << endl;
	cout << "Size of section headers : " << SectionHeaderTableEntrySize << endl;
	cout << "Number of section headers : " << SectionHeaderTableEntryNumber << endl;
	cout << "Section header string table index : " << SectionHeaderStringTableIndex << endl;
}

