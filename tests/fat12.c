#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define PACKED __attribute__((packed))

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef uchar bool;

typedef struct FAT12Header
{
    uchar BS_JMP[3];        // BIOS JMP instruction
    uchar BS_OEMName[8];    // OEM字符串，必须为8个字符，不足以空格填空
    ushort BPB_BytsPerSec;  // 每扇区字节数
    uchar BPB_SecPerClus;   // 每簇占用的扇区数
    ushort BPB_RsvdSecCnt;  // Boot占用的扇区数
    uchar BPB_NumFATs;      // FAT表的记录数
    ushort BPB_RootEntCnt;  // 最大根目录文件数
    ushort BPB_TotSec16;    // 每个FAT占用扇区数
    uchar BPB_Media;        // 媒体描述符
    ushort BPB_FATSz16;     // 每个FAT占用扇区数
    ushort BPB_SecPerTrk;   // 每个磁道扇区数
    ushort BPB_NumHeads;    // 磁头数
    uint BPB_HiddSec;       // 隐藏扇区数
    uint BPB_TotSec32;      // 如果BPB_TotSec16是0，则在这里记录
    uchar BS_DrvNum;        // 中断13的驱动器号
    uchar BS_Reserved1;     // 未使用
    uchar BS_BootSig;       // 扩展引导标志
    uint BS_VolID;          // 卷序列号
    char BS_VolLab[11];     // 卷标，必须是11个字符，不足以空格填充
    char BS_FileSysType[8]; // 文件系统类型，必须是8个字符，不足填充空格
} PACKED FAT12Header;

#define FAT12_ATTR_HIDDEN 0x27
#define FAT12_ATTR_DIRECTORY 0x10
#define FAT12_ATTR_FILE 0x20

typedef struct FAT12RootEntry
{
    char DIR_Name[11];
    uchar DIR_Attr;
    uchar reserve[10];
    ushort DIR_WrtTime;
    ushort DIR_WrtDate;
    ushort DIR_FstClus;
    uint DIR_FileSize;
} PACKED FAT12RootEntry;

typedef union FAT12Cluster
{
    uchar data[2];
} PACKED FAT12Cluster;

bool compare(char *first, char *second, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (first[i] != second[i])
            return false;
    }
    return true;
}

uint get_next_cluster(FILE *stream, uint index, FAT12Cluster *clus, FAT12Header *header)
{
    uint start = 0x200;
    uint offset = (uint)(index * 1.5);

    fseek(stream, start + offset, 0);
    fread(clus, 1, 2, stream);
    uint next = 0;
    if (index % 2 == 1)
    {
        next += clus->data[1] * 0x10;
        next += clus->data[0] >> 4;
    }
    else
    {
        next += clus->data[0];
        next += clus->data[1] & 0x0f;
    }
    return next;
}

int main(int argc, char const *argv[])
{
    printf("Starting read fat12 image file...\n");
    FILE *stream = NULL;

    if ((stream = fopen("../onix/build/onix.img", "rb")) == NULL)
    {
        printf("Cannot open file\n");
        return 0;
    }
    FAT12Header header;
    FAT12RootEntry entry;
    FAT12Cluster clus;

    char *filename = "LOADER  BIN";
    uchar *content = NULL;
    uchar *fat = NULL;
    ushort *fats = NULL;

    fseek(stream, 0, SEEK_SET);
    fread(&header, 1, sizeof(header), stream);

    printf("OEMName: %s \n", header.BS_OEMName);
    printf("FileSystem: %s \n", header.BS_FileSysType);

    bool found = false;
    for (size_t i = 0; i < header.BPB_RootEntCnt; i++)
    {
        int where = 19 * header.BPB_BytsPerSec + i * sizeof(FAT12RootEntry);
        fseek(stream, where, 0);
        fread(&entry, 1, sizeof(entry), stream);
        if (compare(entry.DIR_Name, filename, sizeof(filename)))
        {
            printf("loader.bin find....\n");
            found = true;
            break;
        }
    }
    if (!found)
        goto finish;

    content = malloc(entry.DIR_FileSize + 512); // for remain block

    uchar buffer[512] = {0};
    int i = 0;
    int j = entry.DIR_FstClus;
    int count = 0;

    while (true)
    {
        printf("Read cluster : %d \n", j);
        fseek(stream, header.BPB_BytsPerSec * (33 + j - 2), 0);
        size_t size = fread(buffer, 1, sizeof(buffer), stream);
        memcpy(content + count, buffer, size);
        count += size;
        j = get_next_cluster(stream, j, &clus, &header);
        if (j >= 0xff7)
            break;
    }

    FILE *out = fopen("./build/test.bin", "wb");
    fwrite(content, 1, entry.DIR_FileSize, out);
    fclose(out);

finish:
    if (content != NULL)
        free(content);
    if (stream != NULL)
        fclose(stream);
    return 0;
}
