package rs.aidev.breakout;

import java.awt.Color;
import java.awt.Graphics;

import rs.aidev.jengine.Game;
import rs.aidev.jengine.Position;
import rs.aidev.jengine.Scene;
import rs.aidev.jengine.Sprite;

public class Ball extends Sprite{
	public static final int defaultWidth = 20;
	public static final int defaultHeight = 20;
	public static final Color defaultColor = Color.GREEN;
	private Color color = defaultColor;
	public static final double defaultSpeed = 3;
	
	private double vy=-defaultSpeed;
	private double vx=defaultSpeed*Math.random();
	
	Ball(Scene scene,double x,double y){
		this(scene,x,y,defaultWidth,defaultHeight);
	}
	Ball(Scene scene, double x, double y,int width,int height) {
		super(scene, x, y,width,height);
		
		//Add collision body at ball edges
		this.addCollisionBody(width/2, 0, 2, 2);
		this.addCollisionBody(width/2, height, 2, 2);
		this.addCollisionBody(0, height/2, 2, 2);
		this.addCollisionBody(width, height/2, 2, 2);
		
		this.addCollison("Brick", (self,collider,position)->{
			//Find out which side of brich was hit
			//bottom and up wall
			Game.score.add();
			collider.triggerCollision(self);
			System.out.println("hit @ pos: "+position.name());
			if (position==Position.LEFT || position==Position.RIGHT) {
				 vx=-vx;
			}else if(position==Position.TOP || position==Position.BOTTOM) {
				vy=-vy;
			}
		},true);
		this.addCollison("Player", (self,collider,position)->{
			if(position==Position.TOP) {
				vy=-vy;
				vx=(Math.random()-0.5)*0.1+vx;
				update();
			}
		},true);
		this.addCollison("WorldBounds", (self,collider,position)->{
			if (position==Position.LEFT || position==Position.RIGHT)
		        vx=-vx;
			else if(position==Position.TOP)
		    	vy=-vy;
			else if (position==Position.BOTTOM)
				self.enabled=false;
		}, false);
	}	
	@Override
	public void paint(Graphics g) {
		g.setColor(color);
		g.fillOval((int)x, (int)y, width, height);
		g.setColor(Color.BLACK);
		collisionBodies.forEach((body)->{
			g.fillRect((int) (body.x+x),(int) (body.y+y), body.width, body.height);
		});
	}

	@Override
	public void update() {	
		x+=vx;
		y+=vy;
	}

	@Override
	public String toString() {
		return "Ball";
	}
}
