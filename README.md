# **TP1-EnseaSH**

## **Description**
Ce projet consiste à développer un shell minimaliste en C, nommé **enseash**. Il permet d'exécuter des commandes simples, affiche des informations sur l'exécution (temps, statut de sortie, signaux), et gère les erreurs courantes telles que les commandes inexistantes ou les index hors limites.

---

## 📝 **Fonctionnalités**

### ✅ **1. Affichage d'un message d'accueil et d'un prompt initial**

Lors du lancement du shell, vous verrez :

```bash
$ ./enseash
Bienvenue dans le Shell ENSEA.
enseash %
```

- Le shell affiche un message de bienvenue.
- Ensuite, il affiche un prompt simple `enseash %` pour saisir vos commandes.

---

### ✅ **2. Exécution des commandes simples**

Le shell doit exécuter une commande saisie sans arguments ou avec des arguments.

#### **a) Commandes simples sans argument**

Exemple :

```bash
enseash % fortune
You will be awarded some great honor.
enseash %
```

---

### ✅ **3. Gestion de la sortie du shell avec `exit` ou `Ctrl+D`**

- En tapant **`exit`**, vous quittez le shell proprement.
  
```bash
enseash % exit
Goodbye!
```

- Si vous appuyez sur **`Ctrl+D`**, cela envoie un EOF et ferme le shell.

---

### ✅ **4. Affichage des codes de retour ou des signaux**

Chaque commande exécutée doit afficher le code de sortie ou les signaux :

- Les commandes qui se terminent correctement afficheront `[exit:0]`.
- Les commandes qui échouent avec des signaux afficheront `[sign:<n>]`.

Exemple :

```bash
Welcome to the ENSEA Shell.
enseash % ls
' documents'   ENSEASH6     ENSEASH7.c	    ' output.txt'
 ENSEASH5      ENSEASH6.c  ' filelist.txt'   test_memory_error
 ENSEASH5.c    ENSEASH7    ' listfile.tx'    test_memory_error.c
[exit:0|3ms]
enseash % 
```

---

### ✅ **5. Mesure du temps d'exécution des commandes**

Chaque commande doit être chronométrée en millisecondes à l'aide de l'appel système `clock_gettime`.

Exemple :

```bash
Welcome to the ENSEA Shell.
enseash % ls
' documents'   ENSEASH6     ENSEASH7.c	    ' output.txt'
 ENSEASH5      ENSEASH6.c  ' filelist.txt'   test_memory_error
 ENSEASH5.c    ENSEASH7    ' listfile.tx'    test_memory_error.c
[exit:0|3ms]
enseash % 
```

---

### ✅ **6. Commandes complexes avec arguments**

Le shell doit gérer des commandes avec des arguments complexes.

Exemple :

```bash
Welcome to the ENSEA Shell.
enseash % fortune -s
Give your very best today.  Heaven knows it's little enough.
[exit:0|6ms]
enseash % 

```

---

### ✅ **7. Gestion des redirections `stdin` et `stdout`**

Le shell doit gérer les redirections avec les opérateurs `<` et `>`.

#### Exemple :

```bash
Welcome to the ENSEA Shell.
enseash %  ls > filelist.txt
[exit:0|3ms]
enseash % ls < filelist.txt             
' documents'   ENSEASH6     ENSEASH7.c	    ' output.txt'
 ENSEASH5      ENSEASH6.c  ' filelist.txt'   test_memory_error
 ENSEASH5.c    ENSEASH7    ' listfile.tx'    test_memory_error.c
[exit:0|2ms]
enseash % 
```

- `>` redirige la sortie standard vers un fichier.
- `<` redirige l'entrée standard depuis un fichier.

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
Vérifiez que des commandes comme `ls`, `pwd`, ou `date` fonctionnent correctement:
Welcome to the ENSEA Shell.
enseash % ls
' documents'   ENSEASH6     ENSEASH7.c	    ' output.txt'
 ENSEASH5      ENSEASH6.c  ' filelist.txt'   test_memory_error
 ENSEASH5.c    ENSEASH7    ' listfile.tx'    test_memory_error.c
[exit:0|3ms]
enseash % 

Welcome to the ENSEA Shell.
enseash % pwd
/home/ensea/Documents/TP1 
[exit:0|2ms]
enseash % 

Welcome to the ENSEA Shell.
enseash % date
lundi 9 décembre 2024, 17:46:29 (UTC+0100)
[exit:0|4ms]
enseash % 


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
- Ajout de la gestion des redirections et des pipes.
- Amélioration de la gestion des erreurs.

---

