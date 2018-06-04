package rs.aidev.breakout;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import rs.aidev.jengine.Scene;
import rs.aidev.jengine.Sprite;

public class Player extends Sprite{
	public static final int defaultWidth = 80;
	public static final int defaultHeight = 20;
	public static final int defaultOffset = 3;
	public static final Color defaultColor = Color.BLUE;
	private Color color = defaultColor;
	
	private int xoff=0;
	
	Player(Scene scene,double x,double y){
		this(scene,x,y,defaultWidth,defaultHeight);
	}
	Player(Scene scene, double x, double y,int width,int height) {
		super(scene, x, y,width,height);
		scene.addKeyListener(new KeyAdapter() {
			public void keyPressed(KeyEvent e) {
				switch(e.getKeyCode()) {
				case KeyEvent.VK_LEFT:
					xoff= (xoff==-defaultOffset ? -defaultOffset : xoff-defaultOffset);
					break;
				case KeyEvent.VK_RIGHT:
					xoff=(xoff==defaultOffset ? defaultOffset : xoff+defaultOffset);
					break;
				}
			}
			public void keyReleased(KeyEvent e) {
				switch(e.getKeyCode()) {
				case KeyEvent.VK_LEFT:
					xoff+=defaultOffset;
					break;
				case KeyEvent.VK_RIGHT:
					xoff-=defaultOffset;
					break;
				}
			}
		});
	}	

	@Override
	public void paint(Graphics g) {
		g.setColor(color);
		g.fillRect((int)x, (int)y, width, height);
	}

	@Override
	public void update() {	
		x+=xoff;
	}

	@Override
	public String toString() {
		return "Player";
	}

}
