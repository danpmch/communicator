
#ifndef __SD_H__
#define __SD_H__

void setup_sd();
int read_bytes(const char * path, uint8_t* buffer, uint max_bytes);

#endif