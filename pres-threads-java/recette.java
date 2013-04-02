public class Recette {
	
	private static void preparerIngredients ()
	{
    (...)
	}

	public static void main (String[] args)
	{
		Four four = new Four ();
		four.prechauffer (180);
		preparerIngredients ();
		System.out.println ("On met au four");
	}
}