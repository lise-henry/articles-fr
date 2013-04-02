public class MaClass implements Runnable
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
    Thread thread = new Thread (objet);
    thread.start ();
    objet.uneAutreMethode ();
    /* uneAutreMethode () et thread.run () s'executent en parallele */
  }
}