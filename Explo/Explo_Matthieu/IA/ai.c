#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t pid; // Variable pour stocker le PID du processus fils


void AI_init(void)
{

}

int AI_startRecognition(char *picturePath) {
    int status;

    pid = fork(); // Créer un nouveau processus

    if (pid < 0) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Nous sommes dans le processus fils
        char *scriptPath = "/home/pi/Documents/PFE.code/Explo/Explo_Matthieu/IA/face_reco.py";

        execlp("python3", "python3", scriptPath, picturePath); // Exécuter le script Python
        perror("Erreur lors de l'exécution du script Python");
        exit(EXIT_FAILURE);

    } else {
        // Nous sommes dans le processus parent
        printf("Le script Python est en cours d'exécution (PID : %d).\n", pid);

        // Attendre une entrée de l'utilisateur pour terminer le script
        printf("Appuyez sur Entrée pour arrêter le script Python...\n");
        getchar();

        // Interrompre le processus Python
        AI_stopRecognition();

       // Attendre la fin du processus fils et récupérer la valeur de retour
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Le script Python s'est terminé avec le code de sortie : %d\n", exit_status);
        } else {
            printf("Le script Python ne s'est pas terminé correctement.\n");
        }
    }
    exit(EXIT_SUCCESS);
}

void AI_stopRecognition(void){
    if (pid > 0) {
        kill(pid, SIGKILL); // Interrompre le processus fils
    }
}


