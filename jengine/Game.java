package rs.aidev.jengine;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.HashMap;
import java.util.Map;

@SuppressWarnings("serial")
public class Game extends Frame{
	public final int width;
	public final int height;

	
	public static Score score;
	public Scene scene;
	Map<String,Level> levels;
	
	public Game(int width, int height) {
		super("Breakout");
		
		this.width=width;
		this.height=height;
	
		super.setSize(width,height);
		super.setResizable(false);
		
		super.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				stop();
			}
		});
		
		scene=new Scene(width,height);
		
		//Put screen on the frame
		super.add(scene, BorderLayout.CENTER);	
				
		this.levels=new HashMap<String,Level>();
	}
	
	public void setLevel(String identifier) throws IllegalArgumentException{
		Level level = levels.get(identifier);
		if(level==null)
			throw new IllegalArgumentException("Wrong argument passed");
		scene.stop();
		scene.clean();
		level.init();
	}
	
	public void addLevel(String identifier,Level level) {
		levels.put(identifier, level);
	}
	
	public void addScore() {
		score = new Score(scene,width-30,height-50,100,50);
		scene.add(score);
	}
	
	public void stop() {
		scene.stop();
		dispose();
	}
	
	public void start() {
		super.setVisible(true);
		scene.start();
	}
}
