package rs.aidev.breakout;
import rs.aidev.jengine.Game;
import rs.aidev.jengine.Level;

public class Breakout{	
	public static void main(String[] args) {
		Game game = new Game(300,400);
		
		game.addLevel("Main",new Level(game) {
			@Override
			public void init() {
				final int gap = 7;
				int x=gap;
				int y=gap;
				for(int i=0;i<18;i++) {
					if(x+Brick.defaultWidth>game.width) {
						x=gap;
						y+=Brick.defaultHeight+gap;
					}
					game.scene.add(new Brick(game.scene,x,y));
					x+=Brick.defaultWidth+gap;
				}
				game.scene.add(new Player(game.scene,game.width/2-40,game.height-70));
				game.scene.add(new Ball(game.scene,game.width/2-Ball.defaultWidth/2,game.height/2-Ball.defaultHeight/2));
				
				game.addScore();
			}
		});
		game.setLevel("Main");
		game.start();
	}
}
