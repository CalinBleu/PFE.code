import face_recognition
import sys
from enum import Enum
import os
import time
import signal

class AuthResult(Enum):
    ALLOWED = 4
    FACE_UNKNOWN = 5

#print the current process id
print(os.getpid())

resultRecognition = AuthResult.FACE_UNKNOWN.value

def signal_handler(sig, frame):
    print(resultRecognition)
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

if len(sys.argv) < 3:
    print("Usage: python face_reco.py <image_to_compare> <frame>")
    sys.exit()

# Charger l'image statique à comparer (à partir d'un fichier)
image_path = str(sys.argv[1])
image_to_compare = face_recognition.load_image_file(image_path)
image_to_compare_encoding = face_recognition.face_encodings(image_to_compare)[0]


frame_path = str(sys.argv[2])

frame = face_recognition.load_image_file(frame_path)
face_locations = face_recognition.face_locations(frame) if frame is not None else None

if face_locations:
    # Extraire les encodages faciaux du flux vidéo
    face_encodings = face_recognition.face_encodings(frame, face_locations)

# Comparer les encodages faciaux de l'image à comparer avec ceux du flux vidéo
for face_encoding in face_encodings:
    # Comparer l'encodage du visage dans le flux vidéo avec l'encodage de l'image à comparer
    match = face_recognition.compare_faces([image_to_compare_encoding], face_encoding)
    if match[0]:
        resultRecognition = AuthResult.ALLOWED.value


print(resultRecognition)

