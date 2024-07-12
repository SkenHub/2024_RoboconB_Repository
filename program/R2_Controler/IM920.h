#ifndef IM920_H_
#define IM920_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>

enum IM920Bytes{
	Bytes8 = 1,
	Bytes16,
	Bytes24,
	Bytes32,
};

union ConvertIntFloat{
  long int int_val;
  float float_val;
  uint8_t uint8_val[4];
};

class IM920{
  SoftwareSerial& serial_;
	int rx_size_,data_size_,i_;
	uint16_t *rx_id_;
	uint8_t *return_data_;
	uint8_t rx_data_[108];
	uint8_t get_data_[108];
  const char TXDA_[6] = "TXDA ";
  const char TXDU_[6] = "TXDU ";
	char TxChar_[16];
	char TxData_[16];
  const char finish_char_[2] = {0xA,0xD};
public:
  IM920(SoftwareSerial & serial);
  void init();
  bool read(uint16_t* rx_id,uint8_t* rx_data,IM920Bytes bytes);
	void write(uint8_t* tx_data,IM920Bytes bytes,int node_num = 0x0000);
	void DebugData(uint8_t* data,int data_size);
};

#endif /* IM920_H_ */