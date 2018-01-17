package com.giacca.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.view.View;
import android.widget.TextView;
import com.giacca.R;
import com.giacca.gui.ConnectionActivity;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;

/*
    TODO: Rendere indipendente la classe dall'activity
 */
public class BluetoothConnection {
    private BluetoothSocket mmSocket;
    private OutputStream outStream;
    private InputStream input;
    private BluetoothAdapter bluetoothAdapter;
    private boolean letto;
    private MyHandler h;
    private ConnectionActivity app;

    public BluetoothConnection (ConnectionActivity app){
        this.app = app;
        h = new MyHandler(app);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        mmSocket = null;
        outStream = null;
        input = null;
        letto = false;
    }

    public void connetti(String nome, String mac) {
        BluetoothDevice mmDevice = bluetoothAdapter.getRemoteDevice(mac);
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            mmSocket = (BluetoothSocket) mmDevice.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(mmDevice, 1);
        } catch (Exception e) {
            textView.setText(R.string.error);
        }
        try {
            mmSocket.connect();
            outStream = mmSocket.getOutputStream();
            input = mmSocket.getInputStream();
            String s = R.string.connected + nome;
            textView.setText(s);
            app.getInvia().setVisibility(View.VISIBLE);
            app.getE().setVisibility(View.VISIBLE);
        } catch (Exception e1) {
            textView.setText(R.string.error);
            try {
                mmSocket.close();
            } catch (Exception e2) {
                textView.setText(R.string.error);
            }
        }
    }

    public void disconnetti() {
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            mmSocket.close();
            outStream = null;
            textView.setText(R.string.disconnected);
        } catch (Exception e) {
            textView.setText(R.string.error);
        }
    }

    public boolean invia(String message) {
        byte[] msgBuffer = message.getBytes();
        TextView textView = new TextView(app);
        app.getViews().add(textView);
        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            textView.setText(R.string.error);
            return false;
        }
        byte[] buffer = new byte[256];
        int bytes;
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception ignored) {
        }
        while (!letto) {
            try {
                bytes = input.read(buffer);
                h.obtainMessage(1, bytes, -1, buffer).sendToTarget();
                letto = true;
            } catch (IOException ignored) {
            }
        }
        letto = false;
        return true;
    }
}
