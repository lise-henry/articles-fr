public class MaClasse {
	public void uneFonction () {
		/* Pas de verouillage de cette portion du code */
		synchronized (this) {
			/* Verouillage de cette portion */
		}
		/* Pas de verrouillage de cette portion */
	}
	public void uneAutreFonction () {
		/* Pas de verrouillage */
		synchronized (this) {
			/* Verrouillage */
		}
		/* Pas de verrouillage */
	}
}
