#include <elf.h>

int getptype(long ptype, char *pstring) {

        switch(ptype) {
            case 1:
                pstring = "Loadable Program Segment";
                break;
            case 2:
                pstring = "Dynamic Linking Information";
                break;
            case 3:
                pstring = "Program interpreter";
                break;
            case 4:
                pstring="Auxillary information";
                break;
            case 5:
                pstring="Reserved";
                break;
            case 6:
                pstring="Entry for header table itself";
                break;
            case 7:
                pstring="Thread-local storage segment";
                break;
            case 8:
                pstring="Number of defined types";
                break;
            case 0x60000000:
                pstring="Start of OS-specific";
                break;
            case 0x6474e550:
                pstring="GCC .eh_frame_hdr segment"; 
                break;
            case 0x6474e551:
                pstring="Indicates stack executability";
                break;
            case 0x647e552:
                pstring="Read-only after relocation";
                break;
            case 0x6ffffffa:
                pstring="Sun Specific segment";
                break;
            case 0x6ffffffb:
                pstring="Stack Segment";
                break;
            case 0x6fffffff:
                pstring="End of OS-specific";
                break;
            case 0x70000000:
                pstring="Start of processor-specific";
                break;
            case 0x7fffffff:
                pstring="End of processor-specific";
            default:
                pstring="Other";

        
        }
}
