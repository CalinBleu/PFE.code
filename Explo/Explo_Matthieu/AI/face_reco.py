import face_recognition
import cv2
from imutils.video import VideoStream
import time
import threading
import sys
from enum import Enum
import os

class AuthResult(Enum):
    USER_UNKNOWN = 0
    ALLOWED = 1

#print the current process id
print (os.getpid())

# Charger l'image statique à comparer (à partir d'un fichier)
image_path = str(sys.argv[1])
image_to_compare = face_recognition.load_image_file(image_path)
image_to_compare_encoding = face_recognition.face_encodings(image_to_compare)[0]

# Initialiser la capture vidéo (ici, à partir de la webcam)
vs = VideoStream(usePiCamera=True).start()
time.sleep(2.0)

# Dictionnaire partagé pour stocker les informations partagées entre les threads
shared_data = {
    "frame": None,
    "face_locations": None,
    "running": True
}

def detect_faces(shared_data):
    resultRecognition = AuthResult.USER_UNKNOWN.value
    while shared_data["running"]:
        frame = shared_data["frame"]
        face_locations = face_recognition.face_locations(frame) if frame is not None else None
        shared_data["face_locations"] = face_locations

        if face_locations:
            # Extraire les encodages faciaux du flux vidéo
            face_encodings = face_recognition.face_encodings(frame, face_locations)

            # Comparer les encodages faciaux de l'image à comparer avec ceux du flux vidéo
            for face_encoding in face_encodings:
                # Comparer l'encodage du visage dans le flux vidéo avec l'encodage de l'image à comparer
                match = face_recognition.compare_faces([image_to_compare_encoding], face_encoding)
                if match[0]:
                    shared_data["running"] = False
                    print(AuthResult.ALLOWED.value)

def display_video(shared_data):
    while shared_data["running"]:
        frame = vs.read()
        shared_data["frame"] = frame.copy()  # Mettre à jour le dictionnaire partagé avec la nouvelle frame

        face_locations = shared_data["face_locations"]

        if face_locations:
            # Dessiner des rectangles autour des visages détectés
            for (top, right, bottom, left) in face_locations:
                cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)

        cv2.imshow("Video", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            shared_data["running"] = False  # Mettre à jour le dictionnaire partagé pour arrêter les threads
            break

# Création des threads
detect_thread = threading.Thread(target=detect_faces, args=(shared_data,))
display_thread = threading.Thread(target=display_video, args=(shared_data,))

# Démarrer les threads
detect_thread.start()
display_thread.start()

# Attendre la fin des threads
detect_thread.join()
display_thread.join()

# Libérer les ressources et fermer les fenêtres
vs.stop()
cv2.destroyAllWindows()
