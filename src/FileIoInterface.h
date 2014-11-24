#ifndef __FILE_IO_INTERFACE_H_
#define __FILE_IO_INTERFACE_H_

//struct CardInformation;
typedef struct FileIoInterface {

    FILE *file;
    CardInformation cardInformation;

    //char* (*readFile)(struct FileIoInterface *self, char* path);
    void (*readCard)(struct FileIoInterface *self, char* path, CardInformation *cardInformation);
    void (*writeCard)(struct FileIoInterface *self, const CardInformation *cardInformation, char* path);
    int (*getDailyInfoSize)(struct FileIoInterface *self, char* path);


} FileIoInterface;

FileIoInterface* newFileIoInterface();
//char* readFile(FileIoInterface *self, char* path);
void readCard(FileIoInterface *self, char* path, CardInformation *cardInformation);
void writeCard(struct FileIoInterface *self, const CardInformation *cardInformation, char* path);
int getDailyInfoSize(FileIoInterface *self, char* path);

#endif // __FILE_IO_INTERFACE_H_