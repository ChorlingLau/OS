/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}
int is_MSB(u_char *binary) {
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
	if (ehdr->e_ident[5] == 2) {
		return 1;
	}
	return 0;
}
#define rvs32(n) ((((n)&0xff)<<24)|(((n)&0xff00)<<8)|(((n)&0xff0000)>>8)|(((n)&0xff000000)>>24))
#define rvs16(n) ((((n)&0xff)<<8)|((n)&0xff00)>>8)
/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;
		
		Elf32_Phdr *phdr = NULL;

		u_char *ptr_ph_table = NULL;
        Elf32_Half ph_entry_count;
        Elf32_Half ph_entry_size;

        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
		
		if (is_MSB(binary)) {
			ptr_ph_table = binary + rvs32(ehdr->e_phoff);
            ph_entry_count = rvs16(ehdr->e_phnum);
            ph_entry_size = rvs16(ehdr->e_phentsize);
            // for each section header, output section number and section addr. 
            // hint: section number starts at 0.
            Elf32_Half i = 0;
            for (; i < ph_entry_count; i++) {
                phdr = (Elf32_Phdr *)(ptr_ph_table + i * ph_entry_size);
                printf("%d: %d %d\n", i, rvs32(phdr->p_filesz), rvs32(phdr->p_memsz));
            }
		}
		else {
			// get section table addr, section header number and section header size.
			ptr_sh_table = binary + ehdr->e_shoff;
			sh_entry_count = ehdr->e_shnum;
			sh_entry_size = ehdr->e_shentsize;
			// for each section header, output section number and section addr. 
			// hint: section number starts at 0.
			Elf32_Half i = 0;
			for (; i < sh_entry_count; i++) {
			    shdr = (Elf32_Shdr *)(ptr_sh_table + i * sh_entry_size);
			    printf("%d:0x%x\n", i, shdr->sh_addr);
			}
		}
		// printf("e_type: %d\n", ehdr->e_type);
        return 0;
}

