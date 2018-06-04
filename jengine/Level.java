package rs.aidev.jengine;

abstract public class Level {
	protected Game game;
	public Level(Game game) {
		this.game=game;
	}
	abstract public void init();
}
