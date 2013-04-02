public class MaClass extends Thread
{
  void run ()
  {
    /* Du code */
  }

  void uneAutreMethode ()
  {
    /* Encore du code */
  }

  public static void main (String[] args)
  {
    MaClasse objet = new MaClasse ();
    objet.start ();
    objet.uneAutreMethode ();
    /* uneAutreMethode () et objet.run () s'executent en parallele */
  }
}