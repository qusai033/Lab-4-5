#ifndef RC522_H
#define RC522_H

#include <stdint.h>

void initRC522();
bool readTag(uint8_t *tag);
bool authenticateTag(const uint8_t *tag, const uint8_t *key);
void haltTag();

#endif
