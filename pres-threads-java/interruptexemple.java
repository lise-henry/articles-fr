import java.io.*;
class Boucle extends Thread {
	public void run () {
		for (int i = 0; !this.isInterrupted (); i++)
		{
			try {
				Thread.sleep (1000);
				System.out.println (i);
			} catch (InterruptedException e) {
				return;
			}
		}
	}}
public class InterruptTest	{
	public static void main(String[] args) throws IOException {
		Boucle b = new Boucle ();
		b.start ();
		BufferedReader reader = new BufferedReader (new InputStreamReader (System.in));
		reader.readLine ();
		b.interrupt ();
}}

