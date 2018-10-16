package com.bluetoothdolly.yannickriou.bluetoothdollytimelapse;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class BtInterface {

	private BluetoothDevice device = null;
	private BluetoothSocket socket = null;
	private InputStream receiveStream = null;
	private OutputStream sendStream = null;
	
	private ReceiverThread receiverThread;

	Handler handler;

	public BtInterface(Handler hstatus, Handler h)
    {
		Set<BluetoothDevice> setpairedDevices = BluetoothAdapter.getDefaultAdapter().getBondedDevices();
		BluetoothDevice[] pairedDevices = (BluetoothDevice[]) setpairedDevices.toArray(new BluetoothDevice[setpairedDevices.size()]);
		
		for(int i=0;i<pairedDevices.length;i++) {
			if(pairedDevices[i].getName().contains("BlueMadz")) {
				device = pairedDevices[i];
				try {
					socket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
					receiveStream = socket.getInputStream();
					sendStream = socket.getOutputStream();
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
			}
		}

		handler = hstatus;
		
		receiverThread = new ReceiverThread(h);
	}
	
	public void sendData(String data) {
		sendData(data, false);
	}
	
	public void sendData(String data, boolean deleteScheduledData) {
		try {
			sendStream.write(data.getBytes());
	        sendStream.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

    public boolean isConnected()
    {
        return socket.isConnected();
    }


	public void connect() {
		new Thread() {
			@Override public void run() {
				try
                {
					socket.connect();
					receiverThread.start();
				}
                catch (IOException e) {
					Log.v("N", "Connection Failed : "+e.getMessage());
					e.printStackTrace();
				}
			}
		}.start();
	}

	public BluetoothDevice getDevice() {
		return device;
	}
	
	private class ReceiverThread extends Thread {
		Handler handler;
		
		ReceiverThread(Handler h) {
			handler = h;
		}
		
		@Override public void run() {
			while(true) {
				try {
                    int k = 0;
                    byte buffer[] = new byte[100];
                    while(receiveStream.available() > 0)
                    {

                        buffer[k] = (byte) receiveStream.read();

                        String s = new String(buffer, 0, k);

                        // Send the obtained bytes to the UI Activity
                        if ((buffer[k] == '\n')||(buffer[k]=='\r'))
                        {
                            Message msg = handler.obtainMessage();
                            Bundle b = new Bundle();
                            b.putString("receivedData", new String(buffer, 0, k));
                            msg.setData(b);
                            handler.sendMessage(msg);
                        }
                        else
                        {
                            k++;
                        }
					}

				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
}
