#include <Arduino.h>
#include <stdio.h>

enum IM920Bytes{
	Bytes8 = 1,
	Bytes16,
	Bytes24,
	Bytes32,
};

struct IM920COM{
  const char TXDA[6] = "TXDA ";
  const char TXDU[6] = "TXDU ";
  const char finish[2] = {0xA,0xD};
};

union ConvertIntFloat{
  long int int_val;
  float float_val;
  uint8_t uint8_val[4];
};

String IM920Format(uint8_t data){
  char str_data[2];
  sprintf(str_data,"%02X",data);
  return str_data;
}

