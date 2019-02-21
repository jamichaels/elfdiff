#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>
#include "eat.h"
#include "Ptype.h"

int not_elf();

int main(int argc, char **argv[]) {

    Elf64_Ehdr ehdr1;
	Elf64_Ehdr ehdr2;
    Elf64_Shdr shdr1;
	Elf64_Shdr shdr2;
	Elf64_Phdr phdr1;
    Elf64_Phdr phdr2;

	Elf64_Shdr sectHdr1;
	Elf64_Shdr sectHdr2;
	char* Sectnames1 = NULL;
	char* Sectnames2 = NULL;
	char* name1 = "";
	char* name2 = "";
	int idx;

	char ptype1[30];
	char ptype2[30];

    int i;
    int fd;
	int pcount;
	int scount;

    FILE* fp1;
	FILE* fp2;

	char *r = "\033[0;31m";
	char *g = "\033[0;32m";
	char *b = "\033[0;34m";
	char *e = "\033[0m";
    
	
	if(argc < 3) {
        printf("%susage: %s <ELF file1> <ELF file2>\n",g,(char *)argv[0]);
        exit(-1);
    }


    // Open the files and read the ELF headers

     	fp1 = fopen((char *)argv[1], "r");
    	fread(&ehdr1,sizeof(Elf64_Ehdr),1,fp1);
	    if( is_elf(ehdr1) == -1) { not_elf(); } // Check for valid ELF header
 	

     	fp2 = fopen((char *)argv[2], "r");
    	fread(&ehdr2,sizeof(Elf64_Ehdr),1,fp2);
	    if( is_elf(ehdr2) == -1) { not_elf(); } // Check for valid ELF header



    printf("\n\nELF headers diff for:\n\n%s\t\t%s\n\n",(char *)argv[1],(char *)argv[2]);

	printf("%s-= ELF Header =-%s\n\n",g,e);

	if(ehdr1.e_ident[4] != ehdr2.e_ident[4]) {
    	printf(" File Class:\n");
    	if(ehdr1.e_ident[4] == ELFCLASSNONE) { printf("%s invalid class\t\t",r);}
		if(ehdr2.e_ident[4] == ELFCLASSNONE) { printf("%s invalid class\n",e);}

    	if(ehdr1.e_ident[4] == ELFCLASS32) { printf("%s 32-bit onject\t\t",r);}
    	if(ehdr2.e_ident[4] == ELFCLASS32) { printf("%s 32-bit onject\n",e);}

    	if(ehdr1.e_ident[4] == ELFCLASS64) { printf("%s 64-bit object\t\t",r);}
    	if(ehdr2.e_ident[4] == ELFCLASS64) { printf("%s 64-bit object\n",e);}
	}

  //  printf("\n Data Encoding: ");
  //  if(ehdr.e_ident[5] == ELFDATANONE) { printf("Invalid data encoding\n");}
  //  if(ehdr.e_ident[5] == ELFDATALSB) { printf("Little endian (LSB)\n");}
  //  if(ehdr.e_ident[5] == ELFDATA2MSB) { printf("Big endian (MSB)\n"); }

	if(ehdr1.e_ident[6] != ehdr2.e_ident[6]) {
    	printf("  File Version\n%x\t\t%s%x%s\n", ehdr1.e_ident[6],r,ehdr2.e_ident[6],e);
	}
   
	if(ehdr1.e_type != ehdr2.e_type) {
 
    printf("Object File Type:\n");
	printf("%s",b);
    switch(ehdr1.e_type) {
        case ET_NONE:
            printf(" No file type\t\t"); break;
        case ET_REL:
            printf(" Relocatable file\t\t"); break;  
        case ET_EXEC:
            printf(" Executable file\t\t"); break;
        case ET_DYN:
            printf(" Shared Object File\t\t"); break; 
        case ET_CORE:
            printf(" Core file\t\t"); break; 
        default:
            printf(" Shouldn't get here..\t\t");
    }
        	
	printf("%s",r);

    switch(ehdr2.e_type) {
		case ET_NONE:
            printf(" No file type\n"); break;
        case ET_REL:
            printf(" Relocatable file\n"); break;  
        case ET_EXEC:
            printf(" Executable file\n"); break;
        case ET_DYN:
            printf(" Shared Object File\n"); break; 
        case ET_CORE:
            printf(" Core file\n"); break; 
        default:
            printf(" Shouldn't get here..\n");
		
    }
	
	printf("%s",e);
}    

	// edit
	//if(ehdr1.e_machine != ehdr1.e_machine) {
    	printf("\nRequired Architecture:\n");
    	printf(" %s%d\t\t%s%d\n",r,ehdr1.e_machine,b,ehdr2.e_machine);
	//}

	if(ehdr1.e_version != ehdr2.e_version) {
	printf("Object File Version: ");
		printf("%s",r);
		if(ehdr1.e_version == EV_NONE){printf(" Invalid version\n");}
		if(ehdr1.e_version == EV_CURRENT){printf(" Current version\n");}
		else { printf(" Unknown version\n"); }
		printf("%s",e);			

		if(ehdr2.e_version == EV_NONE){printf(" Invalid version\n");}
		if(ehdr2.e_version == EV_CURRENT){printf(" Current version\n");}
		else { printf(" Unknown version\n"); }

	}

	if(ehdr1.e_entry != ehdr2.e_entry) {
    	printf("%sVirtual Entry Address:\n %s%li\t\t%s%li%s\n",e,r,ehdr1.e_entry,b,ehdr2.e_entry,e);
	}
	
	if(ehdr1.e_phoff != ehdr2.e_phoff) {
    	printf("%sProgram Header Offset:\n %s%li\t\t%s%li%s\n",e,r,ehdr1.e_phoff,b,ehdr2.e_phoff,e);
	}

	if(ehdr1.e_shoff != ehdr2.e_shoff) {
    	printf("%sSection Header Offset:\n %s%li\t\t%s%li%s\n",e,r,ehdr1.e_shoff,b,ehdr2.e_shoff,e);
	}

	if(ehdr1.e_flags != ehdr2.e_flags) {
		printf("%sProcessor Flags:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_flags,b,ehdr2.e_flags,e);
	}

	if(ehdr1.e_ehsize != ehdr2.e_ehsize) {
    	printf("%sELF Header Size:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_ehsize,b,ehdr2.e_ehsize,e);
	}

	if(ehdr1.e_phentsize != ehdr2.e_phentsize) {
	    printf("%sProgram Header Entry Size:\n %s%x\t\t\%s%x%s\n",e,r,ehdr1.e_phentsize,b,ehdr2.e_phentsize,e);
	}

	if(ehdr1.e_phnum != ehdr2.e_phnum) {
    	printf("%sNumber of Program Header Entries:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_phnum,b,ehdr2.e_phnum,e);
	}

	if(ehdr1.e_shentsize != ehdr2.e_shentsize) {
    	printf("%sSection Header Entry Size:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_shentsize,b,ehdr2.e_shentsize,e);
	}

	if(ehdr1.e_shnum != ehdr2.e_shnum) {
    	printf("%sNumber of Section Header Entries:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_shnum,b,ehdr2.e_shnum,e);
	}

	if(ehdr1.e_shstrndx != ehdr2.e_shstrndx) {
    	printf("%sIndex of string table in Section Header Table:\n %s%x\t\t%s%x%s\n",e,r,ehdr1.e_shstrndx,b,ehdr2.e_shstrndx,e);
	}


// Program Header Tables

	printf("\n%s-= Program Header Table =-\n\n",g);

	if(ehdr1.e_phnum != ehdr2.e_phnum) {
		printf("%s **Different number of Program Header Tables!**%s\n",r,e);
		printf("%s %s: contains %d tables		%s%s contains %d tables%s\n",b,(char *)argv[1],ehdr1.e_phnum,g,(char *)argv[2],ehdr2.e_phnum,e);
		printf("%s Only printing difference in the amount of pheaders from the file containing the lesser number\n\n",r);
	}


	pcount = ehdr1.e_phnum <= ehdr2.e_phnum ? ehdr1.e_phnum : ehdr2.e_phnum;

    for(i=0;i<pcount;i++) {

			fread(&phdr1,sizeof(Elf64_Phdr),1,fp1);
			fread(&phdr2,sizeof(Elf64_Phdr),1,fp2);

            getptype((long)phdr1.p_type,ptype1);
			getptype((long)phdr2.p_type,ptype2);


            printf(" %sEntry: %s%d\n",e,r,i);
            //broken for some reason. Probably programmer error.
			//printf(" %sSegment type     : %s%s0x%x\t\t\t%s%s0x%x\n",e,r,ptype1,phdr1.p_type,b,ptype2,phdr2.p_type);
            printf(" %sSegment flags    : %s%x\t\t\t%s%x\n",e,r,phdr1.p_flags,b,phdr2.p_flags);
            printf(" %sFile offset      : %s%li\t\t\t%s%li\n",e,r,phdr1.p_offset,b,phdr2.p_offset);
            printf(" %sSegment vaddr    : %s%li\t\t\t%s%li\n",e,r,phdr1.p_vaddr,b,phdr2.p_vaddr);
            printf(" %sSegment paddr    : %s%li\t\t\t%s%li\n",e,r,phdr1.p_paddr,b,phdr1.p_paddr);
            printf(" %sSize in file     : %s%li\t\t\t%s%li\n",e,r,phdr1.p_filesz,b,phdr2.p_filesz);
            printf(" %sSize in memory   : %s%li\t\t\t%s%li\n",e,r,phdr1.p_memsz,b,phdr2.p_memsz);
            printf(" %sSegment alignment: %s%li\t\t\t%s%li\n",e,r,phdr1.p_align,b,phdr2.p_align);

            printf("\n");


    }


    printf("\n%s-= Section Header Table =-\n\n",g);




    fseek(fp1,ehdr1.e_shoff + ehdr1.e_shstrndx * sizeof(sectHdr1),SEEK_SET);
    fseek(fp2,ehdr2.e_shoff + ehdr2.e_shstrndx * sizeof(sectHdr2),SEEK_SET);
    fread(&sectHdr1,sizeof(sectHdr1),1,fp1);
    fread(&sectHdr2,sizeof(sectHdr2),1,fp2);

    Sectnames1 = malloc(sectHdr1.sh_size);
    Sectnames2 = malloc(sectHdr2.sh_size);
    fseek(fp1,sectHdr1.sh_offset,SEEK_SET);
    fseek(fp2,sectHdr2.sh_offset,SEEK_SET);
    fread(Sectnames1,sectHdr1.sh_size,1,fp1);
    fread(Sectnames2,sectHdr2.sh_size,1,fp2);

  //  lseek(fp,ehdr.e_shoff,SEEK_SET);


    if(ehdr1.e_shnum != ehdr2.e_shnum) {
        printf("%s **Different number of Section Header Tables!**%s\n",r,e);
        printf("%s %s: contains %d tables       %s%s contains %d tables%s\n",b,(char *)argv[1],ehdr1.e_shnum,g,(char *)argv[2],ehdr2.e_shnum,e);
        printf("%s Only printing difference in the amount of sheaders from the file containing the lesser number\n\n",r);
    }



	scount = ehdr1.e_shnum <= ehdr2.e_shnum ? ehdr1.e_shnum : ehdr2.e_shnum;

    for(i=0;i<scount;i++)
    {
        name1 = "";
		name2 = "";

        fseek(fp1, ehdr1.e_shoff + i * sizeof(sectHdr1), SEEK_SET);
        fseek(fp2, ehdr2.e_shoff + i * sizeof(sectHdr2), SEEK_SET);

        fread(&shdr1,sizeof(shdr1),1,fp1);
        if(shdr1.sh_name) {
            name1 = Sectnames1 + shdr1.sh_name;
        }
        else {
            name1 = " ";
        }


        fread(&shdr2,sizeof(shdr2),1,fp2);
        if(shdr2.sh_name) {
            name2 = Sectnames2 + shdr2.sh_name;
        }
        else {
            name2 = " ";
        }


        printf("\n%sSection Name: %s%s\t%s%s\n",e,r,name1,b,name2);
        printf("%sSection Type: %s%d\t%s%d\n",e,r,shdr1.sh_type,b,shdr2.sh_type);
        printf("%sSection Flags: %s%lx\t%s%lx\n",e,r,shdr1.sh_flags,b,shdr2.sh_flags);
        printf("%sSection Vaddr: %s%lx\t%s%lx\n",e,r,shdr1.sh_addr,b,shdr2.sh_addr);
        printf("%sSection File Offset: %s%lx\t%s%lx\n",e,r,shdr1.sh_offset,b,shdr2.sh_offset);
        printf("%sSection Size in Bytes: %s%lx\t%s%lx\n",e,r,shdr1.sh_size,b,shdr2.sh_size);
        printf("%sLink to Another Section: %s%x\t%s%x\n",e,r,shdr1.sh_link,b,shdr2.sh_link);
        printf("%sExtra Section Info: %s%x\t%s%x\n",e,r,shdr1.sh_info,b,shdr2.sh_info);
        printf("%sSection Alignment: %s%lx\t%s%lx\n",e,r,shdr1.sh_addralign,b,shdr2.sh_addralign);
        printf("%sEntry size if section holds table: %s%lx\t%s%lx\n",e,r,shdr1.sh_entsize,b,shdr2.sh_entsize);
    }












exit(1);

}

int not_elf()
{
    printf("\nThis file is not a valid elf object.\n");
    exit(-1);
}
