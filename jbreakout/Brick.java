package rs.aidev.breakout;

import java.awt.Color;
import java.awt.Graphics;

import rs.aidev.jengine.Scene;
import rs.aidev.jengine.Sprite;

public class Brick extends Sprite{
	public static final int defaultWidth = 40;
	public static final int defaultHeight = 20;
	public static final Color defaultColor = Color.RED;
	
	private Color color = defaultColor;
	
	Brick(Scene scene,double x,double y){
		this(scene,x,y,defaultWidth,defaultHeight);
	}
	Brick(Scene scene, double x, double y,int width,int height) {
		super(scene, x, y,width,height);
		this.addCollison("Ball",(self,collider,position)->{
			if(color.equals(Color.GRAY))
				enabled=false;
			color=Color.GRAY;
		},false);
	}	
	@Override
	public void paint(Graphics g) {
		g.setColor(color);
		g.fillRect((int)x, (int)y, width, height);
	}
	@Override
	public void update() {
		
	}
	@Override
	public String toString() {
		return "Brick";
	}

}
