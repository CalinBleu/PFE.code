

void configurePinState(int pin, const char *chipname, int state);

void configurePinValue(int pin, const char *chipname, int value);

int spi_begin(void);

void setSPIConfig();

int initSPI();

void init_rc522();

int spi_transfern(uint8_t* tx_buf, uint8_t* rx_buf, int len);

int spi_transfer(uint8_t* tx_buffer, uint8_t* rx_buffer, int len);