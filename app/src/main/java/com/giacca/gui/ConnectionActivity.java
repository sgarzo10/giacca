package com.giacca.gui;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import com.giacca.R;
import com.giacca.bluetooth.BluetoothConnection;

public class ConnectionActivity extends AppCompatActivity {

    private BluetoothConnection bluetooth;
    private EditText e;
    private TextView t;
    private TextView txtLa;
    private TextView txtLo;

    public BluetoothConnection getBluetooth() { return bluetooth;}
    public EditText getE() {
        return e;
    }
    public TextView getT() {
        return t;
    }
    public TextView getTLa() {
        return txtLa;
    }
    public TextView getTLo() {
        return txtLo;
    }

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connection);
        Button invia = (Button) findViewById(R.id.invia);
        Button setData = (Button) findViewById(R.id.setData);
        Button setGps = (Button) findViewById(R.id.setGps);
        txtLa = (TextView) findViewById(R.id.txtLat);
        txtLo = (TextView) findViewById(R.id.txtLo);
        e = (EditText) findViewById(R.id.inserisci);
        t = (TextView) findViewById(R.id.txtViewLog);
        String nome = getIntent().getExtras().getString("nome");
        String mac = getIntent().getExtras().getString("mac");
        AscoltatoreConnectionActivity ascoltatore = new AscoltatoreConnectionActivity(this);
        bluetooth = new BluetoothConnection();
        invia.setOnClickListener(ascoltatore);
        setData.setOnClickListener(ascoltatore);
        setGps.setOnClickListener(ascoltatore);
        if (bluetooth.connetti(mac)){
            String s = R.string.connected + nome;
            t.setText(s);
            invia.setVisibility(View.VISIBLE);
            e.setVisibility(View.VISIBLE);
        }
        else
            t.setText(R.string.error);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    public void onStop() { super.onStop(); }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (bluetooth.disconnetti())
            t.setText(R.string.disconnected);
        else
            t.setText(R.string.error);
    }
}
