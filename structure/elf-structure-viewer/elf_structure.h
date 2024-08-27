#include<iostream>
#include<fstream>
#include<elf.h>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<malloc.h>
#include<map>

struct EmptyTable {};


union Table {
	struct EmptyTable empty_table;
	char *string_table;
	Elf32_Sym **symbol_table; 
	Elf32_Rel **relocation_table;
	Elf32_Dyn **dynamic_table;
};

class Elf {
public:
	std::ifstream fin;
	Elf32_Ehdr *elf_header;
	Elf32_Shdr **elf_section_headers;
	std::map<int, std::string> section_index_to_name_map;	
	std::map<std::string, int> section_name_to_index_map;
	Table **section_tables;
	Elf32_Phdr **program_headers;
	std::map<int, std::string> dynstr;
	std::map<int, std::string> strtab;

	Elf(std::ifstream fin);

	void AnalyseElfHeader();	
	void AnalyseElfSectionHeader();
	void LoadSectionsName();
	void LoadTables();
	void LoadProgramHeaderTable();
	void LoadDynamicSymbolName();
	void LoadSymbolName();

	void PrintElfSectionHeader();
	void PrintDynamicSymbolInformation();
	void PrintSymbolInformation();

};
