[TOC]



# Structure



## ELF Header



<table>
    <tr>
        <th><center> name </center></th>
        <th><center> size </center></th>
        <th><center> type </center></th>
    </tr>

    <tr>
        <td><center> e_ident </center></td>
        <td><center> 16B </center></td>
        <td><center> int8[16] </center></td>
    </tr>
        
    <tr>
        <td><center> e_type </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
       
    <tr>
        <td><center> e_machine </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_version </center></td>
        <td><center> 4B </center></td>
        <td><center> int32 </center></td>
    </tr>
        
    <tr>
        <td><center> e_entry </center></td>
        <td><center> 4B / 8B </center></td>
        <td><center> int32 / int64 </center></td>
    </tr>
        
    <tr>
        <td><center> e_phoff </center></td>
        <td><center> 4B / 8B </center></td>
        <td><center> int32 / int64 </center></td>
    </tr>
        
    <tr>
        <td><center> e_shoff </center></td>
        <td><center> 4B / 8B </center></td>
        <td><center> int32 / int64 </center></td>
    </tr>
        
    <tr>
        <td><center> e_flags </center></td>
        <td><center> 4B </center></td>
        <td><center> int32 </center></td>
    </tr>
        
    <tr>
        <td><center> e_ehsize </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_phentsize </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_phnum </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_shentsize </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_ shnum </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
        
    <tr>
        <td><center> e_shstrndx </center></td>
        <td><center> 2B </center></td>
        <td><center> int16 </center></td>
    </tr>
</table>



**e_ident** :

- [0 - 3] : 0x7f , 'E', 'L', 'F'
- 4 : ELF_CLASS ; ELFCLASSNONE = 0 ; invalid ; ELFCLASS32 = 1 ; ELFCLASS64 = 2
- 5 :  EI_DATA ; ELFDATANONE = 0; ELFDATA2LSB = 1 ;ELFDATA2MSB = 2;
- 6 : EI_VERSION = EV_CURRENT

- 7 : EI_OSABI = 0 : Unix

**e_type** : 

- ET_NONE = 0 ; no file type
- ET_REL = 1 ; relocation file
- ET_EXEC = 2 ; excutable file
- ET_DYN = 3 ; shared object file 
- ET_CORE = 4 ; core file
- ET_NUM = 5 ; number of defined types



**e_machine** : 

- EM_NONE = 0 ; no machine
- EM_M32 = 1 ; AT&T WE 32100
- EM_386 = 3 ; intel 80386
- EM_860 = 7 ; intel 80860
- EM_MIPS = 8 ; mips R3000 big-endian



**e_version** : 

- EV_NONE  = 0 ; invalid 
- EV_CURRENT = 1 ; usually
- EV_NUM = 2 ;



**e_entry** :

entry point address



**e_phoff** : program header table offset



**e_shoff** : section header tables offset



**e_flags** : no in intel x86 and x64 



**e_ehsize** : size of ELF header



**e_phentsize** : program header table entry size



**e_phnum** : program header table entry count



**e_shentsize** : section header table entry size



**e_shnum** : section header table entry count



**e_shstrndx** : section header string table index 





## Section Header



<table>
    <tr>
        <th><center> name </center></th>
        <th><center> size </center></tr>
    </tr>

	<tr>
	    <td><center> sh_name </center></td>
	    <td><center> int32 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_type </center></td>
	    <td><center> int32 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_flags </center></td>
	    <td><center> int32 / int64 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_offset </center></td>
	    <td><center> int32 / int64 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_size </center></td>
	    <td><center> int32 / int64 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_link </center></td>
	    <td><center> int32 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_info </center></td>
	    <td><center> int32 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_addralign </center></td>
	    <td><center> int32 / int64 </center></td>
	</tr>
	
	<tr>
	    <td><center> sh_ensize </center></td>
	    <td><center> int32 / int64 </center></td>
	</tr>


</table>



**sh_name** : index of name in header string table

- .bss : uninitialized data (global and static variate)
- .comment : 



**sh_type** : section type

- SHT_NULL = 0
- SHT_PROGBITS = 1 : 



**sh_flag** :



**sh_addr** : VOA



**sh_offset** : FOA



**sh_size** : 



**sh_link** : 



**sh_info** :



**sh_addralign** :



**sh_entsize** :



- .bss：包含程序运行时未初始化的数据（全局变量和静态变量）。当程序运行时，这些数据初始化为0。 其类型为`SHT_NOBITS`，表示不占文件空间。`SHF_ALLOC` + `SHF_WRITE`，运行时要占用内存的。
- .comment 包含版本控制信息（是否包含程序的注释信息？不包含，注释在预处理时已经被删除了）。类型为`SHT_PROGBITS`。
- .data和.data1，包含初始化的全局变量和静态变量。 类型为`SHT_PROGBITS`，标志为`SHF_ALLOC` + `SHF_WRITE`（占用内存，可写）。
- .debug，包含了符号调试用的信息，我们要想用`gdb`等工具调试程序，需要该类型信息，类型为`SHT_PROGBITS`。
- .dynamic，类型`SHT_DYNAMIC`，包含了动态链接的信息。标志`SHF_ALLOC`，是否包含`SHF_WRITE`和处理器有关。
- .dynstr，`SHT_STRTAB`，包含了动态链接用的字符串，通常是和符号表中的符号关联的字符串。标志 `SHF_ALLOC`
- .dynsym，类型`SHT_DYNSYM`，包含动态链接符号表， 标志`SHF_ALLOC`。
- .fini，类型`SHT_PROGBITS`，程序正常结束时，要执行该section中的指令。标志`SHF_ALLOC + SHF_EXECINSTR`（占用内存可执行）。现在ELF还包含`.fini_array` section。
- .got，类型`SHT_PROGBITS`，全局偏移表(global offset table)，以后会重点讲。
- .hash，类型`SHT_HASH`，包含符号hash表，以后细讲。标志`SHF_ALLOC`。
- .init，`SHT_PROGBITS`，程序运行时，先执行该节中的代码。`SHF_ALLOC + SHF_EXECINSTR`，和.fini对应。现在ELF还包含`.init_array` section。
- .interp，`SHT_PROGBITS`，该节内容是一个字符串，指定了程序解释器的路径名。如果文件中有一个可加载的segment包含该节，属性就包含`SHF_ALLOC`，否则不包含。
- .line，`SHT_PROGBITS`，包含符号调试的行号信息，描述了源程序和机器代码的对应关系。`gdb`等调试器需要此信息。
- .note `Note Section`, 类型`SHT_NOTE`，以后单独讲。
- .plt 过程链接表（Procedure Linkage Table），类型`SHT_PROGBITS`,以后重点讲。
- .relNAME，类型`SHT_REL`, 包含重定位信息。如果文件有一个可加载的segment包含该section，section属性将包含`SHF_ALLOC`，否则不包含。NAME，是应用重定位的节的名字，比如.text的重定位信息存储在.rel.text中。
- .relaname 类型`SHT_RELA`，和.rel相同。`SHT_RELA`和`SHT_REL`的区别，会在讲重定位的时候说明。
- .rodata和.rodata1。类型`SHT_PROGBITS`, 包含只读数据，组成不可写的段。标志`SHF_ALLOC`。
- .shstrtab，类型`SHT_STRTAB`，包含section的名字。有读者可能会问：section header中不是已经包含名字了吗，为什么把名字集中存放在这里？ `sh_name` 包含的是.shstrtab 中的索引，真正的字符串存储在.shstrtab中。那么section names为什么要集中存储？我想是这样：如果有相同的字符串，就可以共用一块存储空间。如果字符串存在包含关系，也可以共用一块存储空间。
- .strtab `SHT_STRTAB`，包含字符串，通常是符号表中符号对应的变量名字。如果文件有一个可加载的segment包含该section，属性将包含`SHF_ALLOC`。字符串以`\0`结束， section以`\0`开始，也以`\0`结束。一个.strtab可以是空的，它的`sh_size`将是0。针对空字符串表的非0索引是允许的。
- symtab，类型`SHT_SYMTAB`，Symbol Table，符号表。包含了定位、重定位符号定义和引用时需要的信息。符号表是一个数组，Index 0 第一个入口，它的含义是undefined symbol index， `STN_UNDEF`。如果文件有一个可加载的segment包含该section，属性将包含`SHF_ALLOC`。