package rs.aidev.jengine;

public interface Callback {
	default public void call(Sprite sender,Sprite reciever) {
		call(sender,reciever,Position.UNDEFINED);
	}
	public void call(Sprite sender,Sprite receiver,Position p);
}
