#include "new.h"

uint8_t* endian(uint32_t init)
{
uint8_t *paddr;
uint8_t *ptr;

paddr = (uint8_t *)&init;
return paddr;
}


int main()
{
uint8_t *addr;
uint32_t init = 0x12345678;
addr = endian(init);

tipeData->byte1 = *addr;
tipeData->byte2 = *addr++;
tipeData->byte3 = *addr++;
tipeData->byte4 = *addr++;


printf("data :%x",init);
printf("/n");
printf("--memory mapping-- /n");
printf("{%x,",tipeData->byte1);
printf("%x,",tipeData->byte2);
printf("%x,",tipeData->byte3);
printf("%x",tipeData->byte4);

}

