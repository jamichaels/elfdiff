int is_elf(Elf64_Ehdr eh) {

if( eh.e_ident[0] != ELFMAG0 || eh.e_ident[1] != ELFMAG1 ||
    eh.e_ident[2] != ELFMAG2 || eh.e_ident[3] != ELFMAG3 )
    {
        return -1;
    }
else { return 1; }
}
