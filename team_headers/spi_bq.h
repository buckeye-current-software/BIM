
#ifndef SPI_IF_H
#define SPI_IF_H

#define CS_DELAY			10

#define SPI_BAUDRATE       1000000 //Specify baudrate in Hz
#define SPI_BAUDRATE_REG   (unsigned short)(MCLK_FREQ/SPI_BAUDRATE)

//*SPI interface defines to BQ Pack*/
#define NUMBER_OF_SPI_REGS        BQ_SPI_REG_MAX

#define READ_ACCESS               (1<<0)
#define WRITE_ACCESS              (1<<1)

/**
* @brief  System typdefs        .                     
*/
typedef enum BQ_REGS
{
  SPI_DEV_ID,
  SPI_REV,
  SPI_STATUS
} spi_regs_t;

/**
* @brief  External functions declaration         .                     
*/
short spi_read_reg(unsigned char dev_addr,
                   unsigned char reg_addr,
                   short elem_num,
                   unsigned char discard_crc,/*True, False*/
                   unsigned char* pData);

short spi_write_reg(unsigned char dev_addr, 
                    unsigned char reg_addr, 
                    unsigned char data);

#endif

/*EOF*/
