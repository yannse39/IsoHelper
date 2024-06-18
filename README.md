Explorateur de Système de Fichiers ISO 9660

Un projet permettant de naviguer et d'explorer les systèmes de fichiers ISO 9660.

Installation

Dans le répertoire racine, exécutez la commande make pour compiler le projet.

Utilisation

Le programme se compose de deux fichiers exécutables : iso9660 et main.

Exécutez le binaire en tant que explorateur de système de fichiers ISO 9660. Ce programme permet d'explorer et d'interagir avec une image ISO 9660.

Options du iso_helper :

    -f, --file : Spécifie le fichier ISO à explorer;
    ./main -f ../tests/example.iso

Commandes Interactives

Une fois le programme lancé, les commandes suivantes sont disponibles pour interagir avec le système de fichiers ISO 9660 :

    help : Affiche l'aide et les commandes disponibles.
    info : Affiche des informations sur le volume ISO.
    ls : Liste le contenu du répertoire actuel.
    cat <nom_fichier> : Affiche le contenu d'un fichier.
    quit : Quitte le programme.
    cd <nom_répertoire> : Change le répertoire actuel.
    pwd : Affiche le chemin du répertoire actuel.
    get <nom_fichier> : Récupère un fichier du système ISO.
Une fois dans le programme interactif, vous pouvez utiliser les commandes listées ci-dessus pour naviguer et explorer l'image ISO.

License

Ce projet est sous licence GNU.

Project by https://github.com/yannse39 aka yannse.

Note: Assurez-vous d'avoir les droits d'exécution sur les fichiers avant de les utiliser.