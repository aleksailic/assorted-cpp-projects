package rs.aidev.jengine;
import java.awt.Graphics;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;

class Dimension{
	public double x;
	public double y;
	public Dimension(double x,double y) {
		this.x=x;
		this.y=y;
	}
}
class Size{
	public int width;
	public int height;
	public Size(int width,int height) {
		this.width=width;
		this.height=height;
	}
}

abstract public class Sprite {
	protected double x;
	protected double y;
	protected Dimension anchor;
	protected int width;
	protected int height;
	protected Scene scene;
	public boolean enabled=true;
	protected Map<String,Callback> activeCollisions;
	protected Map<String,Callback> collisions;
	protected ArrayList<CollisionBody> collisionBodies;
	
	public Sprite() {}
	
	public Sprite(Scene scene,double x,double y,int width, int height){
		this.scene=scene;
		this.x=x;
		this.y=y;
		this.width=width;
		this.height=height;
		this.anchor=new Dimension(0,0); //default anchor
		this.collisions=new HashMap<String,Callback>();
		this.activeCollisions=new HashMap<String,Callback>();
		this.collisionBodies=new ArrayList<CollisionBody>();
	}
	public abstract void paint(Graphics g);
	public void addCollison(String spriteType,Callback cb,boolean activeCollision) {
		collisions.put(spriteType,cb);
		if(activeCollision)
			activeCollisions.put(spriteType, cb);
	}
	public void addCollisionBody(double x,double y,int width,int height) {
		collisionBodies.add(new CollisionBody(this, x, y, width, height));
	}
	public void triggerCollision(Sprite collider) {
		collisions.forEach((type,cb)->{
			if(type.equals(collider.toString()))
				cb.call(this, collider);
		});
	}
	public void triggerCollision(Sprite collider,Position p) {
		collisions.forEach((type,cb)->{
			if(type.equals(collider.toString()))
				cb.call(this, collider,p);
		});
	}
	public abstract void update();
	public abstract String toString();
}
