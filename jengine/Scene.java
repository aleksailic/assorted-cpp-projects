package rs.aidev.jengine;

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Image;
import java.util.ArrayList;

@SuppressWarnings("serial")
public class Scene extends Canvas implements Runnable{
	public static final int defaultRefreshRate=20; //50fps
	private Sprite worldBounds;
	
	private Thread myThread;
	int refreshRate;
	private ArrayList<Sprite> sprites;
	
	private int width;
	private int height;
	
	public int getWidth() {return width;}
	public int getHeight() {return height;}
	
	//for double buffering as screen flicks by default
	private Graphics bufferGraphics; 
	private Image offscreen;
	
	public Scene(int width,int height){
		this.width=width;
		this.height=height;
		
		this.refreshRate=defaultRefreshRate;
		this.sprites=new ArrayList<Sprite>();
		this.myThread=new Thread(this);
		
		super.setSize(width,height);
		
		//Anonymous inner class used here as this only serves as a dummy sprite for collision detection
		worldBounds = new Sprite(this,0,0,width,height) {
			public void paint(Graphics g) {}
			public void update() {}
			public String toString() {
				return "WorldBounds";
			}
		};	
	}
	public Scene(int width,int height,int refreshRate){
		this(width,height);
		this.refreshRate=refreshRate;
	}
	public void paint(Graphics g) {
		//Double buffering stuff
		bufferGraphics.clearRect(0, 0, width, height);
		sprites.forEach((iter)->{
			if(iter.enabled)
				iter.paint(bufferGraphics);
		});
		g.drawImage(offscreen, 0, 0, this);
	}
	public void add(Sprite s) {
		sprites.add(s);
	}
	
	public void start() {
		offscreen=createImage(width, height);
		bufferGraphics=offscreen.getGraphics();
		myThread.start();
	}
	
	public void update() {
		/* This is on average less than O(n^2) which
		 * is not that great, can do better
		 * but for this example will do fine
		 */
		sprites.forEach((sprite)->{
			if(!sprite.enabled) return; //if sprite is disabled no need to check collision
			sprite.update(); //change needs to happen in order to check !
			//check if collideWorldBounds is set and check collision if it is
			if(sprite.collisions.containsKey(worldBounds.toString())) {
				if (sprite.x >=(this.width-sprite.width))
					sprite.triggerCollision(worldBounds,Position.RIGHT);
				else if(sprite.x<=0)
					sprite.triggerCollision(worldBounds,Position.LEFT);
				else if(sprite.y<=0)
					sprite.triggerCollision(worldBounds,Position.TOP);
				else if(sprite.y>=(this.height-sprite.height))
					sprite.triggerCollision(worldBounds,Position.BOTTOM);	
			}
			sprite.activeCollisions.forEach((String spriteType,Callback cb)->{
				sprites.forEach((collider)->{
					if(spriteType.equals(collider.toString()) && collider.enabled){	
						//If no collision body present, add whole sprite... (should not be here but for now ok)
						if(sprite.collisionBodies.isEmpty())
							sprite.collisionBodies.add(new CollisionBody(sprite));
						sprite.collisionBodies.forEach((body)->{
							double x=sprite.x+body.x;
							double y=sprite.y+body.y;
							
							//check collision parameters here (collisionBody collides with collider?)
							Position p = Position.UNDEFINED;
							if( (x >= collider.x && sprite.x <= (collider.x+collider.width)) || ((x+body.width) >= collider.x && (x+body.width)<=(collider.x+collider.width))) {
								if( ((y >= collider.y) && y <= (collider.y+collider.height)) || ((y+body.height) >= collider.y && (y+body.height) <= (collider.y+collider.height))) {
									//definitely colliding, but what edges does it hit?
									if((collider.x >= x ) && collider.x <= (x+body.width))
										p=Position.LEFT;
									else if(((collider.x+collider.width) >= x) && ((collider.x+collider.width) <= (x+body.width)))
										p=Position.RIGHT;
									else if(collider.y>=y && collider.y<=y+body.height)
										p=Position.TOP;
									else if(collider.y+collider.height >= y  && collider.y+collider.height <=y+body.height)
										p=Position.BOTTOM;
									cb.call(sprite,collider,p);
								}
							}
						});
					}
				});
			});
		});
	}
	//For double buffering, needed by canvas not to be confused with upper fun
	public void update(Graphics g) {
		paint(g);
	}
	
	//Our game loop implemented with java's threads
	@Override
	public void run() {
		long timestamp;
		long elapsedTime;
		try {
			while(!Thread.interrupted()) {
				timestamp=System.currentTimeMillis();
				update();
				repaint();
				elapsedTime=System.currentTimeMillis()-timestamp;
				System.out.println(elapsedTime);
				if(elapsedTime<refreshRate) {
					Thread.sleep(refreshRate-elapsedTime);
				}
			}
		}catch(InterruptedException ie) {
			System.out.println("Goodbye!");
		}
		
	}
	
	//Externally force stop the thread
	public void stop() {
		myThread.interrupt();
	}
	
	//When switching levels, we need to cleanup all the sprites
	public void clean() {
		sprites.clear();
	}
}
