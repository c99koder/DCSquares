package org.c99.dcsquares;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Random;
import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;
import javax.microedition.khronos.opengles.GL11Ext;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.media.AudioManager;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.util.Log;
import android.view.WindowManager;

public class DCSquaresActivity extends Activity  implements GLSurfaceView.Renderer {
	private GLSurfaceView mGLView;

	private static final String TAG = "DCSquaresActivity";
	public static final int TYPE_PLAYER = 0;
	public static final int TYPE_SCORE = 1;
	public static final int TYPE_ENEMY = 2;
	public static final int TYPE_POWERUP = 3;
	public static final int TYPE_POWERDOWN = 4;
	public static final int TYPE_INVINC = 10;
	public static final int TYPE_SLOWMO = 11;
	public static final int TYPE_MINISQUARE = 12;
	public static final int TYPE_PLUS1000 = 13;
	public static final int TYPE_EVIL = 14;
	public static final int TYPE_SPEED = 15;
	public static final int TYPE_BIGSQUARE = 16;
	public static final int TYPE_MINUS1000 = 17;
	
	protected int GAME_WIDTH = 640;
	protected int GAME_HEIGHT = 460;
	
	private Random rnd;
	private long last_time_redraw = System.currentTimeMillis();
	private long spawn_timer = 0;
	
	private int viewportWidth = GAME_WIDTH;
	private int viewportHeight = GAME_HEIGHT;
	
	private Texture bgTex;
	private Texture playerTex;
	private Texture scoreTex;
	private Texture enemyTex;
	private Texture invincTex;
	private Texture slowmoTex;
	private Texture minisquareTex;
	private Texture plus1000Tex;
	private Texture evilTex;
	private Texture speedTex;
	private Texture bigsquareTex;
	private Texture minus1000Tex;
	
	private SoundPool soundPool;
	private int snd_score;
	private int snd_powerup;
	private int snd_powerdown;
	private int snd_gameover;
	
	protected ArrayList<Square> mSquares;
	
	protected float speed = 3;
	protected int spawnInterval = 800;
	protected int powerType = 0;
	protected int power = 0;
	protected Square player = null;
	protected int score = 0;
	protected int squares = 0;
	protected int combo = 0;
	
	private FloatBuffer spriteVB;
	private FloatBuffer spriteTC;
	
    protected void init() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        mGLView = (GLSurfaceView)findViewById(R.id.squaresView);
        mGLView.setRenderer(this);

        mSquares = new ArrayList<Square>();
        rnd = new Random(System.currentTimeMillis());
        
        soundPool = new SoundPool(2, AudioManager.STREAM_MUSIC, 0);
        snd_score = soundPool.load(this, R.raw.collect, 1);
        snd_powerup = soundPool.load(this, R.raw.powerup, 1);
        snd_powerdown = soundPool.load(this, R.raw.powerdown, 1);
        snd_gameover = soundPool.load(this, R.raw.gameover, 1);
        
        float coords[] = {
    		-1, -1,
    		1, -1,
    		-1,  1,
    		1,  1
        }; 
        
        // initialize vertex Buffer for sprite
        ByteBuffer vbb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
        		coords.length * 4); 
        vbb.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        spriteVB = vbb.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        spriteVB.put(coords);    // add the coordinates to the FloatBuffer
        spriteVB.position(0);            // set the buffer to read the first coordinate

        float tc[] = {
    		0, 0,
    		1, 0,
    		0, 1,
    		1, 1
        }; 
        
        // initialize vertex Buffer for sprite
        vbb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
        		coords.length * 4); 
        vbb.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        spriteTC = vbb.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        spriteTC.put(tc);    // add the coordinates to the FloatBuffer
        spriteTC.position(0);            // set the buffer to read the first coordinate
}
    
	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
	}

	@Override
	public void onResume() {
		super.onResume();
        mGLView.onResume();
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Set the background frame color
        gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        bgTex = new Texture(gl);
        bgTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.game_bg));
        playerTex = new Texture(gl);
        playerTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.player));
        scoreTex = new Texture(gl);
        scoreTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.score));
        enemyTex = new Texture(gl);
        enemyTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.enemy));
        invincTex = new Texture(gl);
        invincTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.invincible));
        slowmoTex = new Texture(gl);
        slowmoTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.slowdown));
        minisquareTex = new Texture(gl);
        minisquareTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.smallsquare));
        plus1000Tex = new Texture(gl);
        plus1000Tex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.plus1000));
        evilTex = new Texture(gl);
        evilTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.evil));
        speedTex = new Texture(gl);
        speedTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.speedup));
        bigsquareTex = new Texture(gl);
        bigsquareTex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.bigsquare));
        minus1000Tex = new Texture(gl);
        minus1000Tex.Load(BitmapFactory.decodeResource(getResources(), R.drawable.minus1000));
    }
    
    public void onDrawFrame(GL10 gl) {
        long elapsedms = System.currentTimeMillis() - last_time_redraw;
        Rect p = null;
        if(player != null)
        	p = new Rect((int)(player.x - player.size)+4, (int)(player.y - player.size)+4, (int)(player.x)-4, (int)(player.y)-4);
        // Redraw background color
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);

        gl.glBindTexture(GL10.GL_TEXTURE_2D, bgTex.texture[0]);
        ((GL11Ext) gl).glDrawTexfOES(0,0,0,viewportWidth,viewportHeight);
        
        spawn_timer += elapsedms;
    	if(spawn_timer > spawnInterval) {
    		addSquare(40 + (rnd.nextInt(4) * 4));
    		spawn_timer = 0;
    	}
        
		Iterator<Square> i = new ArrayList<Square>(mSquares).iterator();
		while(i.hasNext()) {
			Square s = i.next();
			s.update(elapsedms);
			if((s.dx > 0 && s.x > (GAME_WIDTH + s.size)) || 
					(s.dx < 0 && s.x < -(s.size)) ||
					(s.dy > 0 && s.y > (GAME_HEIGHT + s.size)) ||
					(s.dy < 0 && s.y < -(s.size))) {
				if(s.type == TYPE_SCORE)
					combo = 0;
				mSquares.remove(s);
				continue;
			}
			if(player != null && p.intersects((int)(s.x - s.size), (int)(s.y - s.size), (int)(s.x + s.size), (int)(s.y + s.size))) {
				switch(s.type) {
					case TYPE_SCORE:
						score += 100;
						combo++;
						squares++;
						soundPool.play(snd_score, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_ENEMY:
						soundPool.play(snd_gameover, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_INVINC:
						soundPool.play(snd_powerup, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_SLOWMO:
						soundPool.play(snd_powerup, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_MINISQUARE:
						soundPool.play(snd_powerup, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_PLUS1000:
						score += 1000;
						soundPool.play(snd_powerup, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_EVIL:
						soundPool.play(snd_powerdown, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_SPEED:
						soundPool.play(snd_powerdown, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_BIGSQUARE:
						soundPool.play(snd_powerdown, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
					case TYPE_MINUS1000:
						score -= 1000;
						soundPool.play(snd_powerdown, 0.8f, 0.8f, 0, 0, 1.0f);
						break;
				}
				mSquares.remove(s);
				continue;
			}
			s.draw(gl);
		}
		
		if(player != null) {
			player.update(elapsedms);
			player.draw(gl);
		}
		last_time_redraw = System.currentTimeMillis();
   }
    
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    	viewportWidth = width;
    	viewportHeight = height;
        gl.glViewport(0, 0, viewportWidth, viewportHeight);
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		gl.glOrthof(0, GAME_WIDTH, GAME_HEIGHT, 0, -1.0f, 1.0f);
		
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, spriteVB);
		gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, spriteVB);
		
		gl.glEnable(GL10.GL_TEXTURE_2D);
		gl.glBlendFunc(GL10.GL_ONE, GL10.GL_ONE_MINUS_SRC_ALPHA);
		gl.glEnable(GL10.GL_BLEND);
    }
    
	protected class Square {
		float x;
		float y;
		float dx;
		float dy;
		float rot = 0;
		int size;
		int type;
		
		void update(long elapsedms) {
			float speedMod = (type == TYPE_PLAYER)?2.0f:1.0f;
			
			x += (dx * speed * speedMod) * (elapsedms / 16.0f);
			y += (dy * speed * speedMod) * (elapsedms / 16.0f);
			
			if(type == TYPE_PLAYER) {
				rot+=(elapsedms / 5.0f);
				if(rot >= 360)
					rot = 0;
			}
		}
		
		void draw(GL10 gl) {
			Texture t = null;
			switch(type) {
    			case TYPE_PLAYER:
    				t = playerTex;
    				break;
    			case TYPE_SCORE:
    				t = scoreTex;
    				break;
    			case TYPE_ENEMY:
    				t = enemyTex;
    				break;
    			case TYPE_INVINC:
    				t = invincTex;
    				break;
    			case TYPE_SLOWMO:
    				t = slowmoTex;
    				break;
    			case TYPE_MINISQUARE:
    				t = minisquareTex;
    				break;
    			case TYPE_PLUS1000:
    				t = plus1000Tex;
    				break;
    			case TYPE_EVIL:
    				t = evilTex;
    				break;
    			case TYPE_SPEED:
    				t = speedTex;
    				break;
    			case TYPE_BIGSQUARE:
    				t = bigsquareTex;
    				break;
    			case TYPE_MINUS1000:
    				t = minus1000Tex;
    				break;
			}
			if(t != null) {
				gl.glPushMatrix();
		        gl.glBindTexture(GL10.GL_TEXTURE_2D, t.texture[0]);
				gl.glTranslatef(x, y, 0);
				gl.glRotatef(rot, 0, 0, 1);
				gl.glScalef(size, size, 0.0f);
				gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);
				gl.glPopMatrix();
			}

		}
	};
	
	private void addSquare(int size) {
		int max = 2;
		
		if(rnd.nextInt(8)==0) max=4; else max=2;
		Square s = new Square();
		s.size = size;
		s.type = 1 + rnd.nextInt(max);

		if(s.type == TYPE_POWERUP) {
			s.type = 10 + rnd.nextInt(4);
			s.size = 48;
		} else if(s.type == TYPE_POWERDOWN) {
			s.type = 14 + rnd.nextInt(4);
			s.size = 48;
		}
		
		switch(rnd.nextInt(4)) {
			case 0:
				s.x = -size;
				s.y = rnd.nextInt(GAME_HEIGHT - (size * 2));
				s.dx = 1;
				s.dy = 0;
				break;
			case 1:
				s.x = GAME_WIDTH + size;
				s.y = rnd.nextInt(GAME_HEIGHT - (size * 2));
				s.dx = -1;
				s.dy = 0;
				break;
			case 2:
				s.x = rnd.nextInt(GAME_WIDTH - (size * 2));
				s.y = -size;
				s.dx = 0;
				s.dy = 1;
				break;
			case 3:
				s.x = rnd.nextInt(GAME_WIDTH - (size * 2));
				s.y = GAME_HEIGHT + size;
				s.dx = 0;
				s.dy = -1;
				break;
		}
		
		mSquares.add(s);
	}
	
	public class Texture {
	    /*Begin public declarations*/
	    public float x = 0;
	    public float y = 0;
	    public float z = 0;
	    public float width = 0;
	    public float height = 0;
	    /*Begin Private Declarations*/
	    private GL10 gl;
	    public int[] texture;    //holds the texture in integer form
	    private int texture_name;
	    private int[] mCropWorkspace;
	    private final BitmapFactory.Options sBitmapOptions;


		/*Begin Methods*/
		public Texture( GL10 gl_obj )
		{
		    gl = gl_obj;
		    texture = new int[1];
		    mCropWorkspace = new int[4];
		    sBitmapOptions = new BitmapFactory.Options();
		    sBitmapOptions.inPreferredConfig = Bitmap.Config.RGB_565;
		}    
		public int get_texture_name( )
		{
		    return texture_name;
		}
	
		/*Loads the resource to memory*/
		public boolean Load( Bitmap bitmap ) //rename this to glLoad and don't have it as an initializer parameter
		{
		    /*many thanks to sprite method test if this works*/
		    if ( gl == null )
		    {
		        Log.e(TAG, "Failed to load resource.  Context/GL is NULL");
		        return false;
		    }
		    int error;
	
		    int textureName = -1;
		    gl.glGenTextures(1, texture, 0);
		    textureName = texture[0];
	
		    gl.glBindTexture(GL10.GL_TEXTURE_2D, textureName);
		    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_NEAREST);
		    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
		    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
		    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);
		    gl.glTexEnvf(GL10.GL_TEXTURE_ENV, GL10.GL_TEXTURE_ENV_MODE, GL10.GL_REPLACE);
	
		    GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
	
		    mCropWorkspace[0] = 0;
		    mCropWorkspace[1] = bitmap.getHeight();
		    mCropWorkspace[2] = bitmap.getWidth();
		    mCropWorkspace[3] = -bitmap.getHeight();
	
		    ((GL11) gl).glTexParameteriv(GL10.GL_TEXTURE_2D, 
		            GL11Ext.GL_TEXTURE_CROP_RECT_OES, mCropWorkspace, 0);
	
		    error = gl.glGetError();
		    if (error != GL10.GL_NO_ERROR)
		    { 
		        Log.e(TAG, "GL Texture Load Error: " + error);
	
		    }
		    return true;
		}
	}
}