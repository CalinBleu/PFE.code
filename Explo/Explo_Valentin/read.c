#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <gpiod.h>
#include "mfrc.h"
#include "read.h"


#define RSTPIN        25 
#define SS_PIN        8
#define MISO_PIN      9
#define MOSI_PIN      10
#define SCK_PIN       11

#define SPI_DEVICE    "/dev/spidev0.0"
#define SPI_SPEED     100000 

#define OUTPUT        0
#define INPUT         1
#define LOW           0
#define HIGH          1

int spi_fd;
uint32_t scratch32;
struct spi_ioc_transfer transfer;
const char *chipname = "gpiochip0";


void configurePinState(int pin, const char *chipname, int state) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int rv;

    // Ouvrir le chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("gpiod_chip_open_by_name");
        return;
    }

    // Obtenenir la ligne correspondant à la broche spécifiée
    line = gpiod_chip_get_line(chip, pin);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return;
    }

    // Configurer la broche en sortie si nécessaire
    if (state == OUTPUT) {
        rv = gpiod_line_request_output(line, "example", 0);
        if (rv < 0) {
            perror("gpiod_line_request_output");
            gpiod_line_release(line);
            gpiod_chip_close(chip);
            return;
        }
    } else{
      rv = gpiod_line_request_input(line, "example");
        if (rv < 0) {
            perror("gpiod_line_request_input");
            gpiod_line_release(line);
            gpiod_chip_close(chip);
            return;
        }
    }

    // Libéreration les ressources
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

void configurePinValue(int pin, const char *chipname, int value) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int rv;

    // Ouvrir le chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("gpiod_chip_open_by_name");
        return;
    }

    // Obtenenir la ligne correspondant à la broche spécifiée
    line = gpiod_chip_get_line(chip, pin);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return;
    }

/*
    // Mettre la broche à l'état spécifié
    rv = gpiod_line_set_value(line, value);
    if (rv < 0) {
        perror("gpiod_line_set_value");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return;
    }
*/
    // Libérer les ressources
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}


int initSPI() {
    spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("Erreur lors de l'ouverture de spidev");
        return -1;
    }


  
    if(ioctl(spi_fd, SPI_IOC_RD_MODE32, &scratch32) !=0){
        printf("Could not read SPI mode...\r\n");
        close(spi_fd);
        return -1;
    }

    scratch32 |= SPI_MODE_0;

    // Configurer le mode SPI
    if(ioctl(spi_fd, SPI_IOC_WR_MODE32, &scratch32) != 0) {
        perror("Erreur lors de la configuration du mode SPI");
        close(spi_fd);
        return -1;
    }

    /*
    // Configurer le mode SPI
    uint8_t mode = SPI_MODE_0;
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Erreur lors de la configuration du mode SPI");
        close(spi_fd);
        return -1;
    }*/

    // Configurer la vitesse de l'horloge SPI
    scratch32 = SPI_SPEED;
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &scratch32) < 0) {
        perror("Erreur lors de la configuration de la vitesse SPI");
        close(spi_fd);
        return -1;
    }

    configurePinState(RSTPIN, chipname, OUTPUT);
    configurePinValue(RSTPIN, chipname, LOW);
    

    
    //bcm2835_gpio_fsel(RPI_GPIO_P1_26, BCM2835_GPIO_FSEL_ALT0); /* CE1 */ PIN 7
    //configurePinState(SS_PIN, chipname , OUTPUT); /* CE0 */  !!!!!!!!!!!!!! gpiod_line_request_output: Device or resource busy !!!!!!!!!!!!!!!!!!
    configurePinState(MISO_PIN, chipname, OUTPUT); /* MISO */
    configurePinState(MOSI_PIN, chipname, OUTPUT); /* MOSI */
    configurePinState(SCK_PIN, chipname, OUTPUT); /* CLK */
    
    transfer.tx_buf = 0;  // Adresse virtuelle du tampon de transmission (FIFO TX)
    transfer.rx_buf = 0;  // Adresse virtuelle du tampon de réception (FIFO RX)
    transfer.len = 0;     // Longueur des données à transférer


    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer) == -1) {
        perror("Erreur lors du transfert SPI");
        return -1;
    }

    return 0;
}


void init_rc522(){
  struct gpiod_chip *chip;
  struct gpiod_line *rstpin_line;

  // Ouvrir le chip GPIO
  chip = gpiod_chip_open_by_name(chipname);
  if (!chip) {
      perror("gpiod_chip_open_by_name");
      return;
  }

  // Obtenir la ligne GPIO pour RSTPIN
  rstpin_line = gpiod_chip_get_line(chip, RSTPIN);
  if (!rstpin_line) {
      perror("gpiod_chip_get_line");
      gpiod_chip_close(chip);
      return;
  }

 /* // Lire l'état de la broche                         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! gpiod_line_get_value: Operation not permitted !!!!!!!!!!!!!!!!!
  int rstpin_state = gpiod_line_get_value(rstpin_line);
  if (rstpin_state < 0) {
      perror("gpiod_line_get_value");
      gpiod_line_release(rstpin_line);
      gpiod_chip_close(chip);
      return;
  } */

  // Effectuer l'action en fonction de l'état lu
  /*if (rstpin_state == LOW) {                       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MET EN ERREUR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // La puce MFRC522 est en mode veille.
      // Mettre la broche RSTPIN à HIGH pour sortir du mode veille (réinitialisation matérielle).
      if (gpiod_line_set_value(rstpin_line, HIGH) < 0) {
          perror("gpiod_line_set_value");
          gpiod_line_release(rstpin_line);
          gpiod_chip_close(chip);
          return;
        }
      // Attendre 50 ms (conformément à la documentation)
      usleep(50000);
  } else {
      // Effectuer une réinitialisation logicielle
     PCD_Reset();
  }*/

  // Libéreration les ressources
  gpiod_line_release(rstpin_line);
  gpiod_chip_close(chip);

}
int spi_transfern( uint8_t* tbuf, uint8_t* rbuf, int len) {
    
    transfer.tx_buf = (unsigned long)tbuf;
    transfer.rx_buf = (unsigned long)rbuf;
    transfer.len = len;
    transfer.delay_usecs = 0;
    transfer.speed_hz = SPI_SPEED;  
    transfer.bits_per_word = 8;


    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer) == -1) {
        perror("Erreur lors du transfert SPI");
        return -1;
    }
}


int spi_transfer(uint8_t* tx_buf, uint8_t* rx_buf, int len) {

    // Configureration de la structure spi_ioc_transfer
    transfer.tx_buf = (unsigned long)tx_buf;
    transfer.rx_buf = (unsigned long)rx_buf;
    transfer.bits_per_word = 0;
    transfer.speed_hz = SPI_SPEED; 
    transfer.delay_usecs = 0;
    transfer.len = 1;

    // Effectuer le transfert SPI
    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer) != 0) {
        perror("Erreur lors du transfert SPI");
        return -1;
    }
    return 0;
    
}
/*
int spi_transfer(uint8_t* tx_buffer, uint8_t* rx_buffer, int len) {
    struct spi_ioc_transfer transfer = {
        .delay_usecs = 0,
        .bits_per_word = 8,
    };
    transfer.tx_buf = (unsigned long)tx_buffer;
    transfer.rx_buf = (unsigned long)rx_buffer;
    transfer.len = len;

    return ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer);
}*/

int main() {
  MFRC522 mfrc;
  initSPI();
  init_rc522();
  PCD_Init(&mfrc);

  while (1) {
    // Look for a card
    if (!PICC_IsNewCardPresent(&mfrc))
      continue;

    if (!PICC_ReadCardSerial(&mfrc))
      continue;

    // Print UID
    for (uint8_t i = 0; i < mfrc.Uid.size; ++i) {
      if (mfrc.Uid.uidByte[i] < 0x10) {
        printf(" 0%X", mfrc.Uid.uidByte[i]);
      } else {
        printf(" %X", mfrc.Uid.uidByte[i]);
      }
    }
    printf("\n");
    sleep(1);
  }
  close(spi_fd);
  return 0;
}
