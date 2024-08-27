#include<iostream>
#include<fstream>
#include<sstream>

#include"elf_structure.h"

int main()
{
	std::ifstream fin("test");

	Elf *elf_structure = new Elf(fin);

	elf_structure->AnalyseElfHeader();
	elf_structure->AnalyseElfSectionHeader();
	//elf_structure->PrintElfSectionHeader();
	elf_structure->LoadSectionsName();
	elf_structure->LoadTables();
	//elf_structure->LoadProgramHeaderTable();
	//elf_structure->LoadDynamicSymbolName();
	//elf_structure->LoadSymbolName();

	//elf_structure->PrintDynamicSymbolInformation();
	elf_structure->PrintSymbolInformation();
}
