
#include "PN532_HSU.h"
#include "PN532_debug.h"


PN532_HSU::PN532_HSU(HardwareSerial &serial)
{
    _serial = &serial;
    command = 0;
}

void PN532_HSU::begin()
{
    _serial->begin(115200);
}

void PN532_HSU::wakeup()
{
    _serial->write(0x55);
    _serial->write(0x40);
    _serial->write(0);
    _serial->write(0);
    _serial->write(0);

#ifdef DEBUG
    /** dump serial buffer */
    if(_serial->available()){
        DMSG("Dump serial buffer: ");

        do {
            DMSG_HEX(_serial->read());
        } while(_serial->available());
    }
#endif
}

int8_t PN532_HSU::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
#ifdef DEBUG
    /** dump serial buffer */
    if(_serial->available()){
        DMSG("Dump serial buffer: ");

        do {
            DMSG_HEX(_serial->read());
        } while(_serial->available());
    }
#endif

    command = header[0];
    {
    	uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
        uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

        DMSG("\nWrite: ");
        for (uint8_t i = 0; i < hlen; i++) {
            sum += header[i];

            DMSG_HEX(header[i]);
        }
        for (uint8_t i = 0; i < blen; i++) {
            sum += body[i];

            DMSG_HEX(body[i]);
        }

		_serial->write(PN532_PREAMBLE);
		_serial->write(PN532_STARTCODE1);
		_serial->write(PN532_STARTCODE2);
		_serial->write(length);
		_serial->write(~length + 1);         // checksum of length
		_serial->write(PN532_HOSTTOPN532);
	    _serial->write(header, hlen);
	    _serial->write(body, blen);
	    _serial->write(~sum + 1);
	    _serial->write(PN532_POSTAMBLE);
    }
    return readAckFrame();
}

int16_t PN532_HSU::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint8_t tmp[3];
    
    DMSG("\nRead:  ");
    
    /** Frame Preamble and Start Code */
    if(receive(tmp, 3, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if(0 != tmp[0] || 0!= tmp[1] || 0xFF != tmp[2]){
        DMSG("Preamble error");

        return PN532_INVALID_FRAME;
    }
    /** receive length and check */
    uint8_t length[2];

    if(receive(length, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(length[0] + length[1]) ){
        DMSG("Length error");
        return PN532_INVALID_FRAME;
    }
    length[0] -= 2;

    if( length[0] > len){
        return PN532_NO_SPACE;
    }
    /** receive command byte */
    uint8_t cmd = command + 1;               // response command

    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( PN532_PN532TOHOST != tmp[0] || cmd != tmp[1]){
        DMSG("Command error");
        return PN532_INVALID_FRAME;
    }
    if(receive(buf, length[0], timeout) != length[0]){
        return PN532_TIMEOUT;
    }
    uint8_t sum = PN532_PN532TOHOST + cmd;

    for(uint8_t i=0; i<length[0]; i++){
        sum += buf[i];
    }
    /** checksum and postamble */
    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(sum + tmp[0]) || 0 != tmp[1] ){
        DMSG("Checksum error");
        return PN532_INVALID_FRAME;
    }
    return length[0];
}

int8_t PN532_HSU::readAckFrame()
{
	const uint8_t PN532_ACK[] = { 0, 0, 0xFF, 0, 0xFF, 0 };
    uint8_t ackBuf[_countof(PN532_ACK)];
    
    DMSG("\nAck: ");
    
    if(receive(ackBuf, _countof(PN532_ACK), PN532_ACK_WAIT_TIME) < 0){
        DMSG("Timeout\n");

        return PN532_TIMEOUT;
    }
    if(!!memcmp(ackBuf, PN532_ACK, _countof(PN532_ACK))){
        DMSG("Invalid\n");

        return PN532_INVALID_ACK;
    }
    return 0;
}

/**
    @brief receive data .
    @param buf --> return value buffer.
           len --> length expect to receive.
           timeout --> time of reveiving
    @retval number of received bytes, 0 means no data received.
*/
int8_t PN532_HSU::receive(uint8_t *buf, int len, uint16_t timeout)
{
  int read_value, read_bytes = 0;
  unsigned long start_millis = 0;

  while (read_bytes < len) {
	start_millis = millis();

	do {
		read_value = _serial->read();

		if (read_value >= 0) {
			break;
		}
	} while (timeout == 0 || (millis() - start_millis) < timeout);

	if (read_value < 0) {
		return !!read_bytes ? read_bytes : PN532_TIMEOUT;
	}
	buf[read_bytes++] = (uint8_t)read_value;
  }
#ifdef DEBUG
  {
	  int i = 0;

	  for (; i < read_bytes; ++i) {
		  DMSG_HEX(buf[i]);
	  }
	  if (!!i) {
		  DMSG("\n");
	  }
  }
#endif
  return read_bytes;
}
