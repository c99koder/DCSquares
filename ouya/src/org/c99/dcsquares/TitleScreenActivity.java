package org.c99.dcsquares;

import java.io.IOException;

import tv.ouya.console.api.OuyaController;

import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.KeyEvent;

public class TitleScreenActivity extends DCSquaresActivity {
	private MediaPlayer mp;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.title);
        init();
        try {
            mp = MediaPlayer.create(this, R.raw.title);
        	mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mp.setLooping(true);
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
    
    @Override
    public boolean onKeyDown(final int keyCode, KeyEvent event){
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());       
        boolean handled = false;

        //Handle the input
        switch(keyCode){
            case OuyaController.BUTTON_O:
				Intent i = new Intent(TitleScreenActivity.this, GameActivity.class);
				startActivity(i);
                handled = true;
                break;
        }
        return handled || super.onKeyDown(keyCode, event);
    }
    
	@Override
	protected void onPause() {
		super.onPause();
		mp.stop();
	}

	@Override
	public void onResume() {
		super.onResume();
		mp.start();
	}
}
