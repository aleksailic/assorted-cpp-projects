package rs.aidev.jengine;

import java.awt.Color;
import java.awt.Graphics;

public class Score extends Sprite{
	private int score=0;
	public void add() {
		score++;
	}
	public void add(int num) {
		score+=num;
	}
	public Score(Scene scene, double x, double y, int width, int height) {
		super(scene, x, y, width, height);
	}

	@Override
	public void paint(Graphics g) {
		g.setColor(Color.BLACK);
		g.drawString(Integer.toString(score),(int) x, (int)y);
	}

	@Override
	public void update() {
		
	}

	@Override
	public String toString() {
		return "Score";
	}

}
