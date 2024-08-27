
#include"section_adder.h"

int main() {
	char file_path[] = "test";	
	char code_path[] = "code";
	char output[] = "output";
	SectionAdder *section_adder = new SectionAdder(file_path, output, code_path);
	section_adder->AddCodeSection();
	//section_adder->AddProgramHeaderTable();
	//section_adder->RemoveProhramHeaderTable();
	//section_adder->ChangeProgramHeaderTable();
	//section_adder->RemoveSectionHeaderTable();
	//section_adder->ReplaceEntryCode();
	//int begin = section_adder->AddCodeSection();
	//section_adder->ChangSymValue(0x13d0);
	
	//section_adder->WriteFile();
	
}
