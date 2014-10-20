package org.c99.dcsquares;

import java.io.IOException;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;

public class TitleScreenActivity extends DCSquaresActivity {
	boolean stayConnected = false;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.title);
        GAME_HEIGHT = 400;
        init();
        
        findViewById(R.id.playBtn).setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				stayConnected = true;
				Intent i = new Intent(TitleScreenActivity.this, GameActivity.class);
				startActivity(i);
			}
        });
	}
    
	@Override
	protected void onPause() {
		super.onPause();
		if(!stayConnected) {
			try {
				DCSquaresApplication.getInstance().disconnectController();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	@Override
	public void onResume() {
		super.onResume();
		stayConnected = false;
	}
}
