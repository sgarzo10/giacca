package com.example.admin.appg;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;

/**
 * Created by Admin on 28/12/2017.
 */

public class BluetoothConnection {
    private BluetoothSocket mmSocket = null;
    private OutputStream outStream = null;
    private InputStream input = null;
    private BluetoothAdapter bluetoothAdapter;
    private boolean letto = false;
    private MyHandler h;
    private Page1 app;

    public BluetoothConnection (Page1 app){
        this.app = app;
        h = new MyHandler(app);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    public void connetti(String nome, String mac) {
        BluetoothDevice mmDevice = bluetoothAdapter.getRemoteDevice(mac);
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            mmSocket = (BluetoothSocket) mmDevice.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(mmDevice, 1);
        } catch (Exception e) {
            textView.setText("Errore1");
        }
        try {
            mmSocket.connect();
            outStream = mmSocket.getOutputStream();
            input = mmSocket.getInputStream();
            textView.setText("Connesso con: " + nome);
            app.getInvia().setVisibility(View.VISIBLE);
            app.getE().setVisibility(View.VISIBLE);
        } catch (Exception e1) {
            textView.setText("Errore 2");
            try {
                mmSocket.close();
            } catch (Exception e2) {
                textView.setText("Errore 3");
            }
        }
    }

    public void disconnetti() {
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            mmSocket.close();
            outStream = null;
            textView.setText("Disconnesso");
        } catch (Exception e) {
            textView.setText("Errore");
        }
    }

    public boolean invia(String message) {
        byte[] msgBuffer = message.getBytes();
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            textView.setText("Errore");
            return false;
        }
        byte[] buffer = new byte[256];  // buffer store for the stream
        int bytes; // bytes returned from read()
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {
        }
        while (!letto) {
            try {
                // Read from the InputStream
                bytes = input.read(buffer);        // Get number of bytes and message in "buffer"
                h.obtainMessage(1, bytes, -1, buffer).sendToTarget();     // Send to message queue Handler
                letto = true;
            } catch (IOException e) {
            }
        }
        letto = false;
        return true;
    }
}
