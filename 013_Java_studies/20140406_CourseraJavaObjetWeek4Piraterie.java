/*******************************************
 * Completez le programme a partir d'ici.
 *******************************************/
/*******************************************
 * Ne pas modifier apres cette ligne
 * pour pr'eserver les fonctionnalit'es et
 * le jeu de test fourni.
 * Votre programme sera test'e avec d'autres
 * donn'ees.
 *******************************************/
class Piraterie {

    static public final int MAX_X = 500;
    static public final int MAX_Y = 500;
    static public final double RAYON_RENCONTRE = 10;

    static public void main(String[] args) {
        // Test de la partie 1
        System.out.println("***Test de la partie 1***");
        System.out.println();
        // un bateau pirate 0,0 avec le drapeau 1 et avec dommages
        Navire ship1 = new Pirate(0, 0, 1, true);
        // un bateau marchand en 25,0 avec le drapeau 2
        Navire ship2 = new Marchand(25, 0, 2);
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println("Distance: " + ship1.distance(ship2));
        System.out.println("Quelques deplacements horizontaux et verticaux");
        // se deplace de 75 unites a droite et 100 en haut
        ship1.avance(75, 100);
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println("Un deplacement en bas:");
        ship1.avance(0, -5);
        System.out.println(ship1);
        ship1.coule();
        ship2.coule();
        System.out.println("Apres destruction:");
        System.out.println(ship1);
        System.out.println(ship2);

        // Test de la partie 2
        System.out.println();
        System.out.println("***Test de la partie 2***");
        System.out.println();

        // deux vaisseaux sont enemis s'ils ont des drapeaux differents

        System.out.println("Bateau pirate et marchand ennemis (trop loins):");
        // bateau pirate intact
        ship1 = new Pirate(0, 0, 1, false);
        ship2 = new Marchand(0, 25, 2);
        System.out.println(ship1);
        System.out.println(ship2);
        ship1.rencontre(ship2);
        System.out.println("Apres la rencontre:");
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println();

        System.out.println("Bateau pirate et marchand ennemis (proches):");
        // bateau pirate intact
        ship1 = new Pirate(0, 0, 1, false);
        ship2 = new Marchand(2, 0, 2);
        System.out.println(ship1);
        System.out.println(ship2);
        ship1.rencontre(ship2);
        System.out.println("Apres la rencontre:");
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println();

        System.out.println("Bateau pirate et marchand amis (proches):");
        // bateau pirate intact
        ship1 = new Pirate(0, 0, 1, false);
        ship2 = new Marchand(2, 0, 1);
        System.out.println(ship1);
        System.out.println(ship2);
        ship1.rencontre(ship2);
        System.out.println("Apres la rencontre:");
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println();

        System.out.println("Deux bateaux pirates ennemis intacts (proches):");
        // bateaux pirates intacts
        ship1 = new Pirate(0, 0, 1, false);
        ship2 = new Pirate(2, 0, 2, false);
        System.out.println(ship1);
        System.out.println(ship2);
        ship1.rencontre(ship2);
        System.out.println("Apres la rencontre:");
        System.out.println(ship1);
        System.out.println(ship2);
        System.out.println();

        System.out.println("Un bateau pirate intact et un avec dommages, ennemis:");
        // bateau pirate intact
        Navire ship3 = new Pirate(0, 2, 3, false);
        System.out.println(ship1);
        System.out.println(ship3);
        ship3.rencontre(ship1);
        System.out.println("Apres la rencontre:");
        System.out.println(ship1);
        System.out.println(ship3);
        System.out.println();

        System.out.println("Deux bateaux pirates ennemis avec dommages:");
        System.out.println(ship2);
        System.out.println(ship3);
        ship3.rencontre(ship2);
        System.out.println("Apres la rencontre:");
        System.out.println(ship2);
        System.out.println(ship3);
        System.out.println();
    }
}
