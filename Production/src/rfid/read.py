import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522


def read_card():
    reader = SimpleMFRC522()
    GPIO.setwarnings(False)
    try:
        id, text = reader.read()
        print(id)
    finally:
        GPIO.cleanup()

 

# Appel de la fonction si le script est exécuté directement
if __name__ == "__main__": 
    read_card()

    # 323905732182 tag bleu 1
    # 1009860165294 tag bleu 2
    # 29416637637 carte blanche 1