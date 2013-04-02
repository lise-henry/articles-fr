public class Four extends Thread {
	private int temperature;
	private int temperatureDesiree;
	public Four (int temperatureDesiree) {
		temperature = 17;
		this.temperatureDesiree = temperatureDesiree;
	}
	public void run () {
		prechauffer (temperatureDesiree);
	}
	/* La methode prechauffer, elle, ne change pas */
  /* (...) */
}