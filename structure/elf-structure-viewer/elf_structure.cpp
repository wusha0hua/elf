#include"elf_structure.h"
#include <cstdlib>
#include <elf.h>
#include <utility>


Elf::Elf(std::ifstream fin) {
	this->fin = fin;
}

void Elf::AnalyseElfHeader() {
	// this->elf_header = (Elf32_Ehdr*)this->elf_str.substr(0, sizeof(Elf32_Ehdr)).c_str();
	std::memcpy(this->elf_header, this->elf_str, sizeof(Elf32_Ehdr));
}

void Elf::AnalyseElfSectionHeader() {
	int section_table_offset = this->elf_header->e_shoff;
	int section_table_number = this->elf_header->e_shnum;

	//this->elf_section_headers = new Elf32_Shdr* [section_table_number];
	this->elf_section_headers = (Elf32_Shdr **)std::malloc(sizeof(Elf32_Shdr*) * section_table_number);
	for(int i = 0; i < section_table_number; i++) {
		this->elf_section_headers[i] = (Elf32_Shdr*)std::malloc(sizeof(Elf32_Shdr));
		std::memcpy(this->elf_section_headers[i], this->elf_str + section_table_offset + i * sizeof(Elf32_Shdr) , sizeof(Elf32_Shdr));
	}

}

void Elf::LoadSectionsName() {
	int shstrtab_index = this->elf_header->e_shstrndx;
	int shstrtab_offset = this->elf_section_headers[shstrtab_index]->sh_offset;
	
	int section_table_number = this->elf_header->e_shnum;
	for(int i = 0; i < section_table_number; i++) {
		int name_str_offset = this->elf_section_headers[i]->sh_name;
		this->section_index_to_name_map.insert(std::pair<int, std::string>(i, std::string((char*)(this->elf_str + shstrtab_offset + name_str_offset))));
		this->section_name_to_index_map.insert(std::pair<std::string, int>(std::string((char*)(this->elf_str + shstrtab_offset + name_str_offset)), i));
	}	
	/*
	std::map<int, std::string>::iterator it, itend;
	it = this->section_name_map.begin();
	itend = this->section_name_map.end();
	while(it != itend) {
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}
	*/
}

void Elf::LoadTables() {
	int section_table_number = this->elf_header->e_shnum;
	//this->tables = (Table**)std::malloc(sizeof(Table*) * section_table_number);
	this->section_tables = new Table* [section_table_number];
	
	for(int i = 0; i < section_table_number; i++) {
		int section_table_offset = this->elf_section_headers[i]->sh_offset;
		int section_table_size = this->elf_section_headers[i]->sh_size;
		int table_number;

		this->section_tables[i] = new Table;
		switch(this->elf_section_headers[i]->sh_type)
		{
			case SHT_NULL:
				break;

			case SHT_STRTAB:
				this->section_tables[i]->string_table = new char[section_table_size];
				std::memcpy(this->section_tables[i]->string_table, this->elf_str + section_table_offset, section_table_size);
				/*
				for(int j = 0; j < section_table_size; j++)
				{
					std::cout << this->section_tables[i]->string_table[j];
				}
				*/
				break;

			case SHT_DYNSYM: 
				table_number = section_table_size / sizeof(Elf32_Sym);
				//this->section_tables[i] = new Table;
				this->section_tables[i]->symbol_table = new Elf32_Sym* [table_number];
				for(int j = 0; j < table_number; j++) {
					this->section_tables[i]->symbol_table[j] = new Elf32_Sym;	
					std::memcpy(this->section_tables[i]->symbol_table[j], this->elf_str + section_table_offset + j * sizeof(Elf32_Sym), sizeof(Elf32_Sym));
				}
			
				break;

			case SHT_SYMTAB: 
				table_number = section_table_size / sizeof(Elf32_Sym);
				//this->section_tables[i] = new Table;
				this->section_tables[i]->symbol_table = new Elf32_Sym* [table_number];
				for(int j = 0; j < table_number; j++) {
					this->section_tables[i]->symbol_table[j] = new Elf32_Sym;	
					std::memcpy(this->section_tables[i]->symbol_table[j], this->elf_str + section_table_offset + j * sizeof(Elf32_Sym), sizeof(Elf32_Sym));
				}
				break;
			

			case SHT_PROGBITS: 
				this->section_tables[i]->string_table = new char[section_table_size];
				std::memcpy(this->section_tables[i]->string_table, this->elf_str + section_table_offset, section_table_size);
				break;

			case SHT_REL: 
				table_number = section_table_size / sizeof(Elf32_Rel);
				this->section_tables[i]->relocation_table = new Elf32_Rel* [table_number];
				for(int j = 0; j < table_number; j++) {
					this->section_tables[i]->relocation_table[j] = new Elf32_Rel;
					std::memcpy(this->section_tables[i]->symbol_table[j], this->elf_str + section_table_offset + j * sizeof(Elf32_Rel), sizeof(Elf32_Rel));
				}
				break;

			case SHT_DYNAMIC:
				table_number = section_table_size / sizeof(Elf32_Dyn);
				this->section_tables[i]->dynamic_table = new Elf32_Dyn* [table_number];
				for(int j = 0; j < table_number; j++) {
					this->section_tables[i]->dynamic_table[j] = new Elf32_Dyn;
					std::memcpy(this->section_tables[i]->dynamic_table[j], this->elf_str + section_table_offset + j * sizeof(Elf32_Dyn), sizeof(Elf32_Dyn));
				}

		}

	}	
}

void Elf::LoadProgramHeaderTable() {
	int program_table_number = this->elf_header->e_phnum;
	int program_table_offset = this->elf_header->e_phoff;
	this->program_headers = new Elf32_Phdr* [program_table_number];
	for(int i = 0; i < program_table_number; i++) {
		this->program_headers[i] = new Elf32_Phdr;
		std::memcpy(this->program_headers[i], this->elf_str + program_table_offset + i * sizeof(Elf32_Phdr), sizeof(Elf32_Phdr));
	}
}

void Elf::LoadDynamicSymbolName() {
	int dynstr_index = this->section_name_to_index_map[std::string(".dynstr")];
	int dynsym_index = this->section_name_to_index_map[std::string(".dynsym")];
	char *dynstr = this->section_tables[dynstr_index]->string_table;		
	Table *dynsym_tables = this->section_tables[dynsym_index];
	for(int i = 0; i < this->elf_section_headers[dynsym_index]->sh_size / sizeof(Elf32_Sym); i++) {
		char *str = (char*)(dynstr + dynsym_tables->symbol_table[i]->st_name);
		this->dynstr.insert(std::pair<int, std::string>(dynsym_tables->symbol_table[i]->st_name, std::string(str)));
	}

}

void Elf::LoadSymbolName() {
	int strtab_index = this->section_name_to_index_map[std::string(".strtab")];
	int symtab_index = this->section_name_to_index_map[std::string(".symtab")];
	char *strtab = this->section_tables[strtab_index]->string_table;		
	Table *symtab_tables = this->section_tables[symtab_index];
	for(int i = 0; i < this->elf_section_headers[symtab_index]->sh_size / sizeof(Elf32_Sym); i++) {
		char *str = (char*)(strtab + symtab_tables->symbol_table[i]->st_name);
		int index = symtab_tables->symbol_table[i]->st_name;
		std::cout << index << " " << str << std::endl;
		//this->strtab[index] = std::string(str);
		this->strtab.insert(std::pair<int, std::string>(symtab_tables->symbol_table[i]->st_name, std::string(str)));
	}
}



void Elf::PrintElfSectionHeader() {
	int section_table_number = this->elf_header->e_shnum;
	for(int i = 0; i < section_table_number; i++)
	{
		std::cout << "sh_name: " << this->elf_section_headers[i]->sh_name << std::endl;
		std::cout << "sh_type: " << this->elf_section_headers[i]->sh_type << std::endl;
		std::cout << "sh_flags " << this->elf_section_headers[i]->sh_flags << std::endl;
		std::cout << "sh_addr: " << this->elf_section_headers[i]->sh_addr << std::endl;
		std::cout << "sh_offset: " << this->elf_section_headers[i]->sh_offset << std::endl;
		std::cout << "sh_size: " << this->elf_section_headers[i]->sh_size << std::endl;
		std::cout << "sh_link: " << this->elf_section_headers[i]->sh_link << std::endl;
		std::cout << "sh_info: " << this->elf_section_headers[i]->sh_info << std::endl;
		std::cout << "sh_addralign: " << this->elf_section_headers[i]->sh_addralign << std::endl;
		std::cout << "sh_entsize: " << this->elf_section_headers[i]->sh_entsize << std::endl << std::endl;
	}
}

void Elf::PrintDynamicSymbolInformation() {
	/*
	for(std::map<int, std::string>::iterator begin = this->dynstr.begin(), end = this->dynstr.end(); begin != end; begin++) {
		std::cout << begin->second << std::endl;
	}
	*/
	int dynstr_index = this->section_name_to_index_map[std::string(".dynstr")];
	int dynsym_index = this->section_name_to_index_map[std::string(".dynsym")];
	char *dynstr = this->section_tables[dynstr_index]->string_table;		
	Table *dynsym_tables = this->section_tables[dynsym_index];
	for(int i = 0; i < this->elf_section_headers[dynsym_index]->sh_size / sizeof(Elf32_Sym); i++) {
		std::cout << "index: " << i << std::endl;
		std::cout << "\tname: " << (char*)(dynstr + dynsym_tables->symbol_table[i]->st_name) << std::endl;
		std::cout << "\tvalue: " << dynsym_tables->symbol_table[i]->st_value << std::endl;
		std::cout << "\tsize: " << dynsym_tables->symbol_table[i]->st_size << std::endl;
		std::cout << "\tbinding: ";
		switch(dynsym_tables->symbol_table[i]->st_info >> 4) {
			case 0:
				std::cout << "local";
				break;
			case 1:
				std::cout << "global";
				break;
			case 2:
				std::cout << "weak";
				break;
		}
		std::cout << std::endl;
		std::cout << "\ttype: ";
		switch(dynsym_tables->symbol_table[i]->st_info & 0xf) {
			case 0:
				std::cout << "notype";
				break;
			case 1:
				std::cout << "object";
				break;
			case 2:
				std::cout << "function";
				break;
			case 3:
				std::cout << "section";
				break;
			case 4:
				std::cout << "file";
				break;
		}
		std::cout << std::endl;
		std::cout << "\tother: " << dynsym_tables->symbol_table[i]->st_other << std::endl;
		std::cout << "\tsection index: " << dynsym_tables->symbol_table[i]->st_shndx << std::endl;
	}
}

void Elf::PrintSymbolInformation() {
	/*
	for(std::map<int, std::string>::iterator begin = this->strtab.begin(), end = this->strtab.end(); begin != end; begin++) {
		std::cout << begin->second << std::endl;
	}
	*/
	int strtab_index = this->section_name_to_index_map[std::string(".strtab")];
	int symtab_index = this->section_name_to_index_map[std::string(".symtab")];
	char *strtab = this->section_tables[strtab_index]->string_table;		
	Table *symtab_tables = this->section_tables[symtab_index];
	for(int i = 0; i < this->elf_section_headers[symtab_index]->sh_size / sizeof(Elf32_Sym); i++) {
		std::cout << "index: " << i << std::endl;
		std::cout << "\tname: " << (char*)(strtab + symtab_tables->symbol_table[i]->st_name) << std::endl;
		std::cout << "\tvalue: " << symtab_tables->symbol_table[i]->st_value << std::endl;
		std::cout << "\tsize: " << symtab_tables->symbol_table[i]->st_size << std::endl;
		std::cout << "\tbinding: ";
		switch(symtab_tables->symbol_table[i]->st_info >> 4) {
			case 0:
				std::cout << "local";
				break;
			case 1:
				std::cout << "global";
				break;
			case 2:
				std::cout << "weak";
				break;
		}
		std::cout << std::endl;
		std::cout << "\ttype: ";
		switch(symtab_tables->symbol_table[i]->st_info & 0xf) {
			case 0:
				std::cout << "notype";
				break;
			case 1:
				std::cout << "object";
				break;
			case 2:
				std::cout << "function";
				break;
			case 3:
				std::cout << "section";
				break;
			case 4:
				std::cout << "file";
				break;
		}
		std::cout << std::endl;
		std::cout << "\tother: " << symtab_tables->symbol_table[i]->st_other << std::endl;
		std::cout << "\tsection index: " << symtab_tables->symbol_table[i]->st_shndx << std::endl;

	}
}
