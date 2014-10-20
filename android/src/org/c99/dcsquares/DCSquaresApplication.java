package org.c99.dcsquares;

import java.io.IOException;

import com.zeemote.zc.Configuration;
import com.zeemote.zc.Controller;
import com.zeemote.zc.event.BatteryEvent;
import com.zeemote.zc.event.ControllerEvent;
import com.zeemote.zc.event.DisconnectEvent;
import com.zeemote.zc.event.IButtonListener;
import com.zeemote.zc.event.IJoystickListener;
import com.zeemote.zc.event.IStatusListener;
import com.zeemote.zc.ui.android.ControllerAndroidUi;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.util.Log;

public class DCSquaresApplication extends Application implements IStatusListener {
	private static DCSquaresApplication instance = null;
	private Controller controller;
	private ControllerAndroidUi controllerUi;
	private boolean controllerConnected = false;

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
		
		controller = new Controller(1);
		controller.addStatusListener(this);
	}
	
	public void disconnectController() throws IOException {
		controller.disconnect();
	}
	
	public void registerActivity(Activity a) {
		controller.addButtonListener((IButtonListener)a);
		controller.addJoystickListener((IJoystickListener)a);
	}
	
	public void unregisterActivity(Activity a) {
		controller.removeButtonListener((IButtonListener)a);
		controller.removeJoystickListener((IJoystickListener)a);
	}
	
	public void connectController(Context ctx) {
		controllerUi = new ControllerAndroidUi(ctx, controller);
		controllerUi.startConnectionProcess();
	}

	public void batteryUpdate(BatteryEvent event) {
		Log.d("Zeemote", "Battery Update: cur=" + event.getCurrentLevel() + ", max="
				+ event.getMaximumLevel() + ", warn=" + event.getWarningLevel()
				+ ", min=" + event.getMinimumLevel());
	}

	public void connected(ControllerEvent event) {
		controllerConnected = true;
		Configuration config = event.getController().getConfiguration();
		Log.i("Zeemote", "Connected to controller: "+
			"Num Buttons=" + config.getButtonCount()+
			", Num Joysticks=" + config.getJoystickCount());
	}

	public void disconnected(DisconnectEvent arg0) {
		controllerConnected = false;
	}
	
	public boolean isControllerConnected() {
		return controllerConnected;
	}
}
