package org.c99.dcsquares;

import tv.ouya.console.api.OuyaController;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.widget.TextView;

public class GameActivity extends DCSquaresActivity {
	Handler handler;
	TextView mScoreLabel;
	TextView mComboLabel;
	TextView mSquaresLabel;
	private MediaPlayer mp;
	
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
        try {
            mp = MediaPlayer.create(this, R.raw.game);
            mp.setVolume(0.6f, 0.6f);
        	mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mp.setLooping(true);
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
    
    final Runnable statsUpdate = new Runnable() {
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
		handler.removeCallbacksAndMessages(null);
		mp.stop();
	}

	@Override
	public void onResume() {
		super.onResume();
		handler.postDelayed(statsUpdate, 500);
		mp.start();
	}

	@Override
	public boolean onGenericMotionEvent(final MotionEvent event) {
	    //Get the player #
	    int playerNum = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());    

	    //Get all the axis for the event
	    float LS_X = event.getAxisValue(OuyaController.AXIS_LS_X);
	    float LS_Y = event.getAxisValue(OuyaController.AXIS_LS_Y);

	    player.dx = LS_X;
	    player.dy = LS_Y;
	    
	    //Do something with the input
	    //updatePlayerInput(player, LS_X, LS_Y, RS_X, RS_Y, L2, R2);

	    return true;
	}
}
