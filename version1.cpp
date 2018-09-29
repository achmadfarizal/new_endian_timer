#include "new.h"
#define byteSatu  0
#define byteDua   1
#define byteTiga  2
#define byteEmpat 3


uint8_t* endian(uint32_t init,uint8_t i_byte)
{
uint8_t *paddr;
uint8_t *ptr;
uint8_t array[4];
int i,cnt;

paddr = (uint8_t *)&init;

cnt = sizeof(init)/sizeof(uint8_t);
  for(i=0; i<cnt ; i++)
  {
    array[i]=paddr;
    paddr++;
  }


  switch(i_byte){
    case byteSatu:
    ptr = array[0];

    case byteDua:
    ptr = array[1];

    case byteTiga:
    ptr = array[2];

    case byteEmpat:
    ptr = array[3];

  }

 return ptr;
}


int main()
{
uint32_t init = 0x12345678;

tipeData->byte1 = endian(init,byteSatu);
tipeData->byte2 = endian(init,byteDua);
tipeData->byte3 = endian(init,byteTiga);
tipeData->byte4 = endian(init,byteEmpat);


printf("data :%x",init);
printf("/n");
printf("--memory mapping-- /n");
printf("{%x,",tipeData->byte1);
printf("%x,",tipeData->byte2);
printf("%x,",tipeData->byte3);
printf("%x",tipeData->byte4);

}

