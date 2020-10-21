package com.bluetoothdolly.yannickriou.bluetoothdollytimelapse;

import android.bluetooth.BluetoothAdapter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.cepheuen.elegantnumberbutton.view.ElegantNumberButton;

public class MainActivity extends AppCompatActivity implements OnClickListener
{
    private ElegantNumberButton finalTimelapseTimePicker;
    private ElegantNumberButton timeBetweenFramesPicker;
    private ElegantNumberButton exposureTimePicker;

	private ImageButton connect;
    private Button go;
    private Button stop;
    private Button pause;

    private SeekBar manualMoveDolly;
    private TextView manualMovePosition;
    private int manualPosition = 0;

    public ProgressBar timelapseStatus;
    private int increment=1;

    public TextView timelapseInfo;
    private TextView progressValue;

	private BtInterface bt = null;

    private boolean errorDetected = false;
    private boolean goForTimelapse = false;

    private boolean emergencyStop = false;
    private boolean pausing = false;
    private boolean resuming = false;
    private boolean running = false;

    private int nombreImages = 0;
    private int tempTotal = 0;

    private int timeBetweenFrames = 1;

    private int finalTimelapseTime = 1;

    private int exposureTime = 1;

    private final static int REQUEST_ENABLE_BT = 1;

	final Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            String data = msg.getData().getString("receivedData");
            if(data.contains("error"))
            {
                errorDetected = true;
                goForTimelapse = false;
                Toast.makeText(MainActivity.this,"ERROR!",Toast.LENGTH_SHORT).show();
            }
            else if(data.contains("k"))
            {
                errorDetected = false;
                goForTimelapse = true;

                Toast.makeText(MainActivity.this,"Starting Timelapse...",Toast.LENGTH_SHORT).show();
            }
        }
    };

    final Handler handlerStatus = new Handler() {
        public void handleMessage(Message msg) {
            Toast.makeText(MainActivity.this,"Received : " + msg.arg1, Toast.LENGTH_SHORT).show();
        }
    };

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!mBluetoothAdapter.isEnabled()) {
            android.content.Intent enableIntent = new android.content.Intent(
                    android.bluetooth.BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }

        bt = new BtInterface(handlerStatus, handler);

        connect = (ImageButton)findViewById(R.id.buttonConnect);
        connect.setOnClickListener(this);

        stop = (Button)findViewById(R.id.buttonStop);
        stop.setOnClickListener(this);

        go = (Button)findViewById(R.id.buttonGo);
        go.setOnClickListener(this);

        pause = (Button)findViewById(R.id.buttonPause);
        pause.setOnClickListener(this);

        timelapseInfo = (TextView) findViewById(R.id.timelapseInfo);

        progressValue = (TextView) findViewById(R.id.progressValue);

        timelapseStatus = (ProgressBar) findViewById(R.id.timelapseProgression);
        timelapseStatus.setIndeterminate(false);

        manualMoveDolly = (SeekBar) findViewById(R.id.manualMoveDolly);
        manualMoveDolly.setMax(2360);
        manualMoveDolly.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
        {
            int progressChanged = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser){
                progressChanged = progress;
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                bt.sendData("move"+progressChanged+"\n");
                manualMovePosition.setText((Math.round(((float)progressChanged/(float)2360)*100))+"/100");
            }
        });

        manualMovePosition = (TextView) findViewById(R.id.manualMovePosition);

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

                timelapseInfo.setText("Temp total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
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

                timelapseInfo.setText("Temp total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
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

                timelapseInfo.setText("Temp total : " + tempTotal / 60 + "min " + tempTotal % 60 + " sec" + "\nNombre d'images : " + nombreImages);
            }
        });


    }

	@Override
	public void onClick(View v)
    {
        if (v == connect)
        {
            bt.connect();
            while(!bt.isConnected());
            Toast.makeText(this,"Connection Successful!",Toast.LENGTH_SHORT).show();
        }
        else if (v == go)
        {
            bt.sendData("go"+nombreImages+","+timeBetweenFrames+","+exposureTime+"\n");

            go.setVisibility(View.INVISIBLE);
            stop.setVisibility(View.VISIBLE);
            pause.setVisibility(View.VISIBLE);
            timelapseStatus.setMax(nombreImages);
            timelapseStatus.setProgress(0);
            running = true;

            // create a thread for updating the progress bar
            Thread background = new Thread (new Runnable() {
                public void run() {
                    try {
                        // enter the code to be run while displaying the progressbar.
                        //
                        // This example is just going to increment the progress bar:
                        // So keep running until the progress value reaches maximum value
                        while (timelapseStatus.getProgress()<= timelapseStatus.getMax() && !emergencyStop)
                        {
                            if(!pausing)
                            {
                                // wait 500ms between each update
                                Thread.sleep((timeBetweenFrames + exposureTime) * 1000);

                                // active the update handler
                                progressHandler.sendMessage(progressHandler.obtainMessage());
                            }
                        }
                    } catch (java.lang.InterruptedException e) {
                        // if something fails do something smart
                    }
                }
            });

            background.start();

        }
        else if(v == stop)
        {
            emergencyStop = true;
            running = false;
            go.setVisibility(View.VISIBLE);
            stop.setVisibility(View.INVISIBLE);
            pause.setVisibility(View.INVISIBLE);
            bt.sendData("stop\n");
            Toast.makeText(this,"Stopped !",Toast.LENGTH_SHORT).show();
        }
        else if(v == pause)
        {

            pausing = !pausing;
            running = !pausing;
            bt.sendData("pause\n");
            Toast.makeText(this,"Paused !",Toast.LENGTH_SHORT).show();
        }
    }



    public void onSeekBarChangeListener()
    {

    }

    // handler for the background updating
    Handler progressHandler = new Handler() {
        public void handleMessage(Message msg) {
            timelapseStatus.incrementProgressBy(increment);
            progressValue.setText((Math.round(((float)timelapseStatus.getProgress() / (float)nombreImages)*100)) + "%");
        }
    };
}