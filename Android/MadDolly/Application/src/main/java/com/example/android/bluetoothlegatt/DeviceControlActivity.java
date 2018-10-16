/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.TextView;

import com.cepheuen.elegantnumberbutton.view.ElegantNumberButton;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    public static final String SERVICE_UUID = "71d22714-025b-424d-9ae4-25680cbb2be0";
    public static final String EXPO_TIME_UUID    = "7fe2924b-4b9b-4262-b281-7b3c1bd054dd";
    public static final String DELAY_TIME_UUID   = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
    public static final String NB_IMAGE_UUID     = "1752f449-8bbc-4fd3-921a-5d0a2eacf612";
    public static final String PLAYPAUSE_UUID    = "1ac17b1f-60d1-4f90-a6e2-c7d57baa7e09";

    public static final String SERVICE_STEPPER_UUID = "9c944cbb-68e4-48b0-abca-91a57b2a130c";
    public static final String  CURRENT_POS_UUID      = "81b4689f-bcd2-47b5-9783-09a15e9811ee";
    public static final String DIR_UUID    = "f10fa210-b70d-44f2-b20f-ce713c789b08";



    private TextView mConnectionState;
    private Button mPlayPause;
    private SeekBar mManualMoveDolly;
    private boolean mplayPauseState;

    private RadioButton avancer;
    private RadioButton reculer;
    private int dir = 0;

    private ElegantNumberButton finalTimelapseTimePicker;
    private ElegantNumberButton timeBetweenFramesPicker;
    private ElegantNumberButton exposureTimePicker;

    private int nombreImages = 0;
    private int tempTotal = 0;

    private int timeBetweenFrames = 1;

    private int finalTimelapseTime = 1;

    private int exposureTime = 1;

    public TextView timelapseInfo;
    private TextView progressValue;

    private String mDeviceName;
    private String mDeviceAddress;
    private BluetoothLeService mBluetoothLeService;
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;


    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                //displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };


    private void clearUI() {
        //mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        //mDataField.setText(R.string.no_data);
    }

    Runnable navHider = new Runnable() {
        @Override
        public void run() {
            Log.e("HIDING", "hide");
            hideSystemUi();
        }
    };

        /** Hides StatusBar and NavigationBar */
        private void hideSystemUi() {
                // Set flags for hiding status bar and navigation bar
                View decorView = getWindow().getDecorView();
                // Hide the status bar.
                int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE;
                decorView.setSystemUiVisibility(uiOptions);
        }

        private void setVisibilityChangeListener() {
            final View decorView = getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener() {
                        @Override
                        public void onSystemUiVisibilityChange(int visibility) {
                            // Note that system bars will only be "visible" if none
                            // of the
                            // LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags are
                            // set.
                                if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                                    decorView.getHandler().postDelayed(navHider,2000);
                            }
                        }
                    });
        }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.maincontrol);

        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LOW_PROFILE
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE);

        // Auto hide navigation bar
        setVisibilityChangeListener();


        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        /*
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
        mGattServicesList = (ExpandableListView) findViewById(R.id.gatt_services_list);
        mGattServicesList.setOnChildClickListener(servicesListClickListner);
    mConnectionState = (TextView) findViewById(R.id.connection_state);
    */

        timelapseInfo = (TextView) findViewById(R.id.timelapseInfo);
        mPlayPause = findViewById(R.id.playpause);

        mManualMoveDolly = findViewById(R.id.mManualMoveDolly);
        mManualMoveDolly.setMax(100);
        mManualMoveDolly.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
        {
            int progressChanged = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser){
                progressChanged = progress;

            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                mBluetoothLeService.writeCustomCharacteristic(SERVICE_UUID,CURRENT_POS_UUID,Integer.toString(progressChanged));
                //manualMovePosition.setText((Math.round(((float)progressChanged/(float)2360)*100))+"/100");
            }
        });

        finalTimelapseTimePicker = (ElegantNumberButton)findViewById(R.id.nbSecFinal);
        finalTimelapseTimePicker.setRange(1,20);

        finalTimelapseTimePicker.setOnValueChangeListener(new ElegantNumberButton.OnValueChangeListener() {
            public void onValueChange (ElegantNumberButton picker,int oldVal, int newVal)
            {
                finalTimelapseTime = newVal;

                nombreImages = finalTimelapseTime * 24;

                tempTotal = nombreImages * (timeBetweenFrames + exposureTime);

                timelapseInfo.setText("Temps total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
            }
        });

        avancer = (RadioButton)findViewById(R.id.avancer);
        reculer = (RadioButton)findViewById(R.id.reculer);

        timeBetweenFramesPicker = (ElegantNumberButton)findViewById(R.id.timeBetweenFrames);
        timeBetweenFramesPicker.setRange(1,20);

        timeBetweenFramesPicker.setOnValueChangeListener(new ElegantNumberButton.OnValueChangeListener() {
            public void onValueChange (ElegantNumberButton picker,int oldVal, int newVal)
            {
                if (picker == timeBetweenFramesPicker) {
                    timeBetweenFrames = newVal;
                } else if (picker == finalTimelapseTimePicker) {
                    finalTimelapseTime = newVal;
                } else if (picker == exposureTimePicker) {
                    exposureTime = newVal;
                }

                nombreImages = finalTimelapseTime * 24;

                tempTotal = nombreImages * (timeBetweenFrames + exposureTime);

                timelapseInfo.setText("Temps total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
            }
        });

        exposureTimePicker = (ElegantNumberButton) findViewById(R.id.exposureTime);
        exposureTimePicker.setRange(1,30);

        exposureTimePicker.setOnValueChangeListener(new ElegantNumberButton.OnValueChangeListener() {
            public void onValueChange (ElegantNumberButton picker,int oldVal, int newVal)
            {
                exposureTime = newVal;

                nombreImages = finalTimelapseTime * 24;

                tempTotal = nombreImages * (timeBetweenFrames + exposureTime);

                timelapseInfo.setText("Temps total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
            }
        });


        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        //unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(String data) {
        if (data != null) {
            //mDataField.setText(data);
        }
    }


    public void onClickWrite(View v){
        if(mBluetoothLeService != null) {
            //mBluetoothLeService.writeCustomCharacteristic(123);
        }
    }

    public void onClickRead(View v){
    }

    public void onRadioButtonClicked(View view) {
        if (avancer.isChecked())
        {
            if (mBluetoothLeService != null) {
                mBluetoothLeService.writeCustomCharacteristic(SERVICE_STEPPER_UUID, DIR_UUID, "0");
            }
        }
       else if (reculer.isChecked())
        {
            if(mBluetoothLeService != null) {
                mBluetoothLeService.writeCustomCharacteristic(SERVICE_STEPPER_UUID,DIR_UUID,"1");
            }
        }
    }

    public void onClickPlayPause(View v){
      if(mplayPauseState)
      {
          mPlayPause.setBackgroundResource(R.drawable.play);
          mplayPauseState = !mplayPauseState;
          if(mBluetoothLeService != null) {
              mBluetoothLeService.writeCustomCharacteristic(SERVICE_UUID,PLAYPAUSE_UUID,"0");
          }
      }
      else
      {
          mPlayPause.setBackgroundResource(R.drawable.pause);
          mplayPauseState = !mplayPauseState;
          if(mBluetoothLeService != null) {
              mBluetoothLeService.writeCustomCharacteristic(SERVICE_UUID,PLAYPAUSE_UUID,"1");
          }
      }
    }
}
