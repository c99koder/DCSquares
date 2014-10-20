package org.c99.dcsquares;

import java.io.IOException;

import javax.microedition.khronos.opengles.GL10;

import com.zeemote.zc.event.ButtonEvent;
import com.zeemote.zc.event.IButtonListener;
import com.zeemote.zc.event.IJoystickListener;
import com.zeemote.zc.event.JoystickEvent;

import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;

public class GameActivity extends DCSquaresActivity implements IButtonListener, IJoystickListener {
	private boolean controllerUiShown = false;

	Handler handler;
	TextView mScoreLabel;
	TextView mComboLabel;
	TextView mSquaresLabel;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game);
        mScoreLabel = (TextView) findViewById(R.id.scoreLabel);
        mComboLabel = (TextView) findViewById(R.id.comboLabel);
        mSquaresLabel = (TextView) findViewById(R.id.squaresLabel);
        
        init();
        player = new Square();
        player.type = TYPE_PLAYER;
        player.x = GAME_WIDTH / 2;
        player.y = GAME_HEIGHT / 2;
        player.size = 20;
        player.dx = 0;
        player.dy = 0;
        
        handler = new Handler();
	}
    
    final Runnable statsUpdate = new Runnable()
    {
        public void run() 
        {
            handler.postDelayed(this, 500);
        	mScoreLabel.setText(String.valueOf(score));
        	mComboLabel.setText(String.valueOf(combo));
        	mSquaresLabel.setText(String.valueOf(squares));
        }
    };

    @Override
	protected void onPause() {
		super.onPause();
		DCSquaresApplication.getInstance().unregisterActivity(this);
		handler.removeCallbacksAndMessages(null);
	}

	@Override
	public void onResume() {
		super.onResume();
		DCSquaresApplication.getInstance().registerActivity(this);
		if(!controllerUiShown && !DCSquaresApplication.getInstance().isControllerConnected()) {
			DCSquaresApplication.getInstance().connectController(this);
			controllerUiShown = true;
		} else {
			controllerUiShown = false;
		}
		handler.postDelayed(statsUpdate, 500);
	}

	public void joystickMoved(JoystickEvent e) {
		// TODO Auto-generated method stub
		int x = e.getScaledX(-100, 100);
		int y = e.getScaledY(-100, 100);
		
		player.dx = x / 100f;
		player.dy = y / 100f;
	}

	public void buttonPressed(ButtonEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	public void buttonReleased(ButtonEvent arg0) {
		// TODO Auto-generated method stub
		
	}

}
