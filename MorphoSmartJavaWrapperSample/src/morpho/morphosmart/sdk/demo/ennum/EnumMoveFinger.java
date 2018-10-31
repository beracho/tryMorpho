package morpho.morphosmart.sdk.demo.ennum;

public enum EnumMoveFinger {
	MOVE_UP(0),
	MOVE_RIGHT(1),
	MOVE_DOWN(2),
	MOVE_LEFT(3);

	private int move;
	
	private EnumMoveFinger(int move) {
		this.move = move;
	}

	public int getValue() {
		return move;
	}

}
