package rs.aidev.jengine;

public enum Position {
	LEFT(0),
	RIGHT(1),
	TOP(2),
	BOTTOM(4),
	UNDEFINED(8);
	
	public final int value;
	Position(int value){this.value=value;}
	
}
