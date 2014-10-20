package org.c99.dcsquares;

import android.app.Application;

public class DCSquaresApplication extends Application {
	private static DCSquaresApplication instance = null;

	public static DCSquaresApplication getInstance() {
		if(instance != null) {
			return instance;
		} else {
			return new DCSquaresApplication();
		}
	}

	@Override
	public void onCreate() {
		super.onCreate();
		instance = this;
		
	}
	
}
