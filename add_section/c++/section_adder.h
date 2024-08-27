#include<iostream>
#include<stdio.h>
#include<string.h>
#include<elf.h>

class SectionAdder {
public:
	FILE *fin;
	FILE *fout;
	FILE *fcode;

	char *file_path;
	char *output;
	char *code_path;

	char *elf_str;
	Elf32_Ehdr *elf_header;

	unsigned long long length;

	SectionAdder(char *file_path, char *output, char *code_path);

	char *GetElfStr();
	Elf32_Ehdr *GetElfHeader();	
	Elf32_Shdr **GetSectionHeaderTable();
	Elf32_Phdr **GetProgramHeaderTable();
	Elf32_Sym *GetSymbolTablebyStr(char *name);
	int GetFunctionOffset(char *func);

	void RemoveSectionHeaderTable();
	void RemoveProhramHeaderTable();
	void ChangeProgramHeaderTable();
	void ReplaceEntryCode();
	void AddCodeSection();
	int AddStrtoStrtab(char *name);
	void ChangSymValue(char *name, int value);
	void ChangSymValue(int value);
	void AddLabel(int address);
	void AddProgramHeaderTable();

	void LoadFile();
	void Rewind();
};


