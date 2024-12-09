# **TP1-EnseaSH**

## **Description**
Ce projet consiste à développer un shell minimaliste en C, nommé **enseash**. Il permet d'exécuter des commandes simples, affiche des informations sur l'exécution (temps, statut de sortie, signaux), et gère les erreurs courantes telles que les commandes inexistantes ou les index hors limites.

---

## **Fonctionnalités**
1. **Interface utilisateur** :
   - Affiche un message de bienvenue à l'ouverture.
   - Propose un prompt personnalisable : `enseash % `.
   - Permet la saisie de commandes via `stdin`.

2. **Exécution de commandes** :
   - Les commandes sont exécutées à l'aide d'un processus enfant (fork + exec).
   - Le parent attend la fin de l'exécution et affiche :
     - Le code de sortie (`exit:0`) si la commande réussit.
     - Le signal (`sign:11`) si une erreur (segmentation fault, etc.) survient.

3. **Gestion des erreurs** :
   - Affiche un message d'erreur si la commande est introuvable.
   - Gère la fin du shell via `Ctrl+D` ou la commande `exit`.
   - Simule des erreurs comme des `segmentation faults` pour tester la robustesse du système.

4. **Mesure de performance** :
   - Affiche le temps d'exécution de chaque commande en millisecondes.

---

## **Exemple d'Utilisation**

### **Démarrage du shell**
```bash
$ ./enseash
Welcome to the ENSEA Shell.
enseash % 
```

### **Exécution d'une commande**
```bash
enseash % ls
file1.txt  file2.txt
[exit:0|5ms]
enseash % 
```

### **Commande inconnue**
```bash
enseash % unknowncmd
Error: Command not found.
enseash % 
```

### **Fin du shell**
```bash
enseash % exit
Goodbye!
```

---

## **Structure du code**

### **Fichiers principaux**
- `main.c` : Contient la logique principale du shell.
- `README.md` : Documentation du projet.

### **Points clés**
1. **Gestion des processus** :
   - Utilisation de `fork()` pour créer un processus enfant.
   - Utilisation de `execlp()` pour exécuter les commandes.

2. **Gestion des signaux** :
   - Interruption par `Ctrl+D` ou commande `exit`.
   - Simulation de segmentation faults via des erreurs intentionnelles.

3. **Mesure du temps** :
   - Utilisation de `clock_gettime()` pour mesurer la durée d'exécution.

---

## **Installation**

1. Clonez ce dépôt :
   ```bash
   git clone https://github.com/username/enseash.git
   cd enseash
   ```

2. Compilez le projet avec `gcc` :
   ```bash
   gcc -o enseash main.c
   ```

3. Lancez le shell :
   ```bash
   ./enseash
   ```

---

## **Tests et Vérification**

### **Commandes valides**
Vérifiez que des commandes comme `ls`, `pwd`, ou `date` fonctionnent correctement.

### **Erreurs simulées**
- Commande inconnue : Affiche un message d'erreur.
- Simulation de `SIGSEGV` : Provoquez un accès mémoire illégal via un code de test.

### **Sorties**
Testez la gestion des codes de sortie et des signaux.

---

## **Auteur**
* AFIF Ikram
* HAKKI Houssam 

---

## **Améliorations possibles**
- Support des commandes complexes avec arguments multiples.
- Ajout de la gestion des redirections et des pipes.
- Amélioration de la gestion des erreurs.

---

Adaptez ce texte à vos besoins et remplacez "username" et "Votre Nom" par les informations appropriées. 😊
