/* Dans la classe Recette */
public static void attendre (int duree) {
	try {
			Thread.sleep (1000 * duree);
	} catch (Exception e){}
}
	
private static void preparerIngredients () {
	System.out.println ("On ajoute le beurre");
	attendre (2);
	System.out.println ("On ajoute le sucre");
	attendre (2);
	/* Etc. */
}
