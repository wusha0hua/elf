#include"section_adder.h"

SectionAdder::SectionAdder(char* file_path, char* output, char* code_path) {
	this->fin = fopen(file_path, "rb");
	this->fout = fopen(output, "wb");
	this->fcode = fopen(code_path, "rb");

	this->code_path = code_path;
	this->output = output;
	this->file_path = file_path;
}

void SectionAdder::RemoveSectionHeaderTable() {
	Elf32_Ehdr *elf_header = new Elf32_Ehdr;
	fread(elf_header, sizeof(Elf32_Ehdr), 1, this->fin);
	rewind(this->fin);
	long length = elf_header->e_shoff;
	char *output = new char [length];
	fread(output, length, 1, this->fin);
	elf_header->e_shoff = 0;
	memcpy(output, elf_header, sizeof(Elf32_Ehdr));
	fwrite(output, length, 1, this->fout);

	this->Rewind();
}

void SectionAdder::AddCodeSection() {
	Elf32_Ehdr *elf_header = new Elf32_Ehdr;
	fread(elf_header, sizeof(Elf32_Ehdr), 1, this->fin);
	rewind(this->fin);

	fseek(this->fin, 0, SEEK_END);
	long length = ftell(this->fin);
	rewind(this->fin);
	
	char *elf_str = new char [length];
	fread(elf_str, length, 1, this->fin);
	rewind(this->fin);

	int program_header_table_num = elf_header->e_phnum;
	int program_header_table_size = elf_header->e_phentsize;
	int program_header_table_offset = elf_header->e_phoff;

	int section_header_table_num = elf_header->e_shnum;
	int section_header_table_size = elf_header->e_shentsize;
	int section_header_table_offset = elf_header->e_shoff;

	int code_begin, code_end, code_size, align, max_len;

	fseek(this->fcode, 0, SEEK_END);
	long code_len = ftell(this->fcode);
	rewind(this->fcode);
	char *code = new char [code_len];
	fread(code, code_len, 1, this->fcode);
	rewind(this->fcode);

	code_size = code_len;

	// set program_header_table
	for(int i = 0; i < program_header_table_num; i++) {
		Elf32_Phdr *program_header_table = new Elf32_Phdr;
		memcpy(program_header_table, elf_str + program_header_table_offset + i * program_header_table_size, program_header_table_size);	
		if(program_header_table->p_flags & PF_X) {
			align = program_header_table->p_align;
			code_begin = program_header_table_offset + program_header_table->p_filesz;
			code_begin = ((code_begin / align) + 1) * align;	
			code_end = code_begin + code_size;
			code_end = ((code_size / align) + 1) * align;

			program_header_table->p_filesz += ((code_size / align) + 1) * align;
			program_header_table->p_memsz += ((code_size / align) + 1) * align;

			memcpy(elf_str + program_header_table_offset + i * program_header_table_size, program_header_table, sizeof(Elf32_Phdr));
			break;
		}
	}
	
	// set section_header_table
	Elf32_Shdr *section_header_table = new Elf32_Shdr;
	section_header_table->sh_name = 5;
	section_header_table->sh_type = SHT_PROGBITS;
	section_header_table->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	section_header_table->sh_addr = code_begin;
	section_header_table->sh_size = code_size;
	section_header_table->sh_link = 0;
	section_header_table->sh_info = 0;
	section_header_table->sh_addralign = align;
	section_header_table->sh_entsize = 0;

	char *tmp = new char [length];
	memcpy(tmp, elf_str, length);
	elf_str = new char [length + sizeof(Elf32_Shdr)];
	memcpy(elf_str, tmp, length);
	memcpy(elf_str + length, section_header_table, sizeof(Elf32_Shdr));
	length += sizeof(Elf32_Shdr);
	delete[] tmp;

	// load elf header
	elf_header->e_shnum += 1;
	elf_header->e_entry = code_begin;
	memcpy(elf_str, elf_header, sizeof(Elf32_Ehdr));

	// get symstr table
	int str_index = elf_header->e_shstrndx;
	Elf32_Shdr *strtab = new Elf32_Shdr;
	memcpy(strtab, elf_str + section_header_table_offset + section_header_table_size * str_index, section_header_table_size);
	int str_offset = strtab->sh_offset;
	int str_size = strtab->sh_size;
	char *str = new char [str_size];
	memcpy(str, elf_str + str_offset, str_size);
	char sym_name[] = ".symstr";
	Elf32_Shdr *symstr = new Elf32_Shdr;
	for(int i = 0; i < section_header_table_num; i++) {
		Elf32_Shdr *section_header_table = new Elf32_Shdr;
		memcpy(section_header_table, elf_str + section_header_table_offset + i * section_header_table_size, section_header_table_size);
		if(!strcmp(str + section_header_table->sh_name, sym_name)) {
			memcpy(symstr, section_header_table, sizeof(Elf32_Shdr));
			break;
		}
	}

	// get sym str
	int sym_str_offset = symstr->sh_offset;
	int sym_str_size = symstr->sh_size;
	char *sym_str = new char [sym_str_size];
	memcpy(sym_str, elf_str + sym_str_offset, sym_str_size);

	// get symtab table
	str_index = elf_header->e_shstrndx;
	memcpy(strtab, elf_str + section_header_table_offset + section_header_table_size * str_index, section_header_table_size);
	str_offset = strtab->sh_offset;
	str_size = strtab->sh_size;
	memcpy(str, elf_str + str_offset, str_size);
	char symtab_name[] = ".symtab";
	Elf32_Shdr *symtab = new Elf32_Shdr;
	for(int i = 0; i < section_header_table_num; i++) {
		Elf32_Shdr *section_header_table = new Elf32_Shdr;
		memcpy(section_header_table, elf_str + section_header_table_offset + i * section_header_table_size, section_header_table_size);
		if(!strcmp(str + section_header_table->sh_name, sym_name)) {
			memcpy(symstr, section_header_table, sizeof(Elf32_Shdr));
			break;
		}
	}

	int symtab_size = symtab->sh_size;
	int symtab_offset = symtab->sh_offset;
	int symtab_num = symtab->sh_size / sizeof(Elf32_Sym);
	Elf32_Sym *sym = new Elf32_Sym;
	char func_name[] = "_start";
	for(int i = 0; i< symtab_num; i++) {
		memcpy(sym, elf_str + symtab_offset + i * sizeof(Elf32_Sym), sizeof(Elf32_Sym));
		//if(!strcmp())
	}



	// set code jmp address
	
}

void SectionAdder::AddProgramHeaderTable() {
	Elf32_Ehdr *elf_header = new Elf32_Ehdr;
	fread(elf_header, sizeof(Elf32_Ehdr), 1, this->fin);
	rewind(this->fin);

	fseek(this->fin, 0, SEEK_END);
	long length = ftell(this->fin);
	rewind(this->fin);
	
	char *elf_str = new char [length];
	fread(elf_str, length, 1, this->fin);
	rewind(this->fin);

	int num = elf_header->e_phnum;
	int size = elf_header->e_phentsize;
	int offset = elf_header->e_phoff;
	int section_offset = elf_header->e_shoff;
	elf_header->e_phnum += 1;
	elf_header->e_shoff += sizeof(Elf32_Phdr);
	elf_header->e_shnum += 1;
	fwrite(elf_header, sizeof(Elf32_Ehdr), 1, this->fout);

	fseek(this->fcode, 0, SEEK_END);
	long code_size = ftell(this->fcode);
	rewind(this->fcode);
	char *code = new char [code_size];
	fread(code, code_size, 1, this->fcode);
	int code_offset;

	Elf32_Phdr program_header_table;
	int code_begin, align;
	for(int i = 0; i < num; i++) {
		memcpy(&program_header_table, elf_str + i * size + offset, size);
		program_header_table.p_offset += sizeof(Elf32_Phdr);
		//fwrite(&program_header_table, size, 1, this->fout);
		/*
		if(program_header_table.p_flags & PF_X) {
			align = program_header_table.p_align;
			code_begin = program_header_table.p_offset + program_header_table.p_filesz;
			code_begin = ((code_begin / align) + 1) * align;
			
			program_header_table.p_filesz += ((code_size / align) + 1) * align;
			program_header_table.p_memsz += ((code_size / align) + 1) * align;
		}
		*/
		fwrite(&program_header_table, size, 1, this->fout);
	}

	int new_program_begin = elf_header->e_shoff;
	//memset(&program_header_table, 0, sizeof(Elf32_Phdr));
	program_header_table.p_type = PT_LOAD;
	program_header_table.p_offset = new_program_begin;
	program_header_table.p_vaddr = 0x5000;
	program_header_table.p_paddr = 0x5000;
	program_header_table.p_filesz = code_size;
	program_header_table.p_memsz = code_size;
	program_header_table.p_flags = PF_X | PF_R;
	program_header_table.p_align = 0x1000;
		
	fwrite(&program_header_table, sizeof(Elf32_Phdr), 1, this->fout);
	
	int len = sizeof(Elf32_Ehdr) + num * size;
	int section_len = elf_header->e_shnum * elf_header->e_shentsize;
	fwrite(elf_str + len, length - section_len - len, 1, this->fout);

	int padding_size = 0x1000 - code_size;
	char *padding = new char [padding_size];
	fwrite(code, code_size, 1, this->fout);
	fwrite(padding, padding_size, 0, this->fout);
	code_begin = elf_header->e_shoff;

	char *section_tables = new char [section_len];
	memcpy(section_tables, elf_str + section_offset, section_len);
	for(int i = 0; i < elf_header->e_shnum; i++) {
		Elf32_Shdr * section = (Elf32_Shdr*)(section_tables + i * sizeof(Elf32_Shdr));
		section->sh_offset += sizeof(Elf32_Phdr) + 0x1000;
	}
	fwrite(section_tables, section_len, 1, this->fout);

	Elf32_Shdr section_table;
	section_table.sh_name = 5;
	section_table.sh_type = SHT_PROGBITS;
	section_table.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	section_table.sh_addr = code_begin;
	section_table.sh_offset = code_begin;
	section_table.sh_size = code_size;
	section_table.sh_link = 0;
	section_table.sh_info = 0;
	section_table.sh_addralign = 16;
	section_table.sh_entsize = 0;

	fwrite(&section_table, sizeof(Elf32_Shdr), 1, this->fout);
	//printf("%d\n", section_len);
}

void SectionAdder::Rewind() {
	rewind(this->fin);
	rewind(this->fout);
	rewind(this->fcode);
}

void SectionAdder::LoadFile() {
	this->fin = fopen(this->file_path, "rb");
	this->fout = fopen(this->output, "wb");
	this->fcode = fopen(this->code_path, "rb");
}

