package rs.aidev.jengine;

public class CollisionBody {
	public Sprite mySprite;
	//Ideally should be polygon, but that would take too much time
	public int width;
	public int height;
	public double x;
	public double y;
	
	//sprite's whole body is collision body
	public CollisionBody(Sprite sprite) {
		this.mySprite=sprite;
		this.width=sprite.width;
		this.height=sprite.height;
		this.x=sprite.anchor.x;
		this.y=sprite.anchor.y;
	}
	
	//specify x and y offset from sprite's anchor, and of course its height
	public CollisionBody(Sprite sprite,double x, double y, int width, int height) {
		this.mySprite=sprite;
		this.width=width;
		this.height=height;
		this.x=x;
		this.y=y;
	}
}
