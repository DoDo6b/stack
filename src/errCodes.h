#ifndef ERRCODES_H
#define ERRCODES_H


typedef enum
{
    RECIVED_NULL                = 1,
    MAIN_SIGNES_CORRUPTED       = 2,
    NOT_INITILIZED              = 4,
    DATABLOCK_SIGNES_CORRUPTED  = 8,
    TOPPTR_OUTOFBOUNDS          = 16,
    CRC_HASCHANGED              = 32,
}EC;


#endif