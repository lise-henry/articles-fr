public static void main (String[] args) throws InterruptedException
{
	Four four = new Four (180);
	four.start ();
	preparerIngredients ();
	four.join ();
	System.out.println ("On met au four");
}